#include "Memory.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include <iostream>
#include <algorithm>

#pragma comment(lib, "psapi.lib")


uintptr_t Memory::GetModuleBase(const std::string& moduleName)
{
    MODULEENTRY32 moduleInfo = GetModuleInfo(moduleName);
    return moduleInfo.modBaseAddr ? reinterpret_cast<uintptr_t>(moduleInfo.modBaseAddr) : 0;
}

uintptr_t Memory::GetMainModuleBase()
{
    return reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
}

size_t Memory::GetModuleSize(const std::string& moduleName)
{
    MODULEENTRY32 moduleInfo = GetModuleInfo(moduleName);
    return moduleInfo.modBaseSize;
}

MODULEENTRY32 Memory::GetModuleInfo(const std::string& moduleName)
{
    MODULEENTRY32 modEntry = { 0 };

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return modEntry;

    modEntry.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnapshot, &modEntry))
    {
        do
        {
            if (moduleName.empty() || _stricmp(modEntry.szModule, moduleName.c_str()) == 0)
            {
                CloseHandle(hSnapshot);
                return modEntry;
            }
        } while (Module32Next(hSnapshot, &modEntry));
    }

    CloseHandle(hSnapshot);
    return { 0 };
}

template<typename T>
T Memory::Read(uintptr_t address)
{
    T value;
    ReadRaw(address, &value, sizeof(T));
    return value;
}

template<typename T>
bool Memory::Write(uintptr_t address, const T& value)
{
    return WriteRaw(address, &value, sizeof(T));
}

bool Memory::ReadRaw(uintptr_t address, void* buffer, size_t size)
{
    return InternalRead(address, buffer, size);
}

bool Memory::WriteRaw(uintptr_t address, const void* buffer, size_t size)
{
    return InternalWrite(address, buffer, size);
}

std::string Memory::ReadString(uintptr_t address, size_t maxLength)
{
    std::string result;
    result.resize(maxLength);

    if (ReadRaw(address, result.data(), maxLength))
    {
        result.resize(strlen(result.c_str()));
        return result;
    }

    return "";
}

std::wstring Memory::ReadWString(uintptr_t address, size_t maxLength)
{
    std::wstring result;
    result.resize(maxLength);

    if (ReadRaw(address, result.data(), maxLength * sizeof(wchar_t)))
    {
        result.resize(wcslen(result.c_str()));
        return result;
    }

    return L"";
}


std::vector<int> Memory::PatternToIntVector(const char* Pattern)
{
    std::vector<int> PatternVector = std::vector<int>{};

    char* Start = const_cast<char*>(Pattern);
    char* End = const_cast<char*>(Pattern) + strlen(Pattern);

    for (char* Current = Start; Current < End; ++Current)
    {
        if (*Current == '?')
        {
            ++Current;
            PatternVector.push_back(-69);
        }
        else {
            PatternVector.push_back(strtoul(Current, &Current, 16));
        }
    }

    return PatternVector;
}

inline void* Memory::FindPointer(std::vector<int> Pattern, uint8_t* Address, int Addition)
{
    int i = 0;
    while (Pattern[i] != -69) i++;

    int Offset = *reinterpret_cast<int*>(Address + i);
    return reinterpret_cast<void*>(Address + i + 0x4 + Offset + Addition);
}

template<typename T>
T Memory::PatternScan(const char* Pattern, uint64_t Start, size_t Size, bool bFindPointer, int Addition)
{
    std::vector<int> PatternVector = PatternToIntVector(Pattern);

    uint8_t* Search = reinterpret_cast<uint8_t*>(Start);
    const size_t SizeOfPattern = PatternVector.size();
    const int* PatternData = PatternVector.data();

    for (int i = 0; i < Size - SizeOfPattern; ++i)
    {
        bool FoundSignature = true;

        for (int j = 0; j < SizeOfPattern; ++j)
        {
            if (Search[i + j] != PatternData[j] && PatternData[j] != -69)
            {
                FoundSignature = false;
                break;
            }
        }

        if (FoundSignature)
        {
            if (bFindPointer)
                return reinterpret_cast<T>(FindPointer(PatternVector, &Search[i], Addition));

            return reinterpret_cast<T>(&Search[i]);
        }
    }

    return reinterpret_cast<T>(nullptr);
}

template<typename T>
std::vector<T> Memory::PatternScanAll(const char* Pattern, uint64_t Start, size_t Size, bool bFindPointer, int Addition)
{
    std::vector<int> PatternVector = PatternToIntVector(Pattern);
    std::vector<T> Results;

    uint8_t* Search = reinterpret_cast<uint8_t*>(Start);
    const size_t SizeOfPattern = PatternVector.size();
    const int* PatternData = PatternVector.data();

    for (size_t i = 0; i <= Size - SizeOfPattern; ++i)
    {
        bool FoundSignature = true;

        for (size_t j = 0; j < SizeOfPattern; ++j)
        {
            if (Search[i + j] != PatternData[j] && PatternData[j] != -69)
            {
                FoundSignature = false;
                break;
            }
        }

        if (FoundSignature)
        {
            T FoundAddress = bFindPointer ? reinterpret_cast<T>(FindPointer(PatternVector, &Search[i], Addition)) : reinterpret_cast<T>(&Search[i]);
            Results.push_back(FoundAddress);
        }
    }

    return Results;
}

int Memory::ScanVTable(std::string Pattern, void* Object, int SkipAmount)
{
    int Out_Index = 0;
    int FunctionCount = 0;

    void** VTable = *reinterpret_cast<void***>(Object);
    while (VTable[FunctionCount]) FunctionCount++;

    for (int Index = SkipAmount; Index <= FunctionCount; Index++)
    {
        void* Result = PatternScan<void*>(Pattern.c_str(), reinterpret_cast<uint64_t>(VTable[Index]), Pattern.size());
        if (!Result) continue;

        Out_Index = Index;
        break;
    }

    return Out_Index;
}


template<typename T>
T Memory::PatternScanModule(const char* pattern, const std::string& moduleName, bool findPointer, int addition)
{
    uintptr_t moduleBase = GetModuleBase(moduleName);
    size_t moduleSize = GetModuleSize(moduleName);

    if (moduleBase && moduleSize > 0)
        return PatternScan<T>(pattern, moduleBase, moduleSize, findPointer, addition);

    return nullptr;
}

template<typename T>
T Memory::PatternScanMainModule(const char* pattern, bool findPointer, int addition)
{
    uintptr_t moduleBase = GetMainModuleBase();

    MODULEINFO modInfo;
    if (GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(moduleBase), &modInfo, sizeof(modInfo)))
        return PatternScan<T>(pattern, moduleBase, modInfo.SizeOfImage, findPointer, addition);

    return nullptr;
}

template<typename T>
std::vector<T> Memory::PatternScanModuleAll(const char* pattern, const std::string& moduleName, bool findPointer, int addition)
{
    uintptr_t moduleBase = GetModuleBase(moduleName);
    size_t moduleSize = GetModuleSize(moduleName);

    if (moduleBase && moduleSize > 0)
        return PatternScanAll<T>(pattern, moduleBase, moduleSize, findPointer, addition);

    return {};
}


uintptr_t Memory::GetAbsoluteAddress(uintptr_t instructionPtr, int offset, int instructionSize)
{
    return instructionPtr + instructionSize + *reinterpret_cast<int*>(instructionPtr + offset);
}

uintptr_t Memory::FollowRelativeJump(uintptr_t address)
{
    uint8_t opcode = Read<uint8_t>(address);

    if (opcode == 0xE9) // JMP relative :p
    {
        int32_t relativeOffset = Read<int32_t>(address + 1);
        return address + 5 + relativeOffset;
    }
    else if (opcode == 0xE8) // CALL relative :D
    {
        int32_t relativeOffset = Read<int32_t>(address + 1);
        return address + 5 + relativeOffset;
    }

    return address; // not an relative jump or call :)
}

bool Memory::IsValidPointer(uintptr_t address)
{
    if (address == 0 || address < 0x10000 || address > 0x7FFFFFFF0000)
        return false;

    MEMORY_BASIC_INFORMATION mbi;
    return VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi)) &&
        (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE));
}

bool Memory::InternalRead(uintptr_t address, void* buffer, size_t size)
{
    if (!IsValidPointer(address))
        return false;

    memcpy(buffer, reinterpret_cast<void*>(address), size);
    return true;
}

bool Memory::InternalWrite(uintptr_t address, const void* buffer, size_t size)
{
    if (!IsValidPointer(address))
        return false;

    DWORD oldProtect;
    if (VirtualProtect(reinterpret_cast<void*>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        memcpy(reinterpret_cast<void*>(address), buffer, size);
        VirtualProtect(reinterpret_cast<void*>(address), size, oldProtect, &oldProtect);
        return true;
    }

    return false;
}

template int8_t Memory::Read<int8_t>(uintptr_t);
template int16_t Memory::Read<int16_t>(uintptr_t);
template int32_t Memory::Read<int32_t>(uintptr_t);
template int64_t Memory::Read<int64_t>(uintptr_t);
template uint8_t Memory::Read<uint8_t>(uintptr_t);
template uint16_t Memory::Read<uint16_t>(uintptr_t);
template uint32_t Memory::Read<uint32_t>(uintptr_t);
template uint64_t Memory::Read<uint64_t>(uintptr_t);
template float Memory::Read<float>(uintptr_t);
template double Memory::Read<double>(uintptr_t);
template bool Memory::Read<bool>(uintptr_t);

template bool Memory::Write<int8_t>(uintptr_t, const int8_t&);
template bool Memory::Write<int16_t>(uintptr_t, const int16_t&);
template bool Memory::Write<int32_t>(uintptr_t, const int32_t&);
template bool Memory::Write<int64_t>(uintptr_t, const int64_t&);
template bool Memory::Write<uint8_t>(uintptr_t, const uint8_t&);
template bool Memory::Write<uint16_t>(uintptr_t, const uint16_t&);
template bool Memory::Write<uint32_t>(uintptr_t, const uint32_t&);
template bool Memory::Write<uint64_t>(uintptr_t, const uint64_t&);
template bool Memory::Write<float>(uintptr_t, const float&);
template bool Memory::Write<double>(uintptr_t, const double&);
template bool Memory::Write<bool>(uintptr_t, const bool&);
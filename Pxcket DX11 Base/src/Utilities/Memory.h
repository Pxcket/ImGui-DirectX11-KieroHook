#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <Windows.h>
#include <TlHelp32.h>

class Memory
{
public:

    Memory() = delete;
    ~Memory() = delete;


    static uintptr_t GetModuleBase(const std::string& moduleName);
    static uintptr_t GetMainModuleBase();
    static size_t GetModuleSize(const std::string& moduleName);
    static MODULEENTRY32 GetModuleInfo(const std::string& moduleName);

    template<typename T>
    static T Read(uintptr_t address);

    template<typename T>
    static bool Write(uintptr_t address, const T& value);

    static bool ReadRaw(uintptr_t address, void* buffer, size_t size);
    static bool WriteRaw(uintptr_t address, const void* buffer, size_t size);

    static std::string ReadString(uintptr_t address, size_t maxLength = 256);
    static std::wstring ReadWString(uintptr_t address, size_t maxLength = 256);

    static std::vector<int> PatternToIntVector(const char* Pattern);
    static inline void* FindPointer(std::vector<int> Pattern, uint8_t* Address, int Addition = 0);

    template<typename T>
    static T PatternScan(const char* Pattern, uint64_t Start, size_t Size, bool bFindPointer = false, int Addition = 0);

    template<typename T>
    static std::vector<T> PatternScanAll(const char* Pattern, uint64_t Start, size_t Size, bool bFindPointer = false, int Addition = 0);

    static int ScanVTable(std::string Pattern, void* Object, int SkipAmount = 0);

    template<typename T>
    static T PatternScanModule(const char* pattern, const std::string& moduleName, bool findPointer = false, int addition = 0);

    template<typename T>
    static T PatternScanMainModule(const char* pattern, bool findPointer = false, int addition = 0);

    template<typename T>
    static std::vector<T> PatternScanModuleAll(const char* pattern, const std::string& moduleName, bool findPointer = false, int addition = 0);


    static uintptr_t GetAbsoluteAddress(uintptr_t instructionPtr, int offset, int instructionSize);
    static uintptr_t FollowRelativeJump(uintptr_t address);
    static bool IsValidPointer(uintptr_t address);

private:
    static bool InternalRead(uintptr_t address, void* buffer, size_t size);
    static bool InternalWrite(uintptr_t address, const void* buffer, size_t size);
};
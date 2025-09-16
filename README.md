
# 🖥️ Pxcket's DirectX 11 ImGui Hook (Kiero-Based)

[![GitHub stars](https://img.shields.io/github/stars/Pxcket/ImGui-DirectX11-KieroHook?style=social)](https://github.com/Pxcket/ImGui-DirectX11-KieroHook/stargazers)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows-0078d7.svg)](https://en.wikipedia.org/wiki/Microsoft_Windows)

> A professional-grade DirectX 11 overlay with ImGui integration, built on Kiero. Universally compatible with any DirectX 11 application optimized for game modding, and runtime analysis.

---

## 📌 Table of Contents

<details>
<summary><b>🔽 Expand All Sections</b></summary>

- [🌟 Features](#-features)
- [🚀 Getting Started](#-getting-started)
- [🧰 Memory Utility - Game Reversing Toolkit](#-memory-utility--game-reversing-toolkit)
  - [Module Discovery](#module-discovery)
  - [Pattern Scanning](#pattern-scanning)
  - [Dynamic Address Resolution](#dynamic-address-resolution)
  - [VTable Scanning](#vtable-scanning)
  - [Pointer Validation & Safety](#pointer-validation--safety)
  - [Real-World Use Cases](#real-world-use-cases)
- [📊 Logging System](#-logging-system)
- [🎨 Overlay & Input](#-overlay--input)
- [⚙️ Core Architecture](#-core-architecture)
- [⚠️ Troubleshooting](#️-troubleshooting)
- [📜 License](#-license)

</details>

---

## 🌟 Features

### ✅ Core Functionality
- 🎮 DirectX 11 hooking via **Kiero**
- 🖌️ Full **ImGui** integration (runtime GUI)
- 🧵 Thread-safe initialization & shutdown
- 🛡️ Built-in mouse-fix overlay (toggle with `DEL` / `INS` / `F9`)
- 🔄 Runtime GUI toggle without restarting
- ⚡ Lightweight, minimal performance impact

---

## 🚀 Getting Started

### Prerequisites
- ✅ Windows 10 or 11 (x64)
- ✅ Visual Studio 2022 (Community or higher)
- ✅ DirectX SDK (June 2010) - *optional, headers are bundled*

### Installation
```bash
git clone --recursive https://github.com/Pxcket/ImGui-DirectX11-KieroHook.git
cd ImGui-DirectX11-KieroHook
```

1. Open `ImGui-DirectX11-KieroHook.sln` in **Visual Studio 2022**
2. Set Configuration -> **Release**
3. Set Platform -> **x64**
4. Build Solution (`Ctrl+Shift+B`)
5. Inject resulting `.dll` into target process

> 💡 Build failed? -> Open an [Issue](https://github.com/Pxcket/ImGui-DirectX11-KieroHook/issues). We respond fast.

---

## 🧰 Memory Utility - Game Reversing Toolkit

This is not just a memory reader - it’s a full **dynamic analysis engine** for reverse engineers. Forget static addresses. Forget Cheat Engine dependency. This toolkit lets you find, validate, and manipulate memory *programmatically* - even across game updates.

---

### 🔍 Module Discovery

Before you can scan or read anything, you need to know where modules are loaded.

```cpp
uintptr_t base = Memory::GetModuleBase("game.dll"); // Get base of game.dll
uintptr_t main = Memory::GetMainModuleBase();       // Get base of .exe
size_t size = Memory::GetModuleSize("engine.dll");  // Get size for scanning bounds
```

> ✅ Use this to calculate dynamic offsets: `healthAddr = base + 0x123456;`

---

### 🕵️ Pattern Scanning - Find Anything, Anywhere

Lost your function address after a game update? Scan for it.

#### ➤ Scan inside a module for a byte signature

```cpp
// Find: E8 ?? ?? ?? ?? 8B 45 FC (CALL + MOV EBP-4)
uintptr_t func = Memory::PatternScanModule<uintptr_t>(
    "E8 ? ? ? ? 8B 45 FC",
    "game.dll",
    true,  // bFindPointer - auto-resolve CALL destination
    0      // Addition - extra offset if needed
);
```

#### ➤ Scan main module (your .exe or injector)

```cpp
uintptr_t renderHook = Memory::PatternScanMainModule<uintptr_t>("55 8B EC 83 EC 20", false, 0);
```

#### ➤ Find ALL matches (for vtables, multiple hooks, etc)

```cpp
std::vector<uintptr_t> allCalls = Memory::PatternScanModuleAll<uintptr_t>(
    "FF 15 ? ? ? ?",  // CALL [address]
    "game.dll",
    true, 0
);

for (auto addr : allCalls) {
    // Log or hook each one
}
```

> 💡 Wildcards (`?`) ignore bytes. Use for offsets, addresses, or changing instructions.

---

### 🧭 Dynamic Address Resolution

Game uses `CALL [RIP + offset]`? Resolve it.

#### ➤ Follow relative jumps (E9/E8)

```cpp
uintptr_t maybeTrampoline = 0x12345000;
uintptr_t realFunction = Memory::FollowRelativeJump(maybeTrampoline);
// If it’s a JMP or CALL, returns destination. Otherwise, returns input.
```

#### ➤ Calculate absolute addresses from instruction-relative offsets

```cpp
// You found: MOV RAX, [RIP + 0x10] at address 0x5000
// Offset starts at +2, instruction is 7 bytes
uintptr_t dataPtr = Memory::GetAbsoluteAddress(0x5000, 2, 7);
// Returns 0x5000 + 7 + *(int*)(0x5002)
```

> ✅ Essential for resolving IAT, GOT, or data section references.

---

### 🎯 VTable Scanning - Find Virtual Functions Fast

Need to hook `Player::TakeDamage()` but don’t know the vtable index? Scan for it.

```cpp
void* pPlayer = /* get player object ptr */;
int vIndex = Memory::ScanVTable(
    "55 8B EC 83 E4 F8",  // Typical __thiscall prologue
    pPlayer,
    3                     // Skip first 3 funcs (dtor, etc)
);

if (vIndex > 0) {
    // Hook vtable[vIndex]  that’s your function
}
```

> ⚠️ Assumes vtable ends with nullptr. Works in most C++ games.

---

### 🛡️ Pointer Validation & Safety

Don’t crash the game. Validate before you read.

```cpp
uintptr_t ptr = playerBase + 0x30;
if (Memory::IsValidPointer(ptr)) {
    int health = Memory::Read<int>(ptr);
    Memory::Write<int>(ptr, 9999); // God mode
} else {
    // Log error - pointer invalid
}
```

Checks:
- Not null
- Not in kernel space
- Memory is readable/executable

> ✅ Thread-safe. Use everywhere.

---

### 💥 Real-World Use Cases

#### ➤ Auto-Health Hack (Dynamic Offset)

```cpp
uintptr_t gameBase = Memory::GetModuleBase("game.dll");
uintptr_t healthAddr = gameBase + 0x123456;

if (Memory::IsValidPointer(healthAddr)) {
    Memory::Write<int>(healthAddr, 9999);
}
```

#### ➤ Dynamic Function Hook

```cpp
uintptr_t takeDamage = Memory::PatternScanModule<uintptr_t>(
    "55 8B EC 83 EC 10 53",  // thiscall + stack setup
    "game.dll",
    false, 0
);

takeDamage = Memory::FollowRelativeJump(takeDamage); // Resolve trampoline

// Now DetourFunction(takeDamage, MyHook);
```

#### ➤ String Reader (Player Name, Weapon, etc)

```cpp
std::string playerName = Memory::ReadString(playerNamePtr, 32);
std::wstring weaponName = Memory::ReadWString(weaponNamePtr, 64);
```

---

## 📊 Logging System

Thread-safe, file + console logging. Toggle with `PPV` preprocessor flag.

```cpp
CMD::INIT_CMD();        // Rotates log.txt -> log_old.txt
CMD::log("Overlay initialized");
CMD::success("Hook installed");
CMD::error("Failed to find pattern");
CMD::debug("Pointer: 0x12345678");
```

> 🔧 Enable in Release: Project Properties -> C/C++ -> Preprocessor -> Add `PPV`

---

## 🎨 Overlay & Input

- ImGui overlay renders every `Present` call
- Input handled via `WndProc` hook
- Toggle GUI with `DEL`, `INS`, or `F9`
- Mouse confined to window when GUI open ( ghetto mouse fix :o )

---

## ⚙️ Core Architecture

- `DllMain` -> Spawns `MainThread`
- `Base::MainThread` -> Initializes `Core`
- `Core::Initialize` -> Sets up `HookManager`
- `HookManager` -> Binds `Present` via Kiero -> Initializes DirectX + Overlay
- `PresentHook` -> Renders ImGui every frame
- `WndProc` -> Toggles GUI + routes input

Thread-safe. Injection-safe. Battle-tested.

---

## ⚠️ Troubleshooting

- ❌ **Build fails?** -> Ensure `Release x64`
- ❌ **No overlay?** -> Verify target is DX11. Try `Present` index `8` or `137`
- ❌ **Crash on inject?** -> Enable logging (`PPV`), check `log.txt`
- ❓  **Need help?** -> Open an [Issue](https://github.com/Pxcket/ImGui-DirectX11-KieroHook/issues)

---

## 📜 License

MIT - Use it however you want. Sell it. Modify it. Break games with it. Just don’t sue me if you crash your PC :)

---

> 💬 **Tip:** Combine `Memory::PatternScanModule` + `Memory::FollowRelativeJump` + `Memory::IsValidPointer` - that’s 90% of modern game hacking right there.



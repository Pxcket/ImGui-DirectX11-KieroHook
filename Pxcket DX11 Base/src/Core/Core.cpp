#include "Core.h"
#include "HookManager.h"
#include "../UI/Overlay.h"
#include "../Utilities/Console.h"

namespace Core {
    static bool s_IsReady = false;

    bool Initialize() {
        if (!HookManager::Initialize()) {
            return false;
        }

#ifdef PPV
        Console::Load();
#endif 
        s_IsReady = true;
        return true;
    }

    void Shutdown() {
        HookManager::Shutdown();
        UI::Overlay::Shutdown();
        s_IsReady = false;
    }

    void Update() {
        if (s_IsReady) {
            
        }
    }

    bool IsReady() {
        return s_IsReady;
    }

    void SetReady(bool ready) {
        s_IsReady = ready;
    }
}
#pragma once

namespace Core {
    bool Initialize();
    void Shutdown();
    void Update();

    bool IsReady();
    void SetReady(bool ready);
}
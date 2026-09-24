#pragma once
#include <functional>
#include "../Unreal/AActor.hpp"

namespace RC::Unreal {
class UEngine;
}

namespace RC::Unreal::Hook {
    // Resolved at runtime via GetProcAddress – no dllimport needed.
    using AActorTickFn = std::function<void(AActor*, float)>;

    // UE4SS's engine-tick hook. Absent from the pinned v3.0.1 build this
    // project used to ship against — which is why the tick trigger was built
    // on AActor::Tick and later a WndProc subclass — but present in current
    // builds, verified against the deployed UE4SS.dll's export table.
    using EngineTickFn = std::function<void(UEngine*, float)>;
} // namespace RC::Unreal::Hook

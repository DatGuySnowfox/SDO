#pragma once
#include <functional>
#include "../Unreal/AActor.hpp"

namespace RC::Unreal::Hook {
    // Resolved at runtime via GetProcAddress – no dllimport needed.
    using AActorTickFn = std::function<void(AActor*, float)>;
} // namespace RC::Unreal::Hook

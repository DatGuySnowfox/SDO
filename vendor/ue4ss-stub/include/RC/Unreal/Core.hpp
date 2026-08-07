#pragma once
// Minimal RC::Unreal types derived from UE4SS.dll export table.
// Only contains what sd-online-bridge actually uses.

#include <cstdint>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

#define RC_API __declspec(dllimport)

#ifndef STR
#  define STR(x) L##x
#endif

namespace RC {

enum class LogLevel { Normal, Warning, Error, Verbose };

namespace Unreal {

// ── FVector / FRotator ────────────────────────────────────────────────────
// UE5 uses double precision.

struct FVector {
    double X = 0.0, Y = 0.0, Z = 0.0;
    FVector() = default;
    FVector(double x, double y, double z) : X(x), Y(y), Z(z) {}
};

struct FRotator {
    double Pitch = 0.0, Yaw = 0.0, Roll = 0.0;

    RC_API FRotator();
    RC_API FRotator(double pitch, double yaw, double roll);
    RC_API double GetYaw() const;
};

// ── FHitResult ────────────────────────────────────────────────────────────
// Opaque blob; 256 bytes is safe for all UE5 versions.
struct FHitResult {
    uint8_t _opaque[256]{};
    FHitResult() { std::memset(_opaque, 0, sizeof(_opaque)); }
};

// ── FName ─────────────────────────────────────────────────────────────────
struct FName {
    RC_API FName();
    RC_API FName(uint32_t comparisonIndex, uint32_t number);
    RC_API std::wstring ToString();
};

// ── Forward declarations ──────────────────────────────────────────────────
class UObject;
class UClass;
class UFunction;
class UWorld;
class AActor;
class UEngine;

} // namespace Unreal
} // namespace RC

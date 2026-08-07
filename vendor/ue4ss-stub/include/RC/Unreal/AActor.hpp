#pragma once
#include "UObject.hpp"

namespace RC::Unreal {

// Matches UE4SS.dll exports (demangled):
//   ?K2_GetActorLocation@AActor@Unreal@RC@@QEAA?AUFVector@23@XZ
//   ?K2_GetActorRotation@AActor@Unreal@RC@@QEAA?AUFRotator@23@XZ
//   ?K2_SetActorLocationAndRotation@AActor@...@@QEAA_NUFVector@23@UFRotator@23@_NAEAUFHitResult@23@2@Z
//   ?GetWorld@AActor@Unreal@RC@@QEAAPEAVUWorld@23@XZ
//   ?K2_DestroyActor@AActor@Unreal@RC@@QEAAXXZ
//   ?SetActorTickEnabled@AActor@Unreal@RC@@QEAAX_N@Z
//   ?SetActorHiddenInGame@AActor@Unreal@RC@@QEAAX_N@Z
//   ?SetActorEnableCollision@AActor@Unreal@RC@@QEAAX_N@Z
//   ?StaticClass@AActor@Unreal@RC@@SAPEAVUClass@23@XZ

class AActor : public UObject {
public:
    // Returns actor world location (by value; UE5 double FVector).
    RC_API FVector  K2_GetActorLocation();

    // Returns actor world rotation (Pitch/Yaw/Roll in degrees).
    RC_API FRotator K2_GetActorRotation();

    // Teleport the actor. Pass bSweep=false, bTeleport=true for instant move.
    // SweepHitResult is ignored when bSweep=false; pass a zero-init FHitResult.
    RC_API bool K2_SetActorLocationAndRotation(
        FVector     NewLocation,
        FRotator    NewRotation,
        bool        bSweep,
        FHitResult& SweepHitResult,
        bool        bTeleport);

    RC_API UWorld* GetWorld();
    RC_API void    K2_DestroyActor();
    RC_API void    SetActorTickEnabled(bool enabled);
    RC_API void    SetActorHiddenInGame(bool hidden);
    RC_API void    SetActorEnableCollision(bool enabled);

    RC_API static UClass* StaticClass();
};

} // namespace RC::Unreal

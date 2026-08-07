#pragma once
#include "AActor.hpp"

namespace RC::Unreal {

// Matches:
//   ?SpawnActor@UWorld@Unreal@RC@@QEAAPEAVAActor@23@PEAVUClass@23@PEBUFVector@23@PEBUFRotator@23@@Z

class UWorld : public UObject {
public:
    // Spawn an actor of the given class at position/rotation.
    // Returns nullptr on failure.
    RC_API AActor* SpawnActor(UClass*         Class,
                              const FVector*  Location,
                              const FRotator* Rotation);
};

} // namespace RC::Unreal

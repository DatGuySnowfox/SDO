#ifndef UE4SS_SDK_AIOptimizer_HPP
#define UE4SS_SDK_AIOptimizer_HPP

#include "AIOptimizer_enums.hpp"

struct FAIODebugSubjectData
{
    int32 Layer;                                                                      // 0x0000 (size: 0x4)
    FVector SubjectLocation;                                                          // 0x0008 (size: 0x18)
    FVector InvokerLocation;                                                          // 0x0020 (size: 0x18)
    uint8 bIsSpawned;                                                                 // 0x0038 (size: 0x1)
    uint8 bIsPending;                                                                 // 0x0038 (size: 0x1)
    uint8 bNotUpdated;                                                                // 0x0038 (size: 0x1)
    uint8 bIsSeen;                                                                    // 0x0038 (size: 0x1)
    float DistanceToInvoker;                                                          // 0x003C (size: 0x4)
    float DespawnRadius;                                                              // 0x0040 (size: 0x4)

}; // Size: 0x48

struct FAIODespawnedSubject
{
    FTransform Transform;                                                             // 0x0000 (size: 0x60)
    TSubclassOf<class AActor> Class;                                                  // 0x0060 (size: 0x8)
    float SpawnRadiusSquared;                                                         // 0x0068 (size: 0x4)
    uint8 Priority;                                                                   // 0x006C (size: 0x1)
    class UAIOData_Base* Data;                                                        // 0x0070 (size: 0x8)
    FAIOSubjectHandle Handle;                                                         // 0x0078 (size: 0x4)
    uint8 bIsForcedToSpawn;                                                           // 0x007C (size: 0x1)
    uint8 bCanBeRespawnedOnlyByHandle;                                                // 0x007C (size: 0x1)
    class AActor* Spawner;                                                            // 0x0080 (size: 0x8)

}; // Size: 0x90

struct FAIOInvoker
{
    class UAIOInvokerComponent* Component;                                            // 0x0000 (size: 0x8)
    class AActor* ComponentOwner;                                                     // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FAIOPendingRespawnGroup
{
    float SpawnGameTime;                                                              // 0x0000 (size: 0x4)
    FAIOPendingSpawnGroup SpawnGroup;                                                 // 0x0004 (size: 0x8)

}; // Size: 0xC

struct FAIOPendingSpawnGroup
{
    int32 SpawnedAmount;                                                              // 0x0000 (size: 0x4)
    int32 TotalAmountToSpawn;                                                         // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FAIOSpawnPoint
{
    FTransform Transform;                                                             // 0x0000 (size: 0x60)

}; // Size: 0x60

struct FAIOSubject
{
    class UAIOSubjectComponent* Component;                                            // 0x0000 (size: 0x8)
    class AActor* ComponentOwner;                                                     // 0x0008 (size: 0x8)
    int32 Priority;                                                                   // 0x0010 (size: 0x4)

}; // Size: 0x18

struct FAIOSubjectHandle
{
    int32 HandleId;                                                                   // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FAIOptimizationLayer
{
    float LayerRadius;                                                                // 0x0000 (size: 0x4)

}; // Size: 0x8

class ISpawnerInterface : public IInterface
{

    void OnSubjectSpawnedByOptimizerSubsystem(class UAIOSubjectComponent* SpawnedSubjectComponent);
    void OnSubjectDespawnedByOptimizerSubsystem(class UAIOSubjectComponent* DespawnedSubjectComponent);
}; // Size: 0x28

class UAIOBPLibrary : public UBlueprintFunctionLibrary
{

    void SetCharacterMovementEnabled(class ACharacter* Character, bool bEnable);
    void SetAILogicEnabled(class AActor* Actor, bool bEnable);
    bool RemoveHandle(TArray<FAIOSubjectHandle>& Array, const FAIOSubjectHandle& Handle);
    bool IsHandleValid(const FAIOSubjectHandle& Handle);
    FName GetSubjectTag();
    FString GetString(const FAIOSubjectHandle& Handle);
    FName GetInvokerTag();
    int32 FindHandle(const TArray<FAIOSubjectHandle>& Array, const FAIOSubjectHandle& HandleToFind);
    void DebugAIOptimizer(class UWorld* World, bool bDebug);
    int32 AddUniqueHandle(TArray<FAIOSubjectHandle>& Array, const FAIOSubjectHandle& Handle);
}; // Size: 0x28

class UAIOData_Base : public UObject
{
}; // Size: 0x28

class UAIODeveloperSettings : public UDeveloperSettings
{
    uint8 bIsSubsystemEnabled;                                                        // 0x0038 (size: 0x1)
    uint8 bDisplayDebugInfo;                                                          // 0x0038 (size: 0x1)
    TSubclassOf<class UUserWidget> DebugWidgetClass;                                  // 0x0040 (size: 0x8)
    float OptimizationUpdateInterval;                                                 // 0x0048 (size: 0x4)
    EDespawnMethod HandleSpawnDespawnMethod;                                          // 0x004C (size: 0x1)
    int32 SpawnCapacityPerUpdate;                                                     // 0x0050 (size: 0x4)
    float SpawnInterval;                                                              // 0x0054 (size: 0x4)
    float DespawnRadius;                                                              // 0x0058 (size: 0x4)
    float PeripheralVisionHalfAngleDegrees;                                           // 0x005C (size: 0x4)

}; // Size: 0x60

class UAIOInvokerComponent : public UActorComponent
{
    class UUserWidget* DebugWidget;                                                   // 0x00A0 (size: 0x8)

    void DebugAIOptimizer(bool bDebug);
}; // Size: 0xA8

class UAIOSubjectComponent : public UActorComponent
{
    FAIOSubjectHandle Handle;                                                         // 0x00C8 (size: 0x4)
    FAIOSubjectComponentOnOptimizationUpdate OnOptimizationUpdate;                    // 0x00D0 (size: 0x10)
    void OnOptimizationUpdate(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    FAIOSubjectComponentOnPreDespawn OnPreDespawn;                                    // 0x00E0 (size: 0x10)
    void OnPreDespawn(class UAIOData_Base* Data);
    FAIOSubjectComponentOnPostSpawned OnPostSpawned;                                  // 0x00F0 (size: 0x10)
    void OnPostSpawned(class UAIOData_Base* Data);
    class AActor* Spawner;                                                            // 0x0100 (size: 0x8)
    uint8 bCanBeUpdatedBySubsystem;                                                   // 0x0108 (size: 0x1)
    uint8 bAllowSubsystemToAutoDespawn;                                               // 0x0108 (size: 0x1)
    float OverrideSubsystemDespawnRadius;                                             // 0x010C (size: 0x4)
    uint8 Priority;                                                                   // 0x0110 (size: 0x1)
    TSubclassOf<class UAIOData_Base> DataClass;                                       // 0x0118 (size: 0x8)
    TArray<FAIOptimizationLayer> OptimizationLayers;                                  // 0x0120 (size: 0x10)
    uint8 bShouldCalculateIsSeen;                                                     // 0x0130 (size: 0x1)

    void UnregisterSubject();
    bool ShouldBeDespawned(class UAIOptimizerSubsystem* Subsystem, bool bForceUpdateDataToInvokers);
    void SetSpawner(TScriptInterface<class ISpawnerInterface> NewSpawner);
    void SetCharacterFeatures(class ACharacter* Character, int32 FeaturesToEnable);
    void SetCanBeUpdatedBySubsystem(bool bCanBeUpdated);
    void ReinitializeOptimizationLayers(TArray<FAIOptimizationLayer>& NewOptimizationLayers);
    void RegisterSubject();
    float IsSeenByAnyInvoker();
    bool IsDespawning();
    float GetSpawnRadiusSquared(class UAIOptimizerSubsystem* Subsystem);
    int32 GetOptimizationLayerForCurrentDistance();
    float GetDistanceToClosestInvoker();
    float GetDespawnRadiusSquared(class UAIOptimizerSubsystem* Subsystem);
    int32 GetCurrentOptimizationLayer();
    FVector GetClosestInvokerLocation();
    bool CanBeUpdatedBySubsystem();
}; // Size: 0x140

class UAIOptimizerSubsystem : public UWorldSubsystem
{
    FAIOptimizerSubsystemOnSubjectDespawned OnSubjectDespawned;                       // 0x0030 (size: 0x10)
    void OnSubjectDespawned(class UAIOSubjectComponent* SubjectComponent);
    FAIOptimizerSubsystemOnSubjectSpawned OnSubjectSpawned;                           // 0x0040 (size: 0x10)
    void OnSubjectSpawned(class UAIOSubjectComponent* SubjectComponent);
    FAIOptimizerSubsystemOnSubsystemEnabledChanged OnSubsystemEnabledChanged;         // 0x0050 (size: 0x10)
    void OnSubsystemEnabledChanged(bool bIsEnabled);
    TArray<FAIOSubject> SpawnedSubjects;                                              // 0x0060 (size: 0x10)
    TArray<FAIODespawnedSubject> DespawnedSubjects;                                   // 0x0070 (size: 0x10)
    TArray<FAIOInvoker> Invokers;                                                     // 0x0080 (size: 0x10)
    TArray<FAIOSubject> PendingDespawnSubjectsHeap;                                   // 0x0090 (size: 0x10)
    TArray<FAIODespawnedSubject> PendingSpawnSubjectsHeap;                            // 0x00A0 (size: 0x10)
    class UUserWidget* DebugWidgetObj;                                                // 0x00C8 (size: 0x8)

    bool UnregisterSubject(class UAIOSubjectComponent* SubjectComponent);
    bool UnregisterInvoker(class UAIOInvokerComponent* InvokerComponent);
    void ShrinkArrays();
    void SetIsSystemEnabled(bool bIsEnabled);
    bool RemoveDespawnedSubjectByHandle(const FAIOSubjectHandle& Handle);
    bool RegisterSubject(class UAIOSubjectComponent* SubjectComponent);
    bool RegisterInvoker(class UAIOInvokerComponent* InvokerComponent);
    void LoopSubjects();
    void LoopPendingSubjects();
    bool K2_SpawnSubjectByHandle(EDespawnMethod Method, const FAIOSubjectHandle& SubjectHandle);
    bool K2_DespawnSubjectByHandle(FAIOSubjectHandle& SubjectHandle, EDespawnMethod Method, float OverrideSpawnRadius, bool bAllowRespawnOnlyByHandle);
    bool K2_DespawnSubject(FAIOSubjectHandle& SubjectHandle, EDespawnMethod Method, class UAIOSubjectComponent* Component, float OverrideSpawnRadius, bool bAllowRespawnOnlyByHandle);
    int32 GetSubjectIndex(class UAIOSubjectComponent* Component);
    int32 GetInvokerIndex(class UAIOInvokerComponent* Component);
    float GetDistanceToClosestInvokerSquared(const FVector& QuerierLocation);
    TArray<FAIODebugSubjectData> GetDebugSubjects();
    FVector GetClosestInvokerLocation(const FVector& QuerierLocation);
    TMap<EAIODebugGroup, int32> GetCategorizedDebugSubjects(const TArray<FAIODebugSubjectData>& DebugSubjects);
    void DebugAIOptimizer(bool bDebug);
}; // Size: 0xD0

#endif

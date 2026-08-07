#ifndef UE4SS_SDK_Event_SpawnAIHumanSpawner_HPP
#define UE4SS_SDK_Event_SpawnAIHumanSpawner_HPP

class UEvent_SpawnAIHumanSpawner_C : public UNarrativeEvent
{
    FTransform Spawn Transform;                                                       // 0x0030 (size: 0x60)
    TArray<FS_AISpawner> Spawning AI;                                                 // 0x0090 (size: 0x10)
    FName Tag;                                                                        // 0x00A0 (size: 0x8)
    FString Argument for Quest;                                                       // 0x00A8 (size: 0x10)
    double Spawning Interval;                                                         // 0x00B8 (size: 0x8)
    bool Override Starting Behaviour;                                                 // 0x00C0 (size: 0x1)
    TEnumAsByte<E_StartingAIBehaviours::Type> Override Behaviour;                     // 0x00C1 (size: 0x1)
    TArray<class ABP_MasterWayPoint_C*> Way Points;                                   // 0x00C8 (size: 0x10)
    bool Skip Meshes?;                                                                // 0x00D8 (size: 0x1)
    TArray<class AActor*> Meshes to Spawn On;                                         // 0x00E0 (size: 0x10)
    double Box Extent X;                                                              // 0x00F0 (size: 0x8)
    double Box Extent Y;                                                              // 0x00F8 (size: 0x8)
    double Box Extent Z;                                                              // 0x0100 (size: 0x8)

    void ExecuteEvent(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x108

#endif

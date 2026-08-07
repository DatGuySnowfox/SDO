#ifndef UE4SS_SDK_BP_KeycardDoor_HPP
#define UE4SS_SDK_BP_KeycardDoor_HPP

class ABP_KeycardDoor_C : public ABP_MasterDoor_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* Scanner1;                                             // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Scanner;                                              // 0x02B0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B8 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* Door;                                                 // 0x02C8 (size: 0x8)
    class UStaticMeshComponent* DoorFrame;                                            // 0x02D0 (size: 0x8)
    float Timeline_1_Alpha_020BC8084747E4088ED566892304DB21;                          // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_1__Direction_020BC8084747E4088ED566892304DB21; // 0x02DC (size: 0x1)
    class UTimelineComponent* Timeline_1;                                             // 0x02E0 (size: 0x8)
    float Timeline_CloseDoor_Alpha_319EF3CC4BBC24B091BCCEAF8446D51F;                  // 0x02E8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_CloseDoor__Direction_319EF3CC4BBC24B091BCCEAF8446D51F; // 0x02EC (size: 0x1)
    class UTimelineComponent* Timeline_CloseDoor;                                     // 0x02F0 (size: 0x8)
    float Timeline_OpenDoor_Alpha_C0E23A7340A8D534984C27B69F1920A4;                   // 0x02F8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_OpenDoor__Direction_C0E23A7340A8D534984C27B69F1920A4; // 0x02FC (size: 0x1)
    class UTimelineComponent* Timeline_OpenDoor;                                      // 0x0300 (size: 0x8)
    class UJigsawItem_DataAsset_C* Keycard;                                           // 0x0308 (size: 0x8)
    bool Open?;                                                                       // 0x0310 (size: 0x1)
    bool Outline?;                                                                    // 0x0311 (size: 0x1)
    bool XPGiven?;                                                                    // 0x0312 (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void Timeline_1__FinishedFunc();
    void Timeline_1__UpdateFunc();
    void Timeline_CloseDoor__FinishedFunc();
    void Timeline_CloseDoor__UpdateFunc();
    void Timeline_OpenDoor__FinishedFunc();
    void Timeline_OpenDoor__UpdateFunc();
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void MC_OpenDoor(class AActor* Actor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void Event_OpenDoorBehind();
    void Event_CloseDoor(class AActor* Actor);
    void Event_OpenDoor(class AActor* Actor);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_KeycardDoor(int32 EntryPoint);
}; // Size: 0x313

#endif

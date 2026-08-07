#ifndef UE4SS_SDK_BP_KeypadDoor_HPP
#define UE4SS_SDK_BP_KeypadDoor_HPP

class ABP_KeypadDoor_C : public ABP_MasterDoor_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* Keypad;                                               // 0x02A8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B0 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Door;                                                 // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* DoorFrame;                                            // 0x02C8 (size: 0x8)
    float Timeline_1_Alpha_24B1A35C4ABB2793F93ECF91F5D538BF;                          // 0x02D0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_1__Direction_24B1A35C4ABB2793F93ECF91F5D538BF; // 0x02D4 (size: 0x1)
    class UTimelineComponent* Timeline_1;                                             // 0x02D8 (size: 0x8)
    float Timeline_CloseDoor_Alpha_CA3F5BFE4C8C6A0CDF98E6B85C2F1F32;                  // 0x02E0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_CloseDoor__Direction_CA3F5BFE4C8C6A0CDF98E6B85C2F1F32; // 0x02E4 (size: 0x1)
    class UTimelineComponent* Timeline_CloseDoor;                                     // 0x02E8 (size: 0x8)
    float Timeline_OpenDoor_Alpha_4C2311F541FA940284509BA379658CCF;                   // 0x02F0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_OpenDoor__Direction_4C2311F541FA940284509BA379658CCF; // 0x02F4 (size: 0x1)
    class UTimelineComponent* Timeline_OpenDoor;                                      // 0x02F8 (size: 0x8)
    FText DoorName;                                                                   // 0x0300 (size: 0x18)
    class ABP_KeypadNote_C* NoteRef;                                                  // 0x0318 (size: 0x8)
    bool DoorFrame?;                                                                  // 0x0320 (size: 0x1)
    int32 Code;                                                                       // 0x0324 (size: 0x4)
    bool Open?;                                                                       // 0x0328 (size: 0x1)
    bool XPGiven?;                                                                    // 0x0329 (size: 0x1)
    class AActor* CurrentActor;                                                       // 0x0330 (size: 0x8)
    bool OpenedInThisInstance?;                                                       // 0x0338 (size: 0x1)

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
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void MC_PlaySound(class USoundBase* Sound);
    void IncorrectCode();
    void CorrectCode();
    void MC_OpenDoor(class AActor* Actor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void Event_OpenDoorBehind();
    void Event_CloseDoor(class AActor* Actor);
    void Event_OpenDoor(class AActor* Actor);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_KeypadDoor(int32 EntryPoint);
}; // Size: 0x339

#endif

#ifndef UE4SS_SDK_BP_LockedDoor_HPP
#define UE4SS_SDK_BP_LockedDoor_HPP

class ABP_LockedDoor_C : public ABP_LockPickObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02C8 (size: 0x8)
    class UStaticMeshComponent* Door;                                                 // 0x02D0 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02D8 (size: 0x8)
    float Timeline_OpenDoorAuto_Alpha_7388A7EA4CB01198880C048DDE54E33F;               // 0x02E0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_OpenDoorAuto__Direction_7388A7EA4CB01198880C048DDE54E33F; // 0x02E4 (size: 0x1)
    class UTimelineComponent* Timeline_OpenDoorAuto;                                  // 0x02E8 (size: 0x8)
    float Timeline_OpenDoor_Alpha_CA4B83EF45872B61E7779EAD32D6380C;                   // 0x02F0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_OpenDoor__Direction_CA4B83EF45872B61E7779EAD32D6380C; // 0x02F4 (size: 0x1)
    class UTimelineComponent* Timeline_OpenDoor;                                      // 0x02F8 (size: 0x8)
    bool IsHeld;                                                                      // 0x0300 (size: 0x1)
    class UStaticMesh* Mesh;                                                          // 0x0308 (size: 0x8)
    double Duration;                                                                  // 0x0310 (size: 0x8)
    class USoundBase* NoiseAudio;                                                     // 0x0318 (size: 0x8)
    bool Closed?;                                                                     // 0x0320 (size: 0x1)
    FText Action Name;                                                                // 0x0328 (size: 0x18)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void AdvancedLockCheck(bool& Check);
    void UserConstructionScript();
    void Timeline_OpenDoorAuto__FinishedFunc();
    void Timeline_OpenDoorAuto__UpdateFunc();
    void Timeline_OpenDoor__FinishedFunc();
    void Timeline_OpenDoor__UpdateFunc();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void LockpickSuccessful();
    void SetInteractOption(FGameplayTag Option);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void EventDoorBehind();
    void Event_Door();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_LockedDoor(int32 EntryPoint);
}; // Size: 0x340

#endif

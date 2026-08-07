#ifndef UE4SS_SDK_BP_LaboratorySlidingDoor_HPP
#define UE4SS_SDK_BP_LaboratorySlidingDoor_HPP

class ABP_LaboratorySlidingDoor_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UArrowComponent* Debug_Front;                                               // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Frame;                                                // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Door;                                                 // 0x02C0 (size: 0x8)
    float TL_DoorsAuto_Alpha_F06518C54DEEEC3499FB10AFDCD4E288;                        // 0x02C8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> TL_DoorsAuto__Direction_F06518C54DEEEC3499FB10AFDCD4E288; // 0x02CC (size: 0x1)
    class UTimelineComponent* TL_DoorsAuto;                                           // 0x02D0 (size: 0x8)
    float TL_DoorsKeycard_Alpha_1807B7464BB9E4B5B4A4719F4AF08F9D;                     // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> TL_DoorsKeycard__Direction_1807B7464BB9E4B5B4A4719F4AF08F9D; // 0x02DC (size: 0x1)
    class UTimelineComponent* TL_DoorsKeycard;                                        // 0x02E0 (size: 0x8)
    bool IsClosed;                                                                    // 0x02E8 (size: 0x1)
    class UJigsawItem_DataAsset_C* Keycard;                                           // 0x02F0 (size: 0x8)
    bool Unlocked;                                                                    // 0x02F8 (size: 0x1)
    bool Locked;                                                                      // 0x02F9 (size: 0x1)
    bool Warning;                                                                     // 0x02FA (size: 0x1)
    bool XPGiven?;                                                                    // 0x02FB (size: 0x1)
    bool CanOpenDoorFromBehind?;                                                      // 0x02FC (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRep_Warning();
    void OnRep_Locked();
    void OnRep_Unlocked();
    void UserConstructionScript();
    void TL_DoorsAuto__FinishedFunc();
    void TL_DoorsAuto__UpdateFunc();
    void TL_DoorsKeycard__FinishedFunc();
    void TL_DoorsKeycard__UpdateFunc();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void SetPower_TurnedOn(bool TurnOn);
    void SetPower_Warning(bool SetWarning);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void Event_AutoDoor();
    void OnExecuteInteractEnded();
    void Event_Door(class AActor* Interactor);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void CallInteractOnObject();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_LaboratorySlidingDoor(int32 EntryPoint);
}; // Size: 0x2FD

#endif

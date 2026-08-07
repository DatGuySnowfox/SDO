#ifndef UE4SS_SDK_BP_Lever_HPP
#define UE4SS_SDK_BP_Lever_HPP

class ABP_Lever_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Lever;                                                // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* base;                                                 // 0x02B8 (size: 0x8)
    float LeverMovement_Lever_09701F514E1A38F04DC8CAA9C9C1EC14;                       // 0x02C0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> LeverMovement__Direction_09701F514E1A38F04DC8CAA9C9C1EC14; // 0x02C4 (size: 0x1)
    class UTimelineComponent* LeverMovement;                                          // 0x02C8 (size: 0x8)
    TArray<class AActor*> Actors;                                                     // 0x02D0 (size: 0x10)
    bool PoweredOn;                                                                   // 0x02E0 (size: 0x1)
    bool LeverToggled?;                                                               // 0x02E1 (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void LeverMovement__FinishedFunc();
    void LeverMovement__UpdateFunc();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void SetPower_Warning(bool SetWarning);
    void CallInteractOnObject();
    void SetPower_TurnedOn(bool TurnOn);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void BndEvt__BP_MPLootContainer_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_Lever(int32 EntryPoint);
}; // Size: 0x2E2

#endif

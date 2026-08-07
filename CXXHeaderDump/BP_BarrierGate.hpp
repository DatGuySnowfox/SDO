#ifndef UE4SS_SDK_BP_BarrierGate_HPP
#define UE4SS_SDK_BP_BarrierGate_HPP

class ABP_BarrierGate_C : public ABP_MasterDoor_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Door2;                                                // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Door1;                                                // 0x02B8 (size: 0x8)
    float Timeline_0_Alpha_46729FC84A35509CAD5069979017673E;                          // 0x02C0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_0__Direction_46729FC84A35509CAD5069979017673E; // 0x02C4 (size: 0x1)
    class UTimelineComponent* Timeline_0;                                             // 0x02C8 (size: 0x8)
    bool Open?;                                                                       // 0x02D0 (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void Timeline_0__FinishedFunc();
    void Timeline_0__UpdateFunc();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void MC_OpenDoor(class AActor* Actor);
    void Event_Door();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_BarrierGate(int32 EntryPoint);
}; // Size: 0x2D1

#endif

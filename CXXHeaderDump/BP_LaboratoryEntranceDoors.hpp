#ifndef UE4SS_SDK_BP_LaboratoryEntranceDoors_HPP
#define UE4SS_SDK_BP_LaboratoryEntranceDoors_HPP

class ABP_LaboratoryEntranceDoors_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* DoorR;                                                // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* DoorL;                                                // 0x02B8 (size: 0x8)
    float Timeline_Alpha_A1EC8B354F84ABC22BA054BB4D120F76;                            // 0x02C0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline__Direction_A1EC8B354F84ABC22BA054BB4D120F76; // 0x02C4 (size: 0x1)
    class UTimelineComponent* Timeline;                                               // 0x02C8 (size: 0x8)
    bool IsClosed;                                                                    // 0x02D0 (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void Timeline__FinishedFunc();
    void Timeline__UpdateFunc();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void Event_Door();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_LaboratoryEntranceDoors(int32 EntryPoint);
}; // Size: 0x2D1

#endif

#ifndef UE4SS_SDK_BP_QuestGiver_HPP
#define UE4SS_SDK_BP_QuestGiver_HPP

class ABP_QuestGiver_C : public ABP_MasterQuestObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class USkeletalMeshComponent* Gun;                                                // 0x02C8 (size: 0x8)
    class UStaticMeshComponent* Mouth;                                                // 0x02D0 (size: 0x8)
    class UStaticMeshComponent* Accessory1;                                           // 0x02D8 (size: 0x8)
    class UStaticMeshComponent* Accessory;                                            // 0x02E0 (size: 0x8)
    class UStaticMeshComponent* Hat;                                                  // 0x02E8 (size: 0x8)
    class UStaticMeshComponent* Beard;                                                // 0x02F0 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x02F8 (size: 0x8)
    TSubclassOf<class UDialogue> Dialogue;                                            // 0x0300 (size: 0x8)
    FText Name;                                                                       // 0x0308 (size: 0x18)
    FString InteractionArgument;                                                      // 0x0320 (size: 0x10)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void BndEvt__BP_MPVendor_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_QuestGiver(int32 EntryPoint);
}; // Size: 0x330

#endif

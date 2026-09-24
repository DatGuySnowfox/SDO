#ifndef UE4SS_SDK_BP_AnimalDog_HPP
#define UE4SS_SDK_BP_AnimalDog_HPP

class ABP_AnimalDog_C : public ABP_Animal_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0670 (size: 0x8)
    class USceneComponent* PlayerPosition;                                            // 0x0678 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x0680 (size: 0x8)
    TMap<USkeletalMesh*, UMaterial*> Model;                                           // 0x0688 (size: 0x50)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<FGameplayTag, FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<FString, FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void SetMeshAndMaterial();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void BndEvt__BP_Zombie_Master_AIOSubject_K2Node_ComponentBoundEvent_2_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void OnExecuteInteractEnded();
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<FGameplayTag, FText> Options);
    void OnEndInteract();
    void ExecuteUbergraph_BP_AnimalDog(int32 EntryPoint);
}; // Size: 0x6D8

#endif

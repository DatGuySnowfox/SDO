#ifndef UE4SS_SDK_BP_RandomCivilian_HPP
#define UE4SS_SDK_BP_RandomCivilian_HPP

class ABP_RandomCivilian_C : public ABP_MainFriendly_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0690 (size: 0x8)
    class UWidgetComponent* NameWidget;                                               // 0x0698 (size: 0x8)
    class UStaticMeshComponent* BackpackMesh;                                         // 0x06A0 (size: 0x8)
    class UStaticMeshComponent* BeardMesh;                                            // 0x06A8 (size: 0x8)
    class UStaticMeshComponent* HairMesh;                                             // 0x06B0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x06B8 (size: 0x8)
    class UNavigationInvokerComponent* NavigationInvoker;                             // 0x06C0 (size: 0x8)
    bool IsMale?;                                                                     // 0x06C8 (size: 0x1)
    TArray<class USkeletalMesh*> Meshes;                                              // 0x06D0 (size: 0x10)
    TArray<class UStaticMesh*> HairMeshes;                                            // 0x06E0 (size: 0x10)
    TArray<class UStaticMesh*> BeardMeshes;                                           // 0x06F0 (size: 0x10)
    TMap<class UStaticMesh*, class FTransform> BackpackMeshes;                        // 0x0700 (size: 0x50)
    double Health;                                                                    // 0x0750 (size: 0x8)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void SetName();
    void SetMeshAndMaterial();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void BndEvt__BP_Zombie_Master_AIOSubject_K2Node_ComponentBoundEvent_2_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void ExecuteUbergraph_BP_RandomCivilian(int32 EntryPoint);
}; // Size: 0x758

#endif

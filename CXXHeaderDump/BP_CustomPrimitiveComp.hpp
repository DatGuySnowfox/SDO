#ifndef UE4SS_SDK_BP_CustomPrimitiveComp_HPP
#define UE4SS_SDK_BP_CustomPrimitiveComp_HPP

class ABP_CustomPrimitiveComp_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UBP_JigPickupComponent_C* BP_JigPickupComponent;                            // 0x02A0 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A8 (size: 0x8)
    class USceneComponent* AttachmentComp;                                            // 0x02B0 (size: 0x8)
    class UJigsawItem_DataAsset_C* DA;                                                // 0x02B8 (size: 0x8)
    bool Use Leader Pose?;                                                            // 0x02C0 (size: 0x1)
    FGuid UID;                                                                        // 0x02C4 (size: 0x10)

    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetMeshFromOwner(class USkeletalMeshComponent*& Mesh);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnEndInteract();
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void ReceiveBeginPlay();
    void Init(class UJigsawItem_DataAsset_C* DA, class USkeletalMeshComponent* OwnerMesh, bool UseLeaderPose?);
    void PickupBuildFromGround();
    void ExecuteUbergraph_BP_CustomPrimitiveComp(int32 EntryPoint);
}; // Size: 0x2D4

#endif

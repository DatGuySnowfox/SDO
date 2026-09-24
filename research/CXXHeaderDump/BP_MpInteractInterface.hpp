#ifndef UE4SS_SDK_BP_MpInteractInterface_HPP
#define UE4SS_SDK_BP_MpInteractInterface_HPP

class IBP_MpInteractInterface_C : public IInterface
{

    void PickupBuildFromGround();
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<FGameplayTag, FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void SetInteractOption(FGameplayTag Option);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnExecuteInteractEnded();
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void JigCanInteract(bool& Result);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<FString, FString>& CustomData);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<FGameplayTag, FText> Options);
}; // Size: 0x28

#endif

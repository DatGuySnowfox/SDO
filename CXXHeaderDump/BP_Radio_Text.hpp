#ifndef UE4SS_SDK_BP_Radio_Text_HPP
#define UE4SS_SDK_BP_Radio_Text_HPP

class ABP_Radio_Text_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02A8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B0 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x02B8 (size: 0x8)
    FName RadioID;                                                                    // 0x02C0 (size: 0x8)
    FStruct_Radio RadioStruct;                                                        // 0x02C8 (size: 0x20)
    int32 Row;                                                                        // 0x02E8 (size: 0x4)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void Event_ScrollText();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void ExecuteUbergraph_BP_Radio_Text(int32 EntryPoint);
}; // Size: 0x2EC

#endif

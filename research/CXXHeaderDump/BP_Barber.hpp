#ifndef UE4SS_SDK_BP_Barber_HPP
#define UE4SS_SDK_BP_Barber_HPP

class ABP_Barber_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UBillboardComponent* DebugCamera;                                           // 0x02A8 (size: 0x8)
    class UBillboardComponent* Debug;                                                 // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Glasses;                                              // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Hair;                                                 // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* Object;                                               // 0x02C8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02D0 (size: 0x8)
    class UCameraComponent* Camera;                                                   // 0x02D8 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x02E0 (size: 0x8)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void SetPlayerGearVisibility(bool Hidden?, class ABP_PlayerCharacter_C* Player);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ReceiveBeginPlay();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void Event_Exit();
    void ExecuteUbergraph_BP_Barber(int32 EntryPoint);
}; // Size: 0x2E8

#endif

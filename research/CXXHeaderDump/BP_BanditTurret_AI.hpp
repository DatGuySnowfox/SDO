#ifndef UE4SS_SDK_BP_BanditTurret_AI_HPP
#define UE4SS_SDK_BP_BanditTurret_AI_HPP

class ABP_BanditTurret_AI_C : public ABP_MasterTurret_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0318 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x0320 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x0328 (size: 0x8)
    float TurretRotateAnimation_Rotate_Alpha_066B7CEC4A8B4D4EBE3614908C6B0696;        // 0x0330 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> TurretRotateAnimation__Direction_066B7CEC4A8B4D4EBE3614908C6B0696; // 0x0334 (size: 0x1)
    class UTimelineComponent* TurretRotateAnimation;                                  // 0x0338 (size: 0x8)
    double Duration;                                                                  // 0x0340 (size: 0x8)
    bool IsHeld;                                                                      // 0x0348 (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void CheckBehindTurret(class AActor* Interactor);
    void IsObjectDamageable?(bool& Damageable?);
    void TurretRotateAnimation__FinishedFunc();
    void TurretRotateAnimation__UpdateFunc();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void Turret Destroyed Effect();
    void Turret Idle Start();
    void Turret Idle Stop();
    void Multicast Turret Idle Stop();
    void Multicast Turret Idle Start();
    void Multicast Spawn Emitter();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void Event_UpdateTimer();
    void Event_StopTimer();
    void Event_HoldTimer();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void ExecuteUbergraph_BP_BanditTurret_AI(int32 EntryPoint);
}; // Size: 0x349

#endif

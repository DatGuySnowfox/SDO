#ifndef UE4SS_SDK_BP_GuaranteedSpawner_HPP
#define UE4SS_SDK_BP_GuaranteedSpawner_HPP

class ABP_GuaranteedSpawner_C : public ABP_LaptopEventMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    class UWidgetComponent* magnifier;                                                // 0x02D8 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02E0 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02E8 (size: 0x8)
    class UStaticMeshComponent* SM_Lid;                                               // 0x02F0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02F8 (size: 0x8)
    float Timeline_Rot_029EDFD8418A46832A52B8B6B3D0ECE8;                              // 0x0300 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline__Direction_029EDFD8418A46832A52B8B6B3D0ECE8; // 0x0304 (size: 0x1)
    class UTimelineComponent* Timeline;                                               // 0x0308 (size: 0x8)
    TArray<class TSubclassOf<AActor>> Items;                                          // 0x0310 (size: 0x10)
    FText ContainerName;                                                              // 0x0320 (size: 0x18)
    FBP_GuaranteedSpawner_CSetCustomDepth_True SetCustomDepth_True;                   // 0x0338 (size: 0x10)
    void SetCustomDepth_True();
    FBP_GuaranteedSpawner_CSetCustomDepth_False SetCustomDepth_False;                 // 0x0348 (size: 0x10)
    void SetCustomDepth_False();
    FTimerHandle OutlineTimer;                                                        // 0x0358 (size: 0x8)
    bool PlayerClose?;                                                                // 0x0360 (size: 0x1)
    bool Opened;                                                                      // 0x0361 (size: 0x1)
    bool IsHeld;                                                                      // 0x0362 (size: 0x1)
    double ChanceToProduceNoise;                                                      // 0x0368 (size: 0x8)
    double DefaultChanceToProduceNoise;                                               // 0x0370 (size: 0x8)
    double DurationToHold;                                                            // 0x0378 (size: 0x8)
    FTimerHandle HoldToInteractTimer;                                                 // 0x0380 (size: 0x8)
    class UAudioComponent* SearchSoundComp;                                           // 0x0388 (size: 0x8)
    class AActor* CurrentActor;                                                       // 0x0390 (size: 0x8)

    void GetSettings(double& ChanceToProduceNoise, double& Default_ChanceToProduceNoise, double& DurationToHold);
    void IsHeld?(bool& Held?);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void JigCanInteract(bool& Result);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void SearchSound(bool Spawn?);
    void HoldToInteract();
    void NoiseEvent();
    void DistanceOutline();
    void Timeline__FinishedFunc();
    void Timeline__UpdateFunc();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void Event_Outline();
    void GetQuestArgument(FString Argument);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void Event_HoldToInteract();
    void OpenContainer();
    void PauseInteract();
    void MC_CrateOpen();
    void Svr_CrateOpen();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void BndEvt__BP_GuaranteedSpawner_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_GuaranteedSpawner(int32 EntryPoint);
    void SetCustomDepth_False__DelegateSignature();
    void SetCustomDepth_True__DelegateSignature();
}; // Size: 0x398

#endif

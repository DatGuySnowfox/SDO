#ifndef UE4SS_SDK_BP_LaboratoryCircuitBox_HPP
#define UE4SS_SDK_BP_LaboratoryCircuitBox_HPP

class ABP_LaboratoryCircuitBox_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UParticleSystemComponent* ParticleSystem;                                   // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Lever2;                                               // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Lever1;                                               // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* Door;                                                 // 0x02C8 (size: 0x8)
    class UStaticMeshComponent* Box;                                                  // 0x02D0 (size: 0x8)
    float TL_DoorReverse_Alpha_32903F654EFB3E23861D8FA9C3DB73BA;                      // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> TL_DoorReverse__Direction_32903F654EFB3E23861D8FA9C3DB73BA; // 0x02DC (size: 0x1)
    class UTimelineComponent* TL_DoorReverse;                                         // 0x02E0 (size: 0x8)
    float TL_Levers_Alpha_A96446E54BFE434078A866AB8C3CFBA0;                           // 0x02E8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> TL_Levers__Direction_A96446E54BFE434078A866AB8C3CFBA0; // 0x02EC (size: 0x1)
    class UTimelineComponent* TL_Levers;                                              // 0x02F0 (size: 0x8)
    float Timeline_Alpha_8D87B80249CBA6091D3C27ABC96F0753;                            // 0x02F8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline__Direction_8D87B80249CBA6091D3C27ABC96F0753; // 0x02FC (size: 0x1)
    class UTimelineComponent* Timeline;                                               // 0x0300 (size: 0x8)
    bool DoorClosed?;                                                                 // 0x0308 (size: 0x1)
    TArray<class ABP_LaboratorySlidingDoor_C*> Doors;                                 // 0x0310 (size: 0x10)
    bool DamageToActivate?;                                                           // 0x0320 (size: 0x1)
    bool CanInteract?;                                                                // 0x0321 (size: 0x1)
    TArray<class ABP_LaboratoryLightSwitch_C*> LightSwitches;                         // 0x0328 (size: 0x10)

    void IsObjectDamageable?(bool& Damageable?);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void UserConstructionScript();
    void TL_DoorReverse__FinishedFunc();
    void TL_DoorReverse__UpdateFunc();
    void TL_Levers__FinishedFunc();
    void TL_Levers__UpdateFunc();
    void Timeline__FinishedFunc();
    void Timeline__UpdateFunc();
    void Damage_Shoved(bool Anim, double Force);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void Damage_Object(double Damage, class AActor* Damage Causer, class AController* Event Instigator);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void Event_Door();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_LaboratoryCircuitBox(int32 EntryPoint);
}; // Size: 0x338

#endif

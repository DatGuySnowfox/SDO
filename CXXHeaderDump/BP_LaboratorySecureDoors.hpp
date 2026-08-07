#ifndef UE4SS_SDK_BP_LaboratorySecureDoors_HPP
#define UE4SS_SDK_BP_LaboratorySecureDoors_HPP

class ABP_LaboratorySecureDoors_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Frame;                                                // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Mechanism;                                            // 0x02B8 (size: 0x8)
    class UArrowComponent* Debug_Front;                                               // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* DoorR;                                                // 0x02C8 (size: 0x8)
    class UStaticMeshComponent* DoorL;                                                // 0x02D0 (size: 0x8)
    float Timeline_0_Alpha_84FEEA2B4FFF2A1BDFDE8A9AADDDBDE2;                          // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_0__Direction_84FEEA2B4FFF2A1BDFDE8A9AADDDBDE2; // 0x02DC (size: 0x1)
    class UTimelineComponent* Timeline_0;                                             // 0x02E0 (size: 0x8)
    float Timeline_Alpha_257419A6479EB97F0DD42598B99925BF;                            // 0x02E8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline__Direction_257419A6479EB97F0DD42598B99925BF; // 0x02EC (size: 0x1)
    class UTimelineComponent* Timeline;                                               // 0x02F0 (size: 0x8)
    float TL_MechanismReverse_Alpha_7E046B354E2630A703D7CFA6CEFF35F1;                 // 0x02F8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> TL_MechanismReverse__Direction_7E046B354E2630A703D7CFA6CEFF35F1; // 0x02FC (size: 0x1)
    class UTimelineComponent* TL_MechanismReverse;                                    // 0x0300 (size: 0x8)
    float TL_Doors_Alpha_ABC0F994427C486A1BD96191AF2AC82C;                            // 0x0308 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> TL_Doors__Direction_ABC0F994427C486A1BD96191AF2AC82C; // 0x030C (size: 0x1)
    class UTimelineComponent* TL_Doors;                                               // 0x0310 (size: 0x8)
    float TL_Mechanism_Alpha_D699054541C534E5BB58F38E390602D5;                        // 0x0318 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> TL_Mechanism__Direction_D699054541C534E5BB58F38E390602D5; // 0x031C (size: 0x1)
    class UTimelineComponent* TL_Mechanism;                                           // 0x0320 (size: 0x8)
    class UJigsawItem_DataAsset_C* Keycard;                                           // 0x0328 (size: 0x8)
    bool IsClosed;                                                                    // 0x0330 (size: 0x1)
    bool Frame?;                                                                      // 0x0331 (size: 0x1)
    bool XPGiven?;                                                                    // 0x0332 (size: 0x1)
    bool CanOpenDoorFromBehind?;                                                      // 0x0333 (size: 0x1)
    bool Unlocked;                                                                    // 0x0334 (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void UserConstructionScript();
    void Timeline_0__FinishedFunc();
    void Timeline_0__UpdateFunc();
    void Timeline__FinishedFunc();
    void Timeline__UpdateFunc();
    void TL_MechanismReverse__FinishedFunc();
    void TL_MechanismReverse__UpdateFunc();
    void TL_Mechanism__FinishedFunc();
    void TL_Mechanism__UpdateFunc();
    void TL_Doors__FinishedFunc();
    void TL_Doors__UpdateFunc();
    void PickupBuildFromGround();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void Event_Close();
    void Event_AutoDoor();
    void SetInteractOption(FGameplayTag Option);
    void Event_Door(class AActor* Interactor);
    void OnExecuteInteractEnded();
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_LaboratorySecureDoors(int32 EntryPoint);
}; // Size: 0x335

#endif

#ifndef UE4SS_SDK_BP_SubwayTravelDoor_HPP
#define UE4SS_SDK_BP_SubwayTravelDoor_HPP

class ABP_SubwayTravelDoor_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Door;                                                 // 0x02B0 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02B8 (size: 0x8)
    float Timeline_OpenDoor_Alpha_7368E6E443BE773EDEF67A924E7C5245;                   // 0x02C0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_OpenDoor__Direction_7368E6E443BE773EDEF67A924E7C5245; // 0x02C4 (size: 0x1)
    class UTimelineComponent* Timeline_OpenDoor;                                      // 0x02C8 (size: 0x8)
    FText TravelToName;                                                               // 0x02D0 (size: 0x18)
    double TimeToTravel;                                                              // 0x02E8 (size: 0x8)
    FVector TravelToLoc;                                                              // 0x02F0 (size: 0x18)
    class UStaticMesh* DoorMesh;                                                      // 0x0308 (size: 0x8)
    class AActor* Interacting Actor;                                                  // 0x0310 (size: 0x8)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void UserConstructionScript();
    void Timeline_OpenDoor__FinishedFunc();
    void Timeline_OpenDoor__UpdateFunc();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void Event_Teleport();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void Event_Door(bool Open?);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_SubwayTravelDoor(int32 EntryPoint);
}; // Size: 0x318

#endif

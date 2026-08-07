#ifndef UE4SS_SDK_BP_WaterWell_HPP
#define UE4SS_SDK_BP_WaterWell_HPP

class ABP_WaterWell_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UActor_Weather_Status_C* Actor_Weather_Status;                              // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02B0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B8 (size: 0x8)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x02C0 (size: 0x8)
    FTimerHandle WellTimer;                                                           // 0x02C8 (size: 0x8)
    double CurrentWater;                                                              // 0x02D0 (size: 0x8)
    double MaxWater;                                                                  // 0x02D8 (size: 0x8)
    class AActor* CurrentActor;                                                       // 0x02E0 (size: 0x8)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void GetCurrentWaterStat(FText& Result);
    void DrinkWater();
    void ActorPreLoad();
    void ActorPreSave();
    void ActorSaved();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void Event_Water();
    void ActorLoaded();
    void Event_UpdateWater(double Value, bool Decrease?);
    void SetInteractOption(FGameplayTag Option);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void ExecuteUbergraph_BP_WaterWell(int32 EntryPoint);
}; // Size: 0x2E8

#endif

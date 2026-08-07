#ifndef UE4SS_SDK_BP_LaboratoryStairwellLightSwitch_HPP
#define UE4SS_SDK_BP_LaboratoryStairwellLightSwitch_HPP

class ABP_LaboratoryStairwellLightSwitch_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02B0 (size: 0x8)
    TArray<class ABP_LaboratoryStairwellLight_C*> Lights;                             // 0x02B8 (size: 0x10)
    bool TurnedOn?;                                                                   // 0x02C8 (size: 0x1)
    TArray<class ABP_LaboratoryStairwellLight_C*> References;                         // 0x02D0 (size: 0x10)

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
    void Event_LightSwitch();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void ExecuteUbergraph_BP_LaboratoryStairwellLightSwitch(int32 EntryPoint);
}; // Size: 0x2E0

#endif

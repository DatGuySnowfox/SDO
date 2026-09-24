#ifndef UE4SS_SDK_BP_LaboratoryStairwellLightSwitch_HPP
#define UE4SS_SDK_BP_LaboratoryStairwellLightSwitch_HPP

class ABP_LaboratoryStairwellLightSwitch_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B8 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02C0 (size: 0x8)
    TArray<ABP_LaboratoryStairwellLight_C*> Lights;                                   // 0x02C8 (size: 0x10)
    bool TurnedOn?;                                                                   // 0x02D8 (size: 0x1)
    TArray<ABP_LaboratoryStairwellLight_C*> References;                               // 0x02E0 (size: 0x10)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<FGameplayTag, FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<FString, FString>& CustomData);
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
    void OnBeginInteract(class AActor* InteractingActor, const TMap<FGameplayTag, FText> Options);
    void OnEndInteract();
    void ExecuteUbergraph_BP_LaboratoryStairwellLightSwitch(int32 EntryPoint);
}; // Size: 0x2F0

#endif

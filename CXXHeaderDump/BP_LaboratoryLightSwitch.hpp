#ifndef UE4SS_SDK_BP_LaboratoryLightSwitch_HPP
#define UE4SS_SDK_BP_LaboratoryLightSwitch_HPP

class ABP_LaboratoryLightSwitch_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02B0 (size: 0x8)
    TArray<class ABP_LaboratoryLight_C*> Lights;                                      // 0x02B8 (size: 0x10)
    TArray<class ABP_LaboratoryLightSwitch_C*> Switches;                              // 0x02C8 (size: 0x10)
    bool On?;                                                                         // 0x02D8 (size: 0x1)
    bool Off?;                                                                        // 0x02D9 (size: 0x1)
    bool Warning?;                                                                    // 0x02DA (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRep_Warning?();
    void OnRep_Off?();
    void OnRep_On?();
    void UserConstructionScript();
    void CallInteractOnObject();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void Event_LightSwitch();
    void SetPower_TurnedOn(bool TurnOn);
    void SetPower_Warning(bool SetWarning);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void ExecuteUbergraph_BP_LaboratoryLightSwitch(int32 EntryPoint);
}; // Size: 0x2DB

#endif

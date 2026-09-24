#ifndef UE4SS_SDK_BP_BunkerScanner_HPP
#define UE4SS_SDK_BP_BunkerScanner_HPP

class ABP_BunkerScanner_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Scanner;                                              // 0x02B8 (size: 0x8)
    TArray<AActor*> ListeningActor;                                                   // 0x02C0 (size: 0x10)
    FText ListeningActorName;                                                         // 0x02D0 (size: 0x10)
    bool KeycardRequired?;                                                            // 0x02E0 (size: 0x1)
    class UJigsawItem_DataAsset_C* Keycard;                                           // 0x02E8 (size: 0x8)
    class USoundBase* UnlockedSoundCue;                                               // 0x02F0 (size: 0x8)
    bool GiveXPOnInteraction;                                                         // 0x02F8 (size: 0x1)
    bool Unlocked?;                                                                   // 0x02F9 (size: 0x1)

    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<FString, FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetInteractOptions(TMap<FGameplayTag, FText>& Options);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void FindKeycard(class AActor* InteractingActor, bool& Found?);
    void UserConstructionScript();
    void SetPower_TurnedOn(bool TurnOn);
    void SetPower_Warning(bool SetWarning);
    void CallInteractOnObject();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<FGameplayTag, FText> Options);
    void MC_CallInteract(class AActor* InteractingActor);
    void ExecuteUbergraph_BP_BunkerScanner(int32 EntryPoint);
}; // Size: 0x2FA

#endif

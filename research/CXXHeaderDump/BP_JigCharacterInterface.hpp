#ifndef UE4SS_SDK_BP_JigCharacterInterface_HPP
#define UE4SS_SDK_BP_JigCharacterInterface_HPP

class IBP_JigCharacterInterface_C : public IInterface
{

    void SetPreviewActor(class AActor* Actor, bool& Result);
    void ExecuteExfil();
    void CancelExfil();
    void StartExfil(double Time);
    void InitiateGameSave();
    void GetTraceActorsToIgnore(TArray<class AActor*>& Actors);
    void OnClientDataRequested(TArray<FGuid>& ActionbarUIDs, bool& Result);
    void OnSpecialContainerattachmentUpdated(class UJSI_Slot_C* ItemRef, bool& Result);
    void OnLoadSavedDataRequested(bool& Result);
    void OnInteractActorOverDistance(class AActor* ActorRef, bool& Result);
    void JigCheckWeaponAmmoCount(bool& Result);
    void GetAnimationInfo(FGameplayTag& ActiveSlot, double& Leaning, bool& ADS, bool& Crouched, bool& IsFirstPerson?, class UJigsawItem_DataAsset_C*& EquippedDA);
    void OnPickupInteractExecuted(class AActor* PickupRef, class UJSIContainer_C* TargetContainer, bool& Result);
    void RequestServerData(class UActorComponent* TargetComp, bool& Result);
    void AddJigWidgetToContent(class UUserWidget* WidgetRef, FText ContentName, bool& Result);
}; // Size: 0x28

#endif

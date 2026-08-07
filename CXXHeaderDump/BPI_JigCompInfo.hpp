#ifndef UE4SS_SDK_BPI_JigCompInfo_HPP
#define UE4SS_SDK_BPI_JigCompInfo_HPP

class IBPI_JigCompInfo_C : public IInterface
{

    void JigComp_GetVendorInfo(double& Amount, class UJigsawItem_DataAsset_C*& Currency, bool& AllowSelling?);
    void JigComp_CallRequestReloadInternal(FGuid MagContainerUID, FGuid AmmoToAdd, int32 MaxCap, bool& Result);
    void JigComp_AddNewPendingRequest(class UJSI_Slot_C* NewItem, bool& Result);
    void JigComp_CallRequestReload(FGuid UnloadUID, FGuid UnloadToContainerUID, int32 ToIndex, bool FinalRot, FGuid ReloadUID, FGuid ReloadToContainerUID, class AActor* WeaponRef, bool& Result);
    void JigComp_CallRequestChamberUID(FGuid ChamberContainerUID, bool& Result);
    void JigComp_CallServerChamberWeapon(FGuid ChamberContainerUID, FGuid MagContainerUID, FGuid BulletToChamberUID, int32 ChamberContainerIndex, bool SendChamberUID?, bool& Result);
    void JigComp_ContextTryEquip(class UJSI_Slot_C* Item, class UJSIContainer_C* RootContainer, bool& Result);
    void JigComp_ContextTryUnloadAmmo(class UJSI_Slot_C* Item, class UJSIContainer_C* RootContainer, bool& Result);
    void JigComp_ContextTryUnequip(class UJSI_Slot_C* Item, class UJSIContainer_C* RootContainer, bool& Result);
    void JigComp_GetInventoryWidgetRef(class UUserWidget*& UserWidget);
    void JigComp_CallServerConsumeRequest(class UActorComponent* TargetComp, FGuid ItemUID, int32 Amount, const FString& CustomData, bool& Result);
    void JigCom_IsLootContainer(bool& Result);
    void JigCom_IsPickup(bool& Result);
    void JigCom_IsMainCharacter(bool& Result);
    void JigCom_IsVendor(bool& Result);
    void JigCom_GetType(FGameplayTag& Type);
}; // Size: 0x28

#endif

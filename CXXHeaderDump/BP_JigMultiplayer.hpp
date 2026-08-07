#ifndef UE4SS_SDK_BP_JigMultiplayer_HPP
#define UE4SS_SDK_BP_JigMultiplayer_HPP

class UBP_JigMultiplayer_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x00A8 (size: 0x10)
    TArray<class UJSIContainer_C*> LocalJSIContainers;                                // 0x00B8 (size: 0x10)
    FGameplayTag MPComponentType;                                                     // 0x00C8 (size: 0x8)
    TArray<class UJSI_Slot_C*> PendingRequests;                                       // 0x00D0 (size: 0x10)
    TArray<FGuid> MainContainersIDs;                                                  // 0x00E0 (size: 0x10)
    TArray<FS_JigCompContentSettings> ContainersSettings;                             // 0x00F0 (size: 0x10)
    double RefillContainerTimerInSeconds;                                             // 0x0100 (size: 0x8)
    TSubclassOf<class UUserWidget> InventoryWidgetClass;                              // 0x0108 (size: 0x8)
    class UUserWidget* InventoryWidgetRef;                                            // 0x0110 (size: 0x8)
    TArray<class AActor*> PendingActors;                                              // 0x0118 (size: 0x10)
    bool DoesContainerRefillLoot?;                                                    // 0x0128 (size: 0x1)
    bool IfPickup_CanLoot?;                                                           // 0x0129 (size: 0x1)
    bool AllowDroppingItems;                                                          // 0x012A (size: 0x1)
    FContainerPickupsInfo PickupInfo;                                                 // 0x0130 (size: 0xD8)
    double VendorCurrentCurrencyAmount;                                               // 0x0208 (size: 0x8)
    FVector2D VendorMaxCurrencyAmount;                                                // 0x0210 (size: 0x10)
    class UJigsawItem_DataAsset_C* VendorAcceptedCurrencyID;                          // 0x0220 (size: 0x8)
    bool VendorReSellSoldItems?;                                                      // 0x0228 (size: 0x1)
    bool AllowSellingItemsToVendor?;                                                  // 0x0229 (size: 0x1)
    TArray<FS_JigCrafting> ItemsToCraft;                                              // 0x0230 (size: 0x10)
    TArray<int32> ExcludeMainContainerIndexesFromLoot;                                // 0x0240 (size: 0x10)
    FGuid MonitorContainerUID;                                                        // 0x0250 (size: 0x10)
    FGuid PendingVendorContainerUID;                                                  // 0x0260 (size: 0x10)
    int32 PendingVendorToIndex;                                                       // 0x0270 (size: 0x4)
    bool PendingVendorFinalRot;                                                       // 0x0274 (size: 0x1)
    int32 ServerVendorRequestedBuyAmount;                                             // 0x0278 (size: 0x4)
    TArray<FS_InvWeight> InventoryWeight;                                             // 0x0280 (size: 0x10)
    TArray<int32> ExcludeMainContainerIndexesFromVendorCurrency;                      // 0x0290 (size: 0x10)
    bool DebugContent?;                                                               // 0x02A0 (size: 0x1)
    bool Looted?;                                                                     // 0x02A1 (size: 0x1)
    FTimerHandle RefillTimerHandle;                                                   // 0x02A8 (size: 0x8)
    TArray<FContainerPickupsInfo> PendingCraftingItems;                               // 0x02B0 (size: 0x10)
    TArray<double> PendingCraftingTimers;                                             // 0x02C0 (size: 0x10)
    FTimerHandle CraftingTimerHandle;                                                 // 0x02D0 (size: 0x8)
    FBP_JigMultiplayer_COnWeightUpdated OnWeightUpdated;                              // 0x02D8 (size: 0x10)
    void OnWeightUpdated();
    FBP_JigMultiplayer_COnInventoryOpenClose OnInventoryOpenClose;                    // 0x02E8 (size: 0x10)
    void OnInventoryOpenClose(bool Opened?);
    FBP_JigMultiplayer_CClientSaveDataReceived ClientSaveDataReceived;                // 0x02F8 (size: 0x10)
    void ClientSaveDataReceived();
    FBP_JigMultiplayer_COnRefillContainer OnRefillContainer;                          // 0x0308 (size: 0x10)
    void OnRefillContainer();

    void JigComp_GetVendorInfo(double& Amount, class UJigsawItem_DataAsset_C*& Currency, bool& AllowSelling?);
    void JigComp_CallRequestReloadInternal(FGuid MagContainerUID, FGuid AmmoToAdd, int32 MaxCap, bool& Result);
    void JigComp_AddNewPendingRequest(class UJSI_Slot_C* NewItem, bool& Result);
    void JigComp_CallRequestReload(FGuid UnloadUID, FGuid UnloadToContainerUID, int32 ToIndex, bool FinalRot, FGuid ReloadUID, FGuid ReloadToContainerUID, class AActor* WeaponRef, bool& Result);
    void JigComp_CallRequestChamberUID(FGuid ChamberContainerUID, bool& Result);
    void JigComp_CallServerChamberWeapon(FGuid ChamberContainerUID, FGuid MagContainerUID, FGuid BulletToChamberUID, int32 ChamberContainerIndex, bool SendChamberUID?, bool& Result);
    void JigComp_ContextTryEquip(class UJSI_Slot_C* Item, class UJSIContainer_C* RootContainer, bool& Result);
    void JigComp_ContextTryUnloadAmmo(class UJSI_Slot_C* Item, class UJSIContainer_C* RootContainer, bool& Result);
    void JigComp_ContextTryUnequip(class UJSI_Slot_C* Item, class UJSIContainer_C* RootContainer, bool& Result);
    void JigComp_CallServerConsumeRequest(class UActorComponent* TargetComp, FGuid ItemUID, int32 Amount, const FString& CustomData, bool& Result);
    void JigComp_GetInventoryWidgetRef(class UUserWidget*& UserWidget);
    void JigCom_IsLootContainer(bool& Result);
    void JigCom_IsPickup(bool& Result);
    void JigCom_IsMainCharacter(bool& Result);
    void JigCom_IsVendor(bool& Result);
    void JigCom_GetType(FGameplayTag& Type);
    void CheckVendorAmountOfMoney(double Price, class UBP_JigMultiplayer_C* Comp, bool Selling?, bool& Enough?);
    void PickupInfoFromActor(class AActor* Target, int32 SlotIndex, bool Rotated, FContainerPickupsInfo& Result);
    void ServerFunc_HandleActorToCapacityContainer(class AActor* ActorRef, class UBP_JigMultiplayer_C* ToComp, FGuid ToContainerUID, int32 ToIndex, bool Rotated, FGuid PendingUID, bool& Proceed?, bool& Fail?);
    void FindItem_Pure(class UJigsawItem_DataAsset_C* DA, bool SearchInEquipTo?, int32 RequiredAmount, bool& Found?);
    void ReduceDurabilityOfFoundItem(class UJigsawItem_DataAsset_C* DA, double MinusDurability, int32 RequiredAmount, bool SearchInEquipTo?, bool DestroyItemIfZeroDur?, bool& Found?);
    void JigTryAddItemSomewhere(class UBP_JigMultiplayer_C* LocalComp, class UJigsawItem_DataAsset_C* ItemId, int32 Count, bool& Added?, FGuid& UID);
    bool IsSCSupportedItem(class UBP_MainSpecialContainer_C* Container, class UJigsawItem_DataAsset_C* Item);
    void SwapTwoItems(FGuid Item1, FGuid Item2, int32 RemoveCount);
    void GetDurability(const FGuid& UID, double& NewDur);
    void HandleServerFuncRequestCraftDrop(class UBP_JigMultiplayer_C* CompRef, int32 CraftIndex, TArray<FGuid>& ConsumeUIDs);
    void FindItemBySubContainerUID(FGuid& UID, bool& Found, FGuid& ItemUID);
    void ClientFunc_UpdateCustomData(FGuid ItemUID, TArray<FString>& Keys, TArray<FString>& Values);
    void ServerFunc_UpdateCustomDataByUID(FGuid UID, TArray<FString>& Keys, TArray<FString>& Values, bool& Updated);
    void GetCapacity(int32& Output_Get);
    void FindJigContainerByUID(FGuid& UID, FS_ReplicatedContainerInfo& Output, int32& Index);
    void AdjustNewContainerSize(class UJigsawItem_DataAsset_C* DA);
    void FindJigItemByDA(class UJigsawItem_DataAsset_C* DA, FGameplayTagContainer SearchOnlyInSlots, bool DeepSearch?, bool SearchInEquipTo?, int32 RequiredAmount, bool& Result, class UJSI_Slot_C*& Found);
    bool IsSupportedItem(class UJigsawItem_DataAsset_C* Container, class UJigsawItem_DataAsset_C* Item);
    void FixUpReferences();
    void BulkUpdateStats(FGameplayTag StatTag, double Value, bool Override?);
    void DropAllItems();
    void LogData();
    void DetectDuplicateIDs(bool& Result);
    void GetTotalCountOfItem(class UJigsawItem_DataAsset_C* DA, int32& TotalCount);
    bool Controller?();
    void ToggleInventory();
    bool IsEmpty?();
    void CheckIfItemsExist(TArray<class UJigsawItem_DataAsset_C*>& Items, bool& Result);
    void UpdatePickupInfo(FContainerPickupsInfo PickupInfo);
    void GetSearchableContainers(TArray<class UJSIContainer_C*>& Result);
    void FindAndConsume(class UJigsawItem_DataAsset_C* ItemDA, int32 Count);
    void Get Index From CraftingArr(TArray<FRepItemInfo>& Arr, class UJigsawItem_DataAsset_C* ID, int32& Array Index);
    void ServerFuncUpdateDurabilityByUID(FGuid UID, double MinusDur, bool DestroyItemIfZeroDur?, double& NewDur);
    void InitNoStartingItems();
    void SetContainerSettingsFromWidget();
    void AdjustEquipToContainerSettings();
    void RefFillCapacityContainer(FGuid UID, class UJigsawItem_DataAsset_C* WithItem);
    void SetVendorPriceOfSoldItem(FContainerPickupsInfo& Info, TArray<FS_ReplicatedContainerInfo>& Containers);
    void FuncHandleCraftingTimers();
    bool IsCraftingStation();
    void VendorCompareItems(FContainerPickupsInfo& Item1, TArray<FS_ReplicatedContainerInfo>& Item1SubContainers, FContainerPickupsInfo& Item2, bool& Proceed?);
    void GetContainerDimensions(class UJigsawItem_DataAsset_C* Asset, TArray<FVector2D>& SpecialContainerDemensions);
    bool UIDNotValid(const FGuid& InGuid);
    void SetupWidgetCleanupTimer();
    void ClearContainerWidgets();
    void CheckCanClearContainer(bool& Result);
    void ClearContainer(bool ResetInit?);
    void ProcessRefillContainer();
    void CheckContainerRefill(bool BeginPlay?);
    void GetInventoryWidgetReference(class UUserWidget*& InventoryWidgetRef, bool& IsVisible?);
    void GetParentItemByContainerUID(FGuid ContainerUID, FContainerPickupsInfo& Array Element, bool& Found);
    void JSI_CheckIfCanAddStackItemToContainer(class UJSI_Slot_C* ItemRef, bool& Result, class UJSI_Slot_C*& WithItem);
    bool SaveMultipleJigContainersByIndexes(FString SaveName, const int32 UserIndex, TArray<int32>& ContainerIndex);
    void HandleLocalInternalReload_CustomCount(FGuid MagContainerUID, FGuid AmmoToAdd, int32 MaxCap);
    void CanLootPickup?(bool& Result);
    bool HasValidWidgetClass?();
    void ClientFunc_UpdateStatByUID(FGuid UID, FGameplayTag STAT_TAG, double NewValue);
    void UpdateStatByUID(FGuid UID, FGameplayTag STAT_TAG, double NewValue);
    void ServerFuncExpandContainer(FGuid ContainerUID, int32 AddColumns, int32 AddRows);
    void JSI_CheckIfEnoughSpaceToPickupActor(class AActor* ActorRef, bool& Result);
    void ClientSetupSwappingTwoContainers(FGuid Container, FGuid SecondaryContainer);
    void ClientHandleTwoContainersSwap(FGuid ContainerUID, FGuid SecondContainerUID, bool Success);
    void HandleTwoContainerSwap(FGuid ContainerUID, FGuid SecondContainerUID);
    void FireArmGetChamberUID(FGuid ChamberContainerUID);
    void FireArmChamberWeapon(FGuid ChamberContainerUID, FGuid MagContainerUID, FGuid BulletToChamberUID, int32 ChamberContainerIndex, bool SendChamberUID?);
    void FindRootContainer_Pure(FGuid ContainerUID, FGuid& RootCUID);
    void GenerateRandomStats(FS_RandomStatsConfig RandomConfig, TArray<FS_ItemStat>& DTStats, TArray<FS_ItemStat>& Stats);
    void VendorTryStackMoney(int32 MaxStack, FRepItemInfo Currency, bool& Stacked?);
    void ClientFunc_GetDesiredMoneyLocation(class UBP_JigMultiplayer_C* VendorCom, class UJSI_Slot_C* DroppedItem, bool& Found, FGuid& ToContainerID, int32& ToIndex);
    void AddItemToContainerByName(FName ContainerName, class UJigsawItem_DataAsset_C* ItemId, int32 Count, bool TopLevelContainersOnly?, class UJigsawItem_DataAsset_C* ItemInside, int32 ItemInsideCount);
    void GetAllItemsSellPrice(FContainerPickupsInfo Item, int32 Amount, double& Price);
    void GetEquippedItemByContainerUID(FGuid ContainerUID, bool OnlyItems, bool& Found?, bool& IsEquipTo, FContainerPickupsInfo& Info);
    void UpdateCountByByMotherUID(FGuid MotherUID, int32 NewCount, bool& Updated?);
    void FindItemByMotherUID(FGuid MotherUID, bool& Found?, FContainerPickupsInfo& FoundItem);
    void GetContainerItemByInContainerIndex(FGuid ContainerUID, int32 ContainerIndex, bool& Found?, FContainerPickupsInfo& FoundItem);
    void GetMainContainerUIDByName(FName ContainerName, FGuid& UID, int32& Index, bool& Found?, bool& IsEquipTo?);
    void ServerFunc_UpdateDurabilityByUID(FGuid UID, FVector2D NewDurability, bool& Updated);
    void ClientFunc_UpdateDurability(FGuid ItemUID, FVector2D NewDurab);
    void AdjustCrafting();
    bool IsLocal();
    void GetTotalInventoryWeight(double& Output_Get);
    void GetInventoryWeightByMainContainerName(FName Name, double& Weight);
    void OnItemRemovedUpdateWeight(FContainerPickupsInfo& Item, TArray<FS_ReplicatedContainerInfo>& Content, int32 OverrideCount);
    void UpdateWeight(FGuid ContUID, double Weight);
    void OnItemAddedUpdateWeight(FContainerPickupsInfo& Item, TArray<FS_ReplicatedContainerInfo>& Content, FGuid OverrideInUID, int32 OverrideCount);
    void FindRootContainer(FGuid ContainerUID, FGuid& RootCUID);
    void InitialzieInventoryWeight();
    void JSI_CheckIfCanAddItemToContainer(class UJSI_Slot_C* ItemRef, bool& Result, class UJSIContainer_C*& AddToContainer, int32& ToIndex, bool& FinalRotation);
    void ServerFindThenConsumeByItemID(class UJigsawItem_DataAsset_C* ItemId, int32 Count, bool& Result);
    void AddNewItemSomewhere(class UJigsawItem_DataAsset_C* ItemId, int32 Count, class UJigsawItem_DataAsset_C* ItemInside, int32 ItemInsideCount, bool DropIfFailedToAdd?, bool& Added, bool& Dropped?, FGuid& ItemUID, FGuid& AddedToUID);
    void MonitorContainerItems();
    void IsInventoryVisible?(bool& Result);
    void ClientFunc_CheckIfCanStackAddActor(class AActor* ActorRef, bool TestOnlly, bool& Handled, FGuid& WithUID, TArray<FGuid>& PartialStacks);
    void ServerFunc_ProcessRequestStackAddActor(class AActor* ActorRef, FGuid WithUID);
    void GetAllContainersOfUIDCustom(FGuid ContainerUID, TArray<FGuid>& ListFound, TArray<FS_ReplicatedContainerInfo>& Containers);
    bool ReplaceLoadJigComponentByIndex(FString SaveName, const int32 UserIndex, int32 ReplaceContainerIndex, TArray<FGuid>& ActionbarUIDs);
    void GetAllContainersOfUID(FGuid ContainerUID, TArray<FGuid>& ListFound);
    void GetMainContainerUIDByIndex(int32 Index, FGuid& UID, int32& ContainerIndex);
    void GetAllContainerByItemUID(FGuid ItemUID, TArray<FS_ReplicatedContainerInfo>& ContainersRef);
    bool Save One Jig ContainerByIndex(FString SaveName, const int32 UserIndex, int32 ContainerIndex);
    bool LoadJigComponent(TArray<FGuid>& ActionbarUIDs);
    bool SaveJigComponent(TArray<FGuid>& ActionbarUIDs);
    void HandleMainPlayerRequestedData();
    void CheckIfCapacityContainer(FGuid ItemMotherUID, bool& IsCapacity);
    void SetActorRefByItemUID(FGuid UID, class AActor* ActorRef);
    void ClientFuncItemCrafted(FGuid TempUID, FContainerPickupsInfo ItemAdded, bool Success?, TArray<FGuid>& UpdateUIDS, TArray<int32>& WithCounts, bool IsCraftingStation?, FContainerPickupsInfo ItemInside);
    void JigTryAddGetPendingRef(FRepItemInfo Info, bool CheckStacking, class UJSI_Slot_C*& OutputPin, bool& Stack?);
    void HandleServerFuncRequestCraft(class UBP_JigMultiplayer_C* CompRef, int32 CraftIndex, TArray<FGuid>& ConsumeUIDs, FGuid AddToContainer, int32 ToIndex, bool FinalRotation, FGuid TempUID, FGuid StackWith);
    void FindCraftingIngredient(class UUserWidget* OneCont, class UJigsawItem_DataAsset_C* ID, int32 Count, TArray<FGuid>& Exclude, TArray<class UJSI_Slot_C*>& OutputPin, bool& Result);
    void CheckCraftingIngredients(FRepItemInfo RequiredItem, TArray<FGuid>& Exclude, TArray<class UJSI_Slot_C*>& ItemFound Element);
    bool IsLootContainer();
    void Context_TryUnloadAmmo(class UJSI_Slot_C* ItemRef, class UJSIContainer_C* RootContainer);
    void Context_TryEquip(class UJSI_Slot_C* EquippedItem, class UJSIContainer_C* JSIRootCont);
    void Context_TryUnequip(class UJSI_Slot_C* EquippedItem, class UJSIContainer_C* JSIRootCont);
    void HandleLocalInternalReload(FGuid MagContainerUID, FGuid AmmoToAdd, int32 MaxCap);
    void AddNewPendingRequest(const class UJSI_Slot_C*& NewItem);
    void ClientFuncHandleReloadResponse(bool Success?, FGuid UnloadUID, FGuid UnloadToContainerUID, int32 ToIndex, bool FinalRot, FGuid ReloadUID, FGuid ReloadToContainerUID);
    void HandleLocalRequestReloadWeapon(FGuid UnloadUID, FGuid UnloadToContainerUID, int32 ToIndex, bool FinalRot, FGuid ReloadUID, FGuid ReloadToContainerUID, class AActor* WeaponRef);
    void HandleLocalRequestConsumeItem(FGuid ItemUID, int32 Amount, class UBP_JigMultiplayer_C* LocalComp, FString CustomData, bool CallbackToPlayer?);
    void ServerFunc_HandleMoveToCapacityContainer(class UBP_JigMultiplayer_C* FromComp, class UBP_JigMultiplayer_C* ToComp, FGuid ItemUID, FGuid ToContainerUID, int32 ToIndex, bool& Proceed?, bool& Fail?);
    bool CheckCapacityContainer(FRepItemInfo ContainerInfo, FGuid ToContainerUID, int32& FreeSpace, TArray<class UJigsawItem_DataAsset_C*>& AvailableIDs, int32& Index);
    void GetPickupInfoFromDT(class UJigsawItem_DataAsset_C* ID, int32 Count, FContainerPickupsInfo& ContainerPickupsInfo, bool& Found);
    void GetListOfAttachments(FGuid ItemUID, TArray<FS_RepAttachmentInfo>& Attachments);
    void Find Pickup Ref by AttachmentMotherUID(FGuid ContainerUID, class AActor*& PickupRef, FGuid& ItemUID);
    void Find Actor Ref byItemUID(FGuid UID, class AActor*& PickupRef);
    void GetCurrencyFromDT(double Price, class UJigsawItem_DataAsset_C* Asset, bool& Found, int32& MaxStack, FContainerPickupsInfo& Currency);
    void GetSubContainersOnce(FGuid UID, TArray<FS_ReplicatedContainerInfo>& RepContainerRef);
    bool TakeMoney(int32 Price, class UJigsawItem_DataAsset_C* Currency, TArray<FS_UpdateCount>& UpdateMoneyCount);
    bool CheckIfHasEnoughMoney(int32 Price, class UJigsawItem_DataAsset_C* Currency);
    bool IsVendor();
    bool IsValidPickup(FContainerPickupsInfo& PickupInfo);
    void SetPickupContainerData(FContainerPickupsInfo PickupInfo, TArray<FS_ReplicatedContainerInfo>& MainReplicatedContainers);
    void HandleServerDropRequest(FGuid ItemUID, int32 Count, class UBP_JigMultiplayer_C* LocalComp);
    void GetContainerNameByUID(FGuid UID, FName& ContainerName);
    void ClientFunc_HandleActorAddedToContainer(class AActor* ActorRef, FContainerPickupsInfo ItemAdded, const TArray<FContainerPickupsInfo>& ContainerContent, FGuid TempUID, TArray<FS_UpdateCount>& UpdateCounts);
    void ServerFunc_HandleRequestAddActorToContainer(class AActor* ActorRef, FGuid AddtoUID, FGuid TempUID, int32 ToIndex, bool FinalRotation, TArray<FGuid>& Stacks);
    void JSI_CheckIfCanAddActorToContainer(class AActor* ActorRef, bool TestOnly?, class UJSIContainer_C* ToSpecificContainer, int32 Index, bool Rotated, bool& Result, FGuid& AddtoUID, FGuid& TempItemUID, int32& ToIndex, bool& FinalRotation, class UJSI_Slot_C*& JigItemRef);
    void HandleActorEquipped(class AActor* ActorRef, FContainerPickupsInfo& ItemAdded, const TArray<FContainerPickupsInfo>& ContainerContent);
    void JSI_TryEquipToMain(class AActor* PickupRef, class UJSIContainer_C* TargetContainer, bool OnlyTest?, class UJSIContainer_C*& EquipTo);
    void ServerFuncHandleEquipActor(class AActor* ActorRef, FGuid ToMainUID);
    bool IsPickupComponent();
    bool IsMainCharacter();
    void FuncHandleStackResponse(FGuid DroppedUID, int32 DropNewCount, FGuid RecUID, int32 RecNewCount);
    void ServerFunc_RequestStackItem(class UBP_JigMultiplayer_C* FromCom, class UBP_JigMultiplayer_C* ToComp, FGuid DroppedUID, FGuid ReceiverUID, int32 MaxStack, bool& Success);
    void JSIFindEmptySpot(class UJSIContainer_C* Container, class UJSI_Slot_C* Item, bool& Found, int32& Index, bool& FinalRotation);
    void Jig_UnhandledEvent(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, int32 ToIndex, class UJSI_Slot_C* DroppedItem, bool Rotated, int32 Amount);
    void ServerUpdateCountByUID(FGuid ItemUID, int32 NewCount);
    void HandleNewItemAdded(FRepItemInfo ItemInfo, FGuid UID, FGuid ToContainerUID, int32 ToIndex, bool FinalRotation, TArray<FS_SubContainerInfo>& SubContainers);
    void FindItemByUID(FGuid UID, class UJSI_Slot_C*& Found);
    void HandleSameCompSplitResult(const FGuid& SourceUID, int32 SourceNewCount, FRepItemInfo NewItem, FGuid NewItemUID, FGuid ToContainerUID, int32 ToSlot, bool Rotated);
    void RemoveRequestByUID(FGuid UID, bool ClearWidget?, bool& Removed);
    void ClientUpdateCount(FGuid ItemUID, int32 NewCount, bool RemoveIf0);
    void AddNewItemToSlot(FContainerPickupsInfo& ItemInfo, FGuid ToContainer, int32 ToIndex, TArray<FS_ReplicatedContainerInfo>& ContainerContent, bool SetUID?, bool& Added, FContainerPickupsInfo& AddedItemInfo);
    void ServerHandleSplitRequest(class UBP_JigMultiplayer_C* FromComp, class UBP_JigMultiplayer_C* ToComp, FGuid ItemUID, FGuid ToContainerUID, int32 ToIndex, int32 SplitAmount, bool Rotated?);
    void OnRep_MainContainersIDs();
    void InitInventoryData();
    void SetupInventoryWidget(bool ClearInv?, bool ReInitWidget?, bool ForceInitSP?, class UUserWidget*& InventoryWidgetRef);
    void Generate_AddNewItemToContainer(FGuid ToContainerUID, class UJigsawItem_DataAsset_C* ItemId, int32 Count, FContainerPickupsInfo& AddedItemInfo, bool& Added?);
    FVector2D GetVectorRot(FVector2D Vect, bool Rotated);
    void SetupNewAddedItem(class UJSIContainer_C* JSIRef, TArray<FContainerPickupsInfo>& AllItems);
    void MainDeepSearchContainer(FGuid UID, class UJSIContainer_C*& Array Element);
    void HandleAddNewItem(FContainerPickupsInfo& ItemInfo, const TArray<FContainerPickupsInfo>& Content);
    void AddItemFromComp(FContainerPickupsInfo& ItemInfo, TArray<FS_ReplicatedContainerInfo>& SubContainers, FGuid ToUID, bool FinalRotation, int32 ToIndex, int32 VendorAmount, bool& VendorCountUpdate);
    void FindItemInContainer(class UJSIContainer_C* InContainer, FGuid UID, class UJSI_Slot_C*& Found);
    void DeepSearchForItem(FGuid UID, class UJSI_Slot_C*& ItemFound);
    void GetSubContainers(FGuid UID, TArray<FS_ReplicatedContainerInfo>& RepContainerRef, TArray<FGuid>& UIDList, TArray<int32>& IndexList, bool SkipCapacity?);
    void RemoveGetInfo(FGuid UID, int32 RemoveCount, FContainerPickupsInfo& ItemInfo, TArray<FS_ReplicatedContainerInfo>& RepContainers, int32& VendorNewCount, bool& CountRemoved?);
    void CanAddItem(FContainerPickupsInfo& Info, FGuid ToContainerUID, int32 Index, bool FinalRotation, bool& Result, bool& ToMain?);
    void GetItemByUID(FGuid& UID, FContainerPickupsInfo& Item, bool& Found, bool& InMainContainer?);
    void Handle Comp to Comp Move(class UBP_JigMultiplayer_C* From, class UBP_JigMultiplayer_C* To, FGuid ItemUID, bool FinalRotation, int32 ToIndex, FGuid ToContainer, int32 VendorAmount, FGuid VendorMoneyToUID, int32 VendorMoneyToIndex);
    void Process Multiple(TArray<FVector2D>& ContainerColRow, TArray<FGuid>& ContainerUIDs);
    void ClearJSIItems();
    void AddJSIContainer(const class UJSIContainer_C*& InputPin);
    void GetContainerIndex(class UJSI_Slot_C* ItemRef, int32& Index);
    void SetPermissions(class UJSIContainer_C* JSITarget);
    void HandleClientMoveItemToIndex(FGuid ItemUID, FGuid ToContainerUID, int32 ToIndex, bool FinalRotation);
    void CanAddToContainerIgnoreIndexes(FS_ReplicatedContainerInfo& RepContainer, int32 ToIndex, FVector2D Vec, TArray<int32>& Ignore, bool& Proceed);
    void GetLocalComp(class UBP_JigMultiplayer_C*& JigComp);
    void HandleSameContainerMoveToIndex(FGuid ItemUID, FGuid ToContainerUID, int32 ToIndex, bool Rotated?, class UBP_JigMultiplayer_C* LocalComp, int32 InContainerIndex);
    void DeepSearchForContainer(FGuid UID, class UJSIContainer_C* Target, class UJSIContainer_C*& Array Element);
    void BuildDataToContainer(TArray<FContainerPickupsInfo>& Data);
    void FindJSIContainer(FGuid UID, class UJSIContainer_C*& JSIRef, bool& Found);
    void GetContainerItems(bool RemoveExcluded?, TArray<FContainerPickupsInfo>& AllItems);
    void GetMainContainerUIDS(TArray<FGuid>& UIDs);
    void GetUniqueID(FGuid& UID);
    void GetColumnRowBySlotIndex(int32 NumOfColumns, int32 SlotIndex, int32& Column, int32& Row);
    void GetHostingSlots(int32 NumOfColumns, int32 SlotIndex, FVector2D ItemVec, int32 NumOfRows, TArray<int32>& Indexes);
    void TryAddNewItem(FS_ReplicatedContainerInfo& RepContainer, FRandomContainerItem& ItemToAdd, bool CheckWeight?, bool TryStacking?, int32 CustomMaxStack, FContainerPickupsInfo& AddedItemInfo, bool& Added?, bool& Stacked?);
    void GetEmptySlotTryRotated(FS_ReplicatedContainerInfo& RepContainer, FVector2D ItemVec, bool& Found, int32& Index, bool& Rotated?);
    int32 GetSlotIndexByColumnRow(int32 NumOfColumns, int32 Column, int32 Row);
    void GetEmptySlot(FS_ReplicatedContainerInfo& RepContainer, FVector2D ItemVec, bool& Found, int32& Index);
    void BuildContainerSlots(TArray<FS_ContainerSlots>& SlotArr, int32 Columns, int32 Rows);
    bool IsLocalPlayer();
    void Process Main Jig Containers(FName Name, bool IsEquipTo, int32 NumOfColumns, int32 NumOfRows, TArray<FRandomContainerItem>& DefaultContainerItems, FGuid& ContUID);
    void HandleCraftingTimers();
    void Drop_ItemOverItem_Event_0(class UJSI_Slot_C* ItemReceiver, class UJSI_Slot_C* ItemDropped, class UJSIContainer_C* ItemReceiverContainer, class UJSIContainer_C* ItemDroppedContainer);
    void HandleItemOverItem(class UJSIContainer_C* Container);
    void EventOnJigItemMouseButtonDown(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, FKey Button);
    void HandleMouseButtonDown(class UJSIContainer_C* Container);
    void ItemDropRequest_Event_0(class UJSI_Slot_C* ItemRef, int32 Count, class UJSIContainer_C* Container);
    void HandleDropRequest(class UDropItemBackGwidget_C* DropWidget);
    void OnItemStackRequest_Event(class UJSI_Slot_C* DroppedItem, class UJSI_Slot_C* ReceiverItem);
    void OnItemSplitRequest_Event(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* ItemSource, int32 ToSlotIndex, int32 TotalAmount, int32 SplitAmount, bool Rotated?);
    void EventOnInventoryAction(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* DroppedSlotRef, class UJSI_Slot_C* SlotReceiverRef, int32 ToSlotIndex, bool Rotated?);
    void BindMainEvents(class UJSIContainer_C* Container);
    void SERVER_RequestRemoveItem(class UBP_JigMultiplayer_C* TargetComp, FGuid ItemUID, int32 Amount, bool CallbackToPlayer?);
    void MC_UpdateDurability(FGuid ItemUID, FVector2D NewDurab);
    void SERVER_RequestCraftItemDrop(class UBP_JigMultiplayer_C* CompRef, int32 CraftIndex, const TArray<FGuid>& ConsumeUIDs);
    void CLIENT_UpdateCustomValues(FGuid ItemUID, const TArray<FString>& Keys, const TArray<FString>& Values);
    void SERVER_LoadSaveDataFromClientSide(const TArray<FS_ReplicatedContainerInfo>& MainJigContainers, const TArray<FGuid>& MainContainersIDs, const TArray<FS_InvWeight>& InventoryWeight, const TArray<FGuid>& ActionbarUIDs);
    void SERVER_ForwardInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload);
    void MC_OnPendingDone(const TArray<FContainerPickupsInfo>& Item);
    void ResetInitComponent();
    void SERVER_RequestExpandContainer(FGuid ContainerUID, int32 AddColumns, int32 AddRows);
    void CLIENT_ExpandContainer(FGuid ContainerUID, int32 AddedColumn, int32 AddedRow);
    void CLIENT_SwapContainersResponse(FGuid ContainerUID, FGuid SecondContainerUID, bool Success);
    void SERVER_RequestSwapContainers(FGuid ContainerUID, FGuid SecondContainerUID);
    void SERVER_RequestChamberUID(FGuid ChamberContainerUID);
    void CLIENT_UpdateChamberBulletUID(FGuid NewUID);
    void SERVER_FireArmChamberWeapon(FGuid ChamberContainerUID, FGuid MagContainerUID, FGuid BulletToChamberUID, int32 ChamberContainerIndex, bool SendChamberUID?);
    void CLIENT_UpdateDurability(FGuid ItemUID, FVector2D NewDurab);
    void SERVER_RequestActorAddStackItem(class AActor* ActorRef, FGuid WithUID);
    void CLIENT_AddStackActorResponse(bool Success, FGuid ItemUID, int32 NewCount, class AActor* ActorRef);
    void SERVER_RequestLoadReplaceOneContainerByIndex(FString SaveName, const int32 UserIndex, int32 ReplaceContainerIndex);
    void SERVER_RequestSaveOneJigContainerByIndex(FString SaveName, const int32 UserIndex, int32 ContainerIndex);
    void SERVER_RequestSaveGame(class UBP_JigMultiplayer_C* MpRef, const TArray<FGuid>& ActionbarUIDs);
    void SERVER_RequestDataFromSave(class UBP_JigMultiplayer_C* JigMpCompRef);
    void CLIENT_ItemCraftedResponse(FGuid TempUID, FContainerPickupsInfo ItemAdded, bool Success?, const TArray<FGuid>& UpdateUIDS, TArray<int32>& WithCounts, bool IsCraftingStation?, FContainerPickupsInfo ItemInside);
    void SERVER_RequestCraftItem(class UBP_JigMultiplayer_C* CompRef, int32 CraftIndex, const TArray<FGuid>& ConsumeUIDs, FGuid AddToContainer, int32 ToIndex, bool FinalRotation, FGuid TempUID, FGuid StackWith);
    void SERVER_RequestReloadInternal(FGuid MagContainerUID, FGuid AmmoToAdd, int32 MaxCap);
    void CLIENT_ReloadResponse(bool Success?, FGuid UnloadUID, FGuid UnloadToContainerUID, int32 ToIndex, bool FinalRot, FGuid ReloadUID, FGuid ReloadToContainerUID);
    void SERVER_RequestReload(FGuid UnloadUID, FGuid UnloadToContainerUID, int32 ToIndex, bool FinalRot, FGuid ReloadUID, FGuid ReloadToContainerUID, class AActor* WeaponRef);
    void SERVER_RequestConsumeItem(class UBP_JigMultiplayer_C* TargetComp, FGuid ItemUID, int32 Amount, FString CustomData);
    void MC_UpdateCounts(const TArray<FS_UpdateCount>& Counts);
    void CLIENT_UpdateCounts(const TArray<FS_UpdateCount>& Counts, FGuid TempUID);
    void CLIENT_DropRequestFailed(FGuid ItemUID);
    void SERVER_RequestDropItem(FGuid ItemUID, int32 Count, class UBP_JigMultiplayer_C* CompTarget);
    void CLIENT_MoveItemToIndex(FGuid ItemUID, FGuid ToContainerUID, int32 ToIndex, bool FinalRotation);
    void CLIENT_AddActorToContainerSUCCESS(class AActor* ActorRef, FContainerPickupsInfo ItemAdded, const TArray<FContainerPickupsInfo>& ContainerContent, FGuid TempUID, const TArray<FS_UpdateCount>& UpdateCounts);
    void CLIENT_RequestAddActorToContainerFAILED(class AActor* ActorRef, FGuid TempUID);
    void SERVER_RequestAddActorToContainer(class AActor* ActorRef, FGuid AddtoUID, FGuid TempUID, int32 ToIndex, bool FinalRotation, const TArray<FGuid>& Stacks);
    void CLIENT_EquipActorSuccess(class AActor* ActorRef, FContainerPickupsInfo ItemAdded, const TArray<FContainerPickupsInfo>& ContainerContent);
    void CLIENT_RequestEquipResponseFailed(FGuid MainContainerUID, class AActor* ActorRef);
    void SERVER_RequestEquipActorToContainer(class AActor* ActorRef, FGuid ToMainUID);
    void MC_AddNewItem(FContainerPickupsInfo ItemInfo, const TArray<FContainerPickupsInfo>& Content);
    void MC_UpdateStack(FGuid DroppedUID, int32 DropNewCount, FGuid RecUID, int32 RecNewCount);
    void CLIENT_UpdateStack(FGuid DroppedUID, int32 DropNewCount, FGuid RecUID, int32 RecNewCount);
    void SERVER_RequestStackItem(class UBP_JigMultiplayer_C* FromCom, class UBP_JigMultiplayer_C* ToComp, FGuid DroppedUID, FGuid ReceiverUID, int32 MaxStack);
    void CLIENT_NewItemAdded(FRepItemInfo ItemInfo, FGuid UID, FGuid ToContainerUID, int32 ToIndex, bool FinalRotation, const FS_UpdateCount UpdateCount, FGuid RemovePending, TArray<FS_SubContainerInfo>& SubContainers, bool ClearPendingW?);
    void MC_NewItemAdded(FRepItemInfo ItemInfo, FGuid UID, FGuid ToContainerUID, int32 ToIndex, bool FinalRotation, FS_UpdateCount UpdateCount, TArray<FS_SubContainerInfo>& SubContainers);
    void Client_SameCompUpdateSplitRequest(const FGuid& SourceUID, int32 SourceNewCount, FRepItemInfo NewItem, FGuid NewItemUID, FGuid ToContainerUID, int32 ToSlot, bool Rotated);
    void CLIENT_UpdateCount(class UBP_JigMultiplayer_C* TargetComp, FGuid ItemUID, int32 NewCount, FGuid RemovedPending);
    void MC_UpdateCount(FGuid ItemUID, int32 NewCount, FGuid RemovePending);
    void MC_SameCompUpdateSplitRequest(const FGuid& SourceUID, int32 SourceNewCount, FRepItemInfo NewItem, FGuid NewItemUID, FGuid ToContainerUID, int32 ToIndex, bool Rotated);
    void SERVER_RequestSplitItem(class UBP_JigMultiplayer_C* FromComp, class UBP_JigMultiplayer_C* ToComp, FGuid ItemUID, FGuid ToContainerUID, int32 ToIndex, int32 SplitAmount, bool Rotated?);
    void InitializeJigComponent();
    void CLIENT_AddNewItem(FContainerPickupsInfo ItemInfo, const TArray<FContainerPickupsInfo>& Content, class UBP_JigMultiplayer_C* Comp, FGuid PendingUID);
    void CLIENT_ItemRemoved(FGuid UID, class UBP_JigMultiplayer_C* Comp, bool SkipPendingRequests?);
    void MC_ItemRemoved(const FGuid& ItemUID);
    void SERVER_RequestMoveItemToAnotherComp(class UBP_JigMultiplayer_C* From, class UBP_JigMultiplayer_C* To, FGuid ItemUID, bool FinalRotation, int32 ToIndex, FGuid ToContainer, int32 VendorAmount, FGuid VendorMoneyToUID, int32 VendorMoneyToIndex);
    void MC_MoveItemToIndex(class UBP_JigMultiplayer_C* TargetComp, FGuid ItemUID, FGuid ToContainerUID, int32 ToIndex, bool FinalRotation);
    void CLIENT_MoveToIndexFailed(const TArray<FGuid>& ItemUID, class UBP_JigMultiplayer_C* JijgCompTarget);
    void SERVER_SameContainer_MoveToIndex(class UBP_JigMultiplayer_C* JigMpCompRef, FGuid ItemUID, FGuid ToContainerUID, int32 ToIndex, bool Rotated?, int32 InContainerIndex);
    void CLIENT_ContainerDataResponse(const TArray<FGuid>& MainContainersUIDs, const TArray<FContainerPickupsInfo>& ContainerItems, class UBP_JigMultiplayer_C* JigMpCompRef, const TArray<FGuid>& ActionbarUIDs);
    void SERVER_RequestData(class UBP_JigMultiplayer_C* JigMpCompRef);
    void StartRefillTimer(bool ClearContainer?);
    void WaitFullReplicationOfUIDs(bool Reset?);
    void ReceiveBeginPlay();
    void CLIENT_OnStatsUpdated(const TArray<FGuid>& UID, const FGameplayTag STAT_NAME, const TArray<double>& Values);
    void CLIENT_OnStatUpdated(FGuid UID, FGameplayTag STAT_TAG, double NewValue);
    void SERVER_UpdateStatByUID(FGuid UID, FGameplayTag STAT_NAME, double NewValue);
    void ExecuteUbergraph_BP_JigMultiplayer(int32 EntryPoint);
    void OnRefillContainer__DelegateSignature();
    void ClientSaveDataReceived__DelegateSignature();
    void OnInventoryOpenClose__DelegateSignature(bool Opened?);
    void OnWeightUpdated__DelegateSignature();
}; // Size: 0x318

#endif

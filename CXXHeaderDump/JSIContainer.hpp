#ifndef UE4SS_SDK_JSIContainer_HPP
#define UE4SS_SDK_JSIContainer_HPP

class UJSIContainer_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* ContainerBorder;                                                   // 0x02C8 (size: 0x8)
    class UTextBlock* ContainerNameTxt;                                               // 0x02D0 (size: 0x8)
    class UJSIActivable_C* JSIActivable;                                              // 0x02D8 (size: 0x8)
    class UOverlay* MainOverlay;                                                      // 0x02E0 (size: 0x8)
    class USizeBox* NameSBox;                                                         // 0x02E8 (size: 0x8)
    class UUniformGridPanel* Ungrid;                                                  // 0x02F0 (size: 0x8)
    bool Initialized;                                                                 // 0x02F8 (size: 0x1)
    FGuid ContainerID;                                                                // 0x02FC (size: 0x10)
    FGameplayTag ContainerType;                                                       // 0x030C (size: 0x8)
    FGameplayTag ContainerTag;                                                        // 0x0314 (size: 0x8)
    int32 NumberOfColumns;                                                            // 0x031C (size: 0x4)
    int32 NumberOfRows;                                                               // 0x0320 (size: 0x4)
    TArray<class UJSI_Slot_C*> WSlots;                                                // 0x0328 (size: 0x10)
    double OpacityValue;                                                              // 0x0338 (size: 0x8)
    int32 MaxEquippedStack;                                                           // 0x0340 (size: 0x4)
    FVector2D SlotSize;                                                               // 0x0348 (size: 0x10)
    FVector2D EquippedItemSlotSize;                                                   // 0x0358 (size: 0x10)
    bool EquippedImageMatchesContainerSize?;                                          // 0x0368 (size: 0x1)
    FMargin SlotPadding;                                                              // 0x036C (size: 0x10)
    FSlateColor SlotOnHoverColor;                                                     // 0x037C (size: 0x14)
    FSlateColor DragHighlightColor;                                                   // 0x0390 (size: 0x14)
    FSlateColor InvalidDragHighlightColor;                                            // 0x03A4 (size: 0x14)
    TArray<FRarityColors> RarityColors;                                               // 0x03B8 (size: 0x10)
    class UTexture2D* SlotImage;                                                      // 0x03C8 (size: 0x8)
    int32 CurrentSlotIndex;                                                           // 0x03D0 (size: 0x4)
    TArray<class UJSI_Slot_C*> HighlightedSlots;                                      // 0x03D8 (size: 0x10)
    bool CanDrop?;                                                                    // 0x03E8 (size: 0x1)
    int32 FromXLoc;                                                                   // 0x03EC (size: 0x4)
    int32 FromYLoc;                                                                   // 0x03F0 (size: 0x4)
    class UJSI_Slot_C* SlotRecH;                                                      // 0x03F8 (size: 0x8)
    FVector2D MouseLocH;                                                              // 0x0400 (size: 0x10)
    TArray<class UJSI_Slot_C*> ArrayOfItems;                                          // 0x0410 (size: 0x10)
    bool HideSlotImageWhenEquipped?;                                                  // 0x0420 (size: 0x1)
    bool RotateEquippedImage;                                                         // 0x0421 (size: 0x1)
    FGameplayTagContainer SupportedItemTypes;                                         // 0x0428 (size: 0x20)
    FJSIContainer_CItemsCombined ItemsCombined;                                       // 0x0448 (size: 0x10)
    void ItemsCombined(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotReceiver, class UJSI_Slot_C* SlotDropped, bool SlotDroppedRemoved?);
    FJSIContainer_CDropInfo_ContainerToContainer DropInfo_ContainerToContainer;       // 0x0458 (size: 0x10)
    void DropInfo_ContainerToContainer(class UJSIContainer_C* From, class UJSIContainer_C* To, class UJSI_Slot_C* ItemMovedRef, bool ToEmptySlot?, bool MovedAll?);
    FJSIContainer_CDropInfo_OnItemEquippedChange DropInfo_OnItemEquippedChange;       // 0x0468 (size: 0x10)
    void DropInfo_OnItemEquippedChange(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    FGameplayTagContainer NotSupportedItemTypes;                                      // 0x0478 (size: 0x20)
    TArray<class UJigsawItem_DataAsset_C*> OnlySupportTheseItems;                     // 0x0498 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> AlsoSupportTheseItems;                     // 0x04A8 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> DoNotSupportTheseItems;                    // 0x04B8 (size: 0x10)
    FKey RotateKey;                                                                   // 0x04C8 (size: 0x18)
    int32 DropToSlot;                                                                 // 0x04E0 (size: 0x4)
    FKey SplitKey;                                                                    // 0x04E8 (size: 0x18)
    FJSIContainer_CDropInfo_SplitPerformed DropInfo_SplitPerformed;                   // 0x0500 (size: 0x10)
    void DropInfo_SplitPerformed(class UJSI_Slot_C* SourceSlotRef, class UJSI_Slot_C* NewSplitSlotRef, class UJSIContainer_C* ContainerSource, class UJSIContainer_C* ContainerReceiver, int32 SplitAmount, int32 AmountRemaining);
    FJSIContainer_CDrop_ItemOverItem Drop_ItemOverItem;                               // 0x0510 (size: 0x10)
    void Drop_ItemOverItem(class UJSI_Slot_C* ItemReceiver, class UJSI_Slot_C* ItemDropped, class UJSIContainer_C* ItemReceiverContainer, class UJSIContainer_C* ItemDroppedContainer);
    EDragPivot DragBehaviour;                                                         // 0x0520 (size: 0x1)
    double MaxWeight;                                                                 // 0x0528 (size: 0x8)
    bool HideItemOnDrag?;                                                             // 0x0530 (size: 0x1)
    FText ContainerName;                                                              // 0x0538 (size: 0x18)
    FSlateFontInfo ContainerNameFont;                                                 // 0x0550 (size: 0x58)
    FMargin ContainerNamePadding;                                                     // 0x05A8 (size: 0x10)
    TArray<FDefaultItemInfo> AddDefaultItems;                                         // 0x05B8 (size: 0x10)
    bool AllowDrag;                                                                   // 0x05C8 (size: 0x1)
    bool AllowDragFromOtherContainers;                                                // 0x05C9 (size: 0x1)
    bool AllowMovingItemsOnDrop;                                                      // 0x05CA (size: 0x1)
    bool AllowOpeningContainers;                                                      // 0x05CB (size: 0x1)
    bool HandleContainerToContainerDrop;                                              // 0x05CC (size: 0x1)
    FJSIContainer_CDrop_ContainerOnContainerUnhandled Drop_ContainerOnContainerUnhandled; // 0x05D0 (size: 0x10)
    void Drop_ContainerOnContainerUnhandled(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* DroppedSlotRef, class UJSI_Slot_C* SlotReceiverRef, int32 ToSlotIndex, bool Rotated?);
    bool ReceiveAllUnhandledEvent;                                                    // 0x05E0 (size: 0x1)
    bool AllowVisualHighlight;                                                        // 0x05E1 (size: 0x1)
    bool ShowPrices;                                                                  // 0x05E2 (size: 0x1)
    FJSIContainer_COnSlotMouseButtonDown OnSlotMouseButtonDown;                       // 0x05E8 (size: 0x10)
    void OnSlotMouseButtonDown(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, FKey Button);
    FJSIContainer_COnSlotDoubleClick OnSlotDoubleClick;                               // 0x05F8 (size: 0x10)
    void OnSlotDoubleClick(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, FKey Button);
    bool AllowDropToGround;                                                           // 0x0608 (size: 0x1)
    FJSIContainer_COnItemSlotOrRotationChanged OnItemSlotOrRotationChanged;           // 0x0610 (size: 0x10)
    void OnItemSlotOrRotationChanged(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, int32 NewIndex, bool Rotated);
    class UJSI_Slot_C* SlotMother;                                                    // 0x0620 (size: 0x8)
    FJSIContainer_COnItemSplitRequest OnItemSplitRequest;                             // 0x0628 (size: 0x10)
    void OnItemSplitRequest(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* ItemSource, int32 ToSlotIndex, int32 TotalAmount, int32 SplitAmount, bool Rotated?);
    bool AutoPerformSplit;                                                            // 0x0638 (size: 0x1)
    FJSIContainer_COnItemStackRequest OnItemStackRequest;                             // 0x0640 (size: 0x10)
    void OnItemStackRequest(class UJSI_Slot_C* DroppedItem, class UJSI_Slot_C* ReceiverItem);
    bool AutoPerformStack;                                                            // 0x0650 (size: 0x1)
    FJSIContainer_COnStackCountChanged OnStackCountChanged;                           // 0x0658 (size: 0x10)
    void OnStackCountChanged(class UJSI_Slot_C* ItemRef, int32 NewCount);
    int32 ParentID;                                                                   // 0x0668 (size: 0x4)
    FGameplayTagContainer SortHighlightedTypes;                                       // 0x0670 (size: 0x20)
    bool AllowBlindAddItemToContainer?;                                               // 0x0690 (size: 0x1)
    bool IsPartSpecialContainer;                                                      // 0x0691 (size: 0x1)
    bool DropOnContainerSlot;                                                         // 0x0692 (size: 0x1)
    FJSIContainer_COnSlotMouseButtonUp OnSlotMouseButtonUp;                           // 0x0698 (size: 0x10)
    void OnSlotMouseButtonUp(class UJSIContainer_C* Container, class UJSI_Slot_C* ItemRef, FKey Button);
    class UJSIContainer_C* SpecialSContainer;                                         // 0x06A8 (size: 0x8)
    bool TriggerItemOverItemIfCannotAddToContainer;                                   // 0x06B0 (size: 0x1)
    bool IsSpecialLootContainer;                                                      // 0x06B1 (size: 0x1)
    class AActor* EquippedActorRef;                                                   // 0x06B8 (size: 0x8)
    bool Debug?;                                                                      // 0x06C0 (size: 0x1)
    bool AllowContextMenu;                                                            // 0x06C1 (size: 0x1)
    class UActorComponent* JigMultiplayerComp;                                        // 0x06C8 (size: 0x8)
    FGameplayTagContainer DoNotAllowContextMenuOptions;                               // 0x06D0 (size: 0x20)
    FJSIContainer_COnItemRemoved OnItemRemoved;                                       // 0x06F0 (size: 0x10)
    void OnItemRemoved(class UJSI_Slot_C* ItemRef, class UJSIContainer_C* FromContainer);
    bool IsActionbar;                                                                 // 0x0700 (size: 0x1)
    bool AllowStackingEquippedItem;                                                   // 0x0701 (size: 0x1)
    double ContainerWeight;                                                           // 0x0708 (size: 0x8)
    FJSIContainer_COnWeightUpdated OnWeightUpdated;                                   // 0x0710 (size: 0x10)
    void OnWeightUpdated(double NewWeight);
    bool ShowDurability;                                                              // 0x0720 (size: 0x1)
    bool OnImageCaptureTryAdjustScaleWhenEquipped;                                    // 0x0721 (size: 0x1)
    bool AllowCapturingItemImage;                                                     // 0x0722 (size: 0x1)
    FJSIContainer_COnCapacityUpdated OnCapacityUpdated;                               // 0x0728 (size: 0x10)
    void OnCapacityUpdated(int32 Current, int32 Max, class UJSIContainer_C* Container, class UJSI_Slot_C* SlotMother);
    class UJSIContainer_C* CanSwapWithJSIContainer;                                   // 0x0738 (size: 0x8)
    FJSIContainer_COnUniqueIDSet OnUniqueIDSet;                                       // 0x0740 (size: 0x10)
    void OnUniqueIDSet(class UJSI_Slot_C* ItemRef);
    bool VisualHighlighted?;                                                          // 0x0750 (size: 0x1)
    FSlateColor DragVisualHighlightColor;                                             // 0x0754 (size: 0x14)
    bool CallStackRequestOnContainerEmitter;                                          // 0x0768 (size: 0x1)
    bool OnOpenContainerInspectInstead;                                               // 0x0769 (size: 0x1)
    int32 CurrentControllerIndex;                                                     // 0x076C (size: 0x4)
    TArray<FKey> GampadDirectionalKeys;                                               // 0x0770 (size: 0x10)
    bool IsMainContainer?;                                                            // 0x0780 (size: 0x1)
    bool ControllerSupport;                                                           // 0x0781 (size: 0x1)
    bool ForceNoBlindAdd;                                                             // 0x0782 (size: 0x1)

    void GetLootContent(class UUserWidget*& Widget);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void GetAllAttachments(TArray<FName>& Attachments);
    void GetListOfNonAddContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetListOfContainers(TArray<class UJSIContainer_C*>& Containers);
    void DifficultyCheckBackpack?(class UJSI_Slot_C* SlotToCheck, bool& CarryOn?);
    bool IsInventory?();
    void GetLowestIndexItem(class UJSI_Slot_C*& HostingItem);
    bool IsWidgetReady?();
    bool HasItems();
    void GetColumnRowBySlotIndex_NonPure(int32 SlotIndex, int32& Column, int32& Row);
    void FindNearestToColumnRow(bool Reverse, bool CheckCol, double UP?, double Right?, bool Flip?, class UJSI_Slot_C*& Output_Get, int32& Index, bool& RetReverse, bool& RetCheckCol, double& RetUP?, double& RetRight?);
    void CheckKey(const FKeyEvent& Input, FKey B, bool& Value);
    void ControllerHighlightItem(int32 Index, bool Value);
    void GetContainersFromEquipped(TArray<class UJSIContainer_C*>& Container);
    FEventReply OnKeyUp(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    void On Controller Enter Leave(bool Value);
    bool IsContainerWithinHierarchy(class UJSIContainer_C* Container);
    void Deep_GettAllItemsByID(TArray<class UJSI_Slot_C*>& ArrByRef, class UJigsawItem_DataAsset_C* DA, bool IncludeEquippedItems?);
    void OnDropCheckStackability(class UJSI_Slot_C* FixedSlot, class UJSI_Slot_C* ItemDropped, class UJSI_Slot_C*& StackWith);
    void SetSlotDragVisualHighlight(bool Valid?);
    void HighlightNext(class UJSI_Slot_C* Current, class UJSI_Slot_C*& CurrentHighlighted);
    void GetContainerIndex(int32& Index);
    void Deep_GettAllItems(TArray<class UJSI_Slot_C*>& ArrByRef);
    void CheckWeightLimit(double ItemWeight, class UJSIContainer_C* FromContainer, bool& Result);
    void CheckLimitedEquipToStack(class UJSI_Slot_C* DroppedSlotRef, class UDragWidget_C* DragWidgetRef, class UJSI_Slot_C* FixedSlotRef, bool& Handled?);
    void GetItemsByStackAbility(FName ItemId, bool& Sucess, TArray<class UJSI_Slot_C*>& Items);
    void AdjustFromXY(FVector2D ItemVe);
    void Update_OnCountChanged(class UJSI_Slot_C* ItemRef, int32 OldCount, int32 NewCount);
    void Update_OnItemRemoved(class UJSI_Slot_C* ItemRef);
    void Update_OnItemAdded(class UJSI_Slot_C* ItemRef);
    void CheckIfCanStack(class UJSI_Slot_C* WithItemRef, bool& Sucess, class UJSI_Slot_C*& ItemRef);
    void ManageContainerName();
    void GetLastItem(class UJSI_Slot_C*& ItemRef);
    void CanAddItemToSlot_IgnoreItems(int32 ToSlot, FVector2D ItemVector, bool TryRotated?, TArray<class UJSI_Slot_C*>& IgnoreItems, bool& Result, bool& RotatedVector?);
    void TrySwapItems(class UJSI_Slot_C* Item1, class UJSI_Slot_C* Item2, bool& Success);
    void Deep Search for Item by ID(class UJigsawItem_DataAsset_C* ItemId, bool SearchInEquipTo?, class UJSI_Slot_C*& ItemRef, bool& Found?);
    bool DoesItemExist(class UJSI_Slot_C* ItemRef);
    void GetEmptySlotTryRotated_NonPure(FVector2D SlotVector, bool& Found?, int32& ToSlotIndex, bool& Rotated?);
    void GetHighestCapacityContainer(TArray<class UJSI_Slot_C*>& Items, class UJSI_Slot_C*& Output_Get);
    void CheckUnhandledStack(class UJSI_Slot_C* DropedSlotRef, class UJSI_Slot_C* FixedSlotRef, bool& Result);
    void CheckUnhandledSplit(class UJSI_Slot_C* DroppedSlotRef, class UDragWidget_C* DragWidgetRef, class UJSI_Slot_C* FixedSlotRef, bool DroppedRotated?, bool& Handled?);
    int32 GetFreeSpaceIfCapacityContainer();
    void DragSelectContainer(class UJSIContainer_C* MotherC, class UJSI_Slot_C* SlotReceiver, class UJSIContainer_C*& MotherRef);
    void GetInventoryItemsByIDList(TArray<class UJigsawItem_DataAsset_C*>& ItemId, bool& Found?, TArray<class UJSI_Slot_C*>& ItemsFound);
    void GetTotalCountOfItemType(FGameplayTag ItemType, int32& TotalItemCount);
    void GetTotalCountOfItemID(FName ItemId, int32& TotalItemCount);
    void SetLock Column Row(int32 ColumnToLock, int32 RowToLock, bool Lock?);
    void ExpandContainer(int32 AddColumns, int32 AddRows, bool& Success);
    void FindSlotByIndex(int32 Index, class UJSI_Slot_C*& SlotRef);
    void FindBestStackingItem(FName ItemId, int32 DroppedItemCount, class UJSI_Slot_C*& BestStackingItem, int32& FreeSpace, bool& CanStackAll);
    void GetTotalWeight(double& WeightRef);
    void ClearItemFromArr(class UJSI_Slot_C* SlotRef, class UJSIContainer_C* ToContainer?, bool TriggerUnequippedEvent?, bool& Removed);
    void IsSupportedByThis(class UJigsawItem_DataAsset_C* ItemId, const FGameplayTag& Type, bool& Supported?);
    void EquipItemFromJigRef(class UJSI_Slot_C* ItemRef, bool CallEquipEvent?, bool& Added, class UJSI_Slot_C*& EquippedItemRef);
    void CanAddItemToSlot(int32 ToSlot, FVector2D ItemVector, bool TryRotated?, bool& Result, bool& RotatedVector?);
    void GetHostingSlots(int32 SlotIndex, FVector2D ItemVec, TArray<int32>& Indexes);
    void AddItemFromJigRef(class UJSI_Slot_C* ItemRef, int32 ToSlot, bool RotateItem?, bool TryRotated?, bool& Added, class UJSI_Slot_C*& AddedItemRef);
    void GetMaxCapacity(int32 ItemMaxStack, int32& MaxCap);
    void GetCapacity(int32& Cap);
    void UpdateCapacityContainer();
    bool CheckIfCapacityContainer();
    void DummyFunction();
    void GetImageFromInfo(class UJigsawItem_DataAsset_C* Info, class UTexture*& Image);
    void CheckIfCanAddToSpecial(class UJSI_Slot_C* ItemSP, class UJSI_Slot_C* ItemToAdd, bool CheckBlind?, bool DigDeeper?, bool& CanAdd, class UJSIContainer_C*& ToContainer);
    void CheckRootContainer(class UJSI_Slot_C* DraggedItem, bool& Proceed?);
    bool CompareItems(class UJSI_Slot_C* Item1, class UJSI_Slot_C* Item2);
    void SetEquippedHighlight(bool Valid);
    bool IsSlotContainer();
    void GetEmptySlotTryRotated(FVector2D SlotVector, bool& Found?);
    bool IsEquipTo?();
    void HandleDragEnter(class UJSI_Slot_C* DraggedItem);
    void SetSlotHighlight(bool Valid?);
    void GetContainerHierarchy(TArray<class UJSIContainer_C*>& Containers);
    void SelectRotation(class UJSI_Slot_C* ItemRef, bool DroppedRotated?, bool& Rotated);
    bool SomeFunctionBool();
    void Re-SortHighlightItems();
    void RemoveSortHighlightedTypes(FGameplayTagContainer Types);
    void AddSortHighlightedTypes(const FGameplayTagContainer& Types);
    void HighlightItemsByType(FGameplayTagContainer Types);
    void GetRootContainerRef(bool ExcludeEquipTo, class UJSIContainer_C*& ContainerRef);
    void GetItemByUniqueID(FGuid UniqueID, bool& Found, class UJSI_Slot_C*& Item);
    void GetSlotMotherUID(bool ReturnContainerID_IfInvalid?, bool GetContainerIDIfSpecialC?, FGuid& UDID);
    void MoveItemToIndexBySlotIndex(int32 ItemSlotIndex, int32 ToIndex, bool Rotate?, bool& Moved?);
    void MoveItemToIndexByItemRef(class UJSI_Slot_C* ItemRef, int32 ToIndex, bool Rotate?, bool& Moved?);
    void GetColumnRowBySlotIndex(int32 SlotIndex, int32& Column, int32& Row);
    void GetItemBySlotIndex(int32 SlotIndex, bool& Found, class UJSI_Slot_C*& Item);
    void GetEquippedItemRef(bool& Found?, class UJSI_Slot_C*& ItemRef, bool& IsPending);
    void CanStackSingleSlot?(class UJigsawItem_DataAsset_C* Info, int32 Count, class UJSI_Slot_C* WithItem, bool& Sucess, class UJSI_Slot_C*& ItemRef);
    void UpdateCountByUniqueID(FGuid ID, int32 NewCount, bool& Sucess);
    void RemoveItemByUniqueID(FGuid ID, bool& Sucess);
    void UnequipItem(bool TriggerUnequippedEvent?, bool& Unequipped, class UJSI_Slot_C*& SlotRef);
    void EquipItem(class UJigsawItem_DataAsset_C* ItemId, int32 Count, class UJSIContainer_C* SlotContainer, bool TriggerEquippedEvent?, bool& Equipped, class UJSI_Slot_C*& SlotItemRef);
    bool IsEquipped?();
    void GetContainerName(FText& ContainerName);
    FVector2D CalculateSizeCustom(FVector2D SlotVector, FVector2D CSlotSize);
    void GetItemsByRarity(FGameplayTag Rarity, bool& Found?, TArray<class UJSI_Slot_C*>& ItemsFound);
    int32 GetItemsCount();
    void CalculateWeight(double& Weight);
    void ChangeItemRarity(class UJSI_Slot_C* ItemRef, FGameplayTag NewRarity, bool& Changed?);
    void GetInventoryItemsByID(class UJigsawItem_DataAsset_C* ItemId, bool& Found?, TArray<class UJSI_Slot_C*>& ItemsFound);
    void GetInventoryItemsByType(FGameplayTag Type, bool& Found?, TArray<class UJSI_Slot_C*>& ItemsFound);
    void ChangeItemCount(class UJSI_Slot_C* ItemRef, int32 NewCount, bool& Changed?);
    void ChangeItemImage(class UJSI_Slot_C* ItemRef, class UTexture2D* NewTexture, bool& Changed?);
    void ResizeItem(class UJSI_Slot_C* SlotRef, int32 AddToX, int32 AddToY, bool& Resized?);
    void CanResize?(class UJSI_Slot_C* SlotRef, int32 AddToX, int32 AddToY, bool& Result, TArray<class UJSI_Slot_C*>& HostingSlotsArray, TArray<class UJSI_Slot_C*>& FreedHostingSlotsArray);
    void CanStack?(class UJSIContainer_C* Container, class UJSI_Slot_C* ItemRef, bool& CanStack);
    void FlipSlotDim(FVector2D SlotVector, FVector2D& Flipped);
    void AddMultipleInventoryItems(TArray<class UJigsawItem_DataAsset_C*>& ItemIDs, TArray<class UJigsawItem_DataAsset_C*>& NoTAdded);
    void IsSupportedID?(class UJigsawItem_DataAsset_C* ID, bool& Supported);
    void IsSupportedType?(FGameplayTagContainer SupportedItemTypes, FGameplayTagContainer NotSupportedItemTypes, class UJigsawItem_DataAsset_C* ItemId, const FGameplayTag& Type, bool& Supported?);
    FEventReply OnKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    void ChangeItemsInteract(bool Interact?);
    void HandleContainerOnContainer(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, class UJSI_Slot_C* ToSlotRef, int32 ToSlot, bool DraggedRotated?, bool TryRotated?, bool& Handled?);
    void CombineItemRequest(class UJSIContainer_C* FromContainer, class UJSI_Slot_C* SlotRec, class UJSI_Slot_C* SlotDropped, bool TriggerEventDispatcher?);
    FVector2D CalculateSize(FVector2D SlotVector);
    void ClearContainer();
    void AddNewInventoryItem_CustomVector(class UJigsawItem_DataAsset_C* ItemId, int32 Count, int32 ToSlot, bool& Added, int32& SlotIndex, class UJigsawItem_DataAsset_C*& ItemInfo, class UJSI_Slot_C*& SlotItemRef);
    void RemoveInventoryItemByRef(class UJSI_Slot_C* SlotRef, bool TriggerUnequippedEvent?, bool& Removed);
    int32 GetLength();
    void AddNewInventoryItem(class UJigsawItem_DataAsset_C* ItemDA, int32 Count, int32 ToSlot, bool Rotated?, bool TryRotated?, bool TryToStack?, bool TriggerEquippedEvent?, bool& Added, int32& SlotIndex, class UJigsawItem_DataAsset_C*& ItemInfo, class UJSI_Slot_C*& SlotItemRef, bool& Stacked?);
    bool OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, class UDragDropOperation* Operation);
    void GetEmptySlot(FVector2D SlotVector, int32& SlotIndex, bool& Found?);
    void CancelHighlights();
    void HighlightSlots(int32 SlotIndex, FVector2D SlotVector, FVector2D MouseLoc, class UJSI_Slot_C* SlotRef, class UJSI_Slot_C* SlotRecRef, bool Rotated?, bool Supported?, bool DroppedWithinSelf?);
    void Hightlight_DragMove(int32 SlotIndex, class UJSI_Slot_C* DragSlotRef, class UJSI_Slot_C* SlotRec, bool Rotated?, bool DroppedWithinSelf?);
    int32 Get Slot Index by Column Row(int32 Column, int32 Row);
    void PerfromDrop(int32 SlotIndex, FVector2D SlotVector, class UJSI_Slot_C* SlotRef, bool Rotated?, bool& Moved);
    void GetPaddingBySlotIndex(int32 SlotIndex, FVector2D SlotVector, FMargin& Padding, int32& Conumn, int32& Row);
    void GetSlotIndex(int32& Result);
    void AddNewItem(int32 ToSlot, class UTexture* Image, FVector2D SlotVector, FGameplayTag Rarity, class UJigsawItem_DataAsset_C* ItemInfo, bool SetVector, int32 Count, bool& Added, int32& AddedToSlot, class UJigsawItem_DataAsset_C*& Info, class UJSI_Slot_C*& SlotItemRef);
    void SetItemReference(class UJSI_Slot_C* ItemRef);
    void SetActorOwner(class AActor* ActorRef);
    void JSICheckStatus();
    void JSIOnWeightUpdated(double NewWeight);
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void ForceInitSpecialcontainer();
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void OnCreatedFromUtility();
    void PreConstruct(bool IsDesignTime);
    void Initialize(bool Design?);
    void Destruct();
    void Re-Init(int32 Columns, int32 Rows);
    void Call_ContainerOnContainer(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, class UJSI_Slot_C* ToSlotRef, int32 ToSlot, bool DragRotated?, bool TryRotated?);
    void OnDragLeave(FPointerEvent PointerEvent, class UDragDropOperation* Operation);
    void OnDragEnter(FGeometry MyGeometry, FPointerEvent PointerEvent, class UDragDropOperation* Operation);
    void ClearData();
    void Add_DefaultItems();
    void OnInitialized();
    void ProcessControllerStick();
    void ExecuteUbergraph_JSIContainer(int32 EntryPoint);
    void OnUniqueIDSet__DelegateSignature(class UJSI_Slot_C* ItemRef);
    void OnCapacityUpdated__DelegateSignature(int32 Current, int32 Max, class UJSIContainer_C* Container, class UJSI_Slot_C* SlotMother);
    void OnWeightUpdated__DelegateSignature(double NewWeight);
    void OnItemRemoved__DelegateSignature(class UJSI_Slot_C* ItemRef, class UJSIContainer_C* FromContainer);
    void OnSlotMouseButtonUp__DelegateSignature(class UJSIContainer_C* Container, class UJSI_Slot_C* ItemRef, FKey Button);
    void OnStackCountChanged__DelegateSignature(class UJSI_Slot_C* ItemRef, int32 NewCount);
    void OnItemStackRequest__DelegateSignature(class UJSI_Slot_C* DroppedItem, class UJSI_Slot_C* ReceiverItem);
    void OnItemSplitRequest__DelegateSignature(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* ItemSource, int32 ToSlotIndex, int32 TotalAmount, int32 SplitAmount, bool Rotated?);
    void OnItemSlotOrRotationChanged__DelegateSignature(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, int32 NewIndex, bool Rotated);
    void OnSlotDoubleClick__DelegateSignature(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, FKey Button);
    void OnSlotMouseButtonDown__DelegateSignature(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, FKey Button);
    void Drop_ContainerOnContainerUnhandled__DelegateSignature(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* DroppedSlotRef, class UJSI_Slot_C* SlotReceiverRef, int32 ToSlotIndex, bool Rotated?);
    void Drop_ItemOverItem__DelegateSignature(class UJSI_Slot_C* ItemReceiver, class UJSI_Slot_C* ItemDropped, class UJSIContainer_C* ItemReceiverContainer, class UJSIContainer_C* ItemDroppedContainer);
    void DropInfo_SplitPerformed__DelegateSignature(class UJSI_Slot_C* SourceSlotRef, class UJSI_Slot_C* NewSplitSlotRef, class UJSIContainer_C* ContainerSource, class UJSIContainer_C* ContainerReceiver, int32 SplitAmount, int32 AmountRemaining);
    void DropInfo_OnItemEquippedChange__DelegateSignature(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void DropInfo_ContainerToContainer__DelegateSignature(class UJSIContainer_C* From, class UJSIContainer_C* To, class UJSI_Slot_C* ItemMovedRef, bool ToEmptySlot?, bool MovedAll?);
    void ItemsCombined__DelegateSignature(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotReceiver, class UJSI_Slot_C* SlotDropped, bool SlotDroppedRemoved?);
}; // Size: 0x783

#endif

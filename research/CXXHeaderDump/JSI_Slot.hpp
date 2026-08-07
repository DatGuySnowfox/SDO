#ifndef UE4SS_SDK_JSI_Slot_HPP
#define UE4SS_SDK_JSI_Slot_HPP

class UJSI_Slot_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* FadeInOut;                                                // 0x02C8 (size: 0x8)
    class UTextBlock* Durability;                                                     // 0x02D0 (size: 0x8)
    class UOverlay* MainSlotOverlay;                                                  // 0x02D8 (size: 0x8)
    class UTextBlock* ObjItemName;                                                    // 0x02E0 (size: 0x8)
    class UTextBlock* PriceCount;                                                     // 0x02E8 (size: 0x8)
    class UScaleBox* ScaleBox_133;                                                    // 0x02F0 (size: 0x8)
    class USizeBox* SizeBox;                                                          // 0x02F8 (size: 0x8)
    class UBorder* SlotBorder;                                                        // 0x0300 (size: 0x8)
    class UImage* SlotFrameImg;                                                       // 0x0308 (size: 0x8)
    class UImage* SlotHighlight;                                                      // 0x0310 (size: 0x8)
    class UImage* SlotImage;                                                          // 0x0318 (size: 0x8)
    class UTextBlock* StockCount;                                                     // 0x0320 (size: 0x8)
    class UTextBlock* TextCout;                                                       // 0x0328 (size: 0x8)
    class UVerticalBox* VerticalBox_0;                                                // 0x0330 (size: 0x8)
    int32 SlotIndex;                                                                  // 0x0338 (size: 0x4)
    FVector2D Size;                                                                   // 0x0340 (size: 0x10)
    FSlateColor OnHoverTint;                                                          // 0x0350 (size: 0x14)
    class UTexture* Image;                                                            // 0x0368 (size: 0x8)
    bool Highlighted?;                                                                // 0x0370 (size: 0x1)
    class UJSIContainer_C* ContainerMother;                                           // 0x0378 (size: 0x8)
    FKey RotateKey;                                                                   // 0x0380 (size: 0x18)
    bool IsFixedSlot?;                                                                // 0x0398 (size: 0x1)
    int32 ParentID;                                                                   // 0x039C (size: 0x4)
    class UContainerWindowWidget_C* WindowContainer;                                  // 0x03A0 (size: 0x8)
    bool IsLocked?;                                                                   // 0x03A8 (size: 0x1)
    class UTexture2D* FrameImageRect;                                                 // 0x03B0 (size: 0x8)
    double OpacityValue;                                                              // 0x03B8 (size: 0x8)
    bool DragHighlighted?;                                                            // 0x03C0 (size: 0x1)
    class UOnHoverTooltipWidget_C* OnHoverWidget;                                     // 0x03C8 (size: 0x8)
    bool SortHighlighted?;                                                            // 0x03D0 (size: 0x1)
    bool ShouldUpdateCapacity?;                                                       // 0x03D1 (size: 0x1)
    bool HideOnDrag;                                                                  // 0x03D2 (size: 0x1)
    bool IsPending;                                                                   // 0x03D3 (size: 0x1)
    class UJSI_Slot_C* HostedItem;                                                    // 0x03D8 (size: 0x8)
    TArray<class UJSI_Slot_C*> ArrayOfHostingItem;                                    // 0x03E0 (size: 0x10)
    class UJSIContainer_C* SlotContainer;                                             // 0x03F0 (size: 0x8)
    FLinearColor LastRarityColor;                                                     // 0x03F8 (size: 0x10)
    class UTextureRenderTarget2D* RenderTargetSnap;                                   // 0x0408 (size: 0x8)
    bool MouseEnter?;                                                                 // 0x0410 (size: 0x1)
    class UBP_InspectorCanvas_C* CanvasBP;                                            // 0x0418 (size: 0x8)
    bool ShouldRestoreImage;                                                          // 0x0420 (size: 0x1)
    TArray<FName> PendingSnapAttachements;                                            // 0x0428 (size: 0x10)
    class UBP_ItemSnapLoading_C* LoadingSnap;                                         // 0x0438 (size: 0x8)
    class UUserWidget* InspectorWidget;                                               // 0x0440 (size: 0x8)
    bool PendingSnap;                                                                 // 0x0448 (size: 0x1)
    FVector2D OriginalSize;                                                           // 0x0450 (size: 0x10)
    int32 InitCapacity;                                                               // 0x0460 (size: 0x4)
    class UTexture2D* FrameImage;                                                     // 0x0468 (size: 0x8)
    class UJigsawItem_DataAsset_C* JigDataAsset;                                      // 0x0470 (size: 0x8)
    FGameplayTag Rarity;                                                              // 0x0478 (size: 0x8)
    bool SlotRotated?;                                                                // 0x0480 (size: 0x1)
    EDragPivot DragBehaviour;                                                         // 0x0481 (size: 0x1)
    bool SnapIsOnStandby;                                                             // 0x0482 (size: 0x1)
    class UJSI_Slot_C* MirroredActionbar;                                             // 0x0488 (size: 0x8)
    double ItemWeight;                                                                // 0x0490 (size: 0x8)
    FVector2D SlotVector;                                                             // 0x0498 (size: 0x10)
    FVector2D DragOffset;                                                             // 0x04A8 (size: 0x10)
    class UOverlaySlot* OverlayRef;                                                   // 0x04B8 (size: 0x8)
    FVector2D LastClickLoc;                                                           // 0x04C0 (size: 0x10)
    bool DragDisabled?;                                                               // 0x04D0 (size: 0x1)
    bool IsEmpty;                                                                     // 0x04D1 (size: 0x1)
    int32 Row;                                                                        // 0x04D4 (size: 0x4)
    int32 Column;                                                                     // 0x04D8 (size: 0x4)
    bool HighlightOnHover?;                                                           // 0x04DC (size: 0x1)
    FGameplayTagContainer DropSupportedItems;                                         // 0x04E0 (size: 0x20)
    bool AllowInteractionOnSort;                                                      // 0x0500 (size: 0x1)
    double SortHighlightOpacity;                                                      // 0x0508 (size: 0x8)
    FJSI_Slot_COnItemDestructEvent OnItemDestructEvent;                               // 0x0510 (size: 0x10)
    void OnItemDestructEvent();
    FGuid ItemUniqueID;                                                               // 0x0520 (size: 0x10)
    int32 ItemCount;                                                                  // 0x0530 (size: 0x4)
    FVector2D ItemDim;                                                                // 0x0538 (size: 0x10)
    bool ItemRotated?;                                                                // 0x0548 (size: 0x1)
    class UUserWidget* SpecialContainerRef;                                           // 0x0550 (size: 0x8)
    double Price;                                                                     // 0x0558 (size: 0x8)
    double Dur;                                                                       // 0x0560 (size: 0x8)
    double MaxDur;                                                                    // 0x0568 (size: 0x8)
    class UTexture* InvTexture;                                                       // 0x0570 (size: 0x8)
    class UTexture* EquipTexture;                                                     // 0x0578 (size: 0x8)
    TArray<FS_ItemStat> ItemStats;                                                    // 0x0580 (size: 0x10)
    class UBP_PendingTimer_C* PendingTimer;                                           // 0x0590 (size: 0x8)
    bool CTRL_Down;                                                                   // 0x0598 (size: 0x1)
    bool SHIFT_Down;                                                                  // 0x0599 (size: 0x1)
    bool ALT_Down;                                                                    // 0x059A (size: 0x1)
    TMap<class FString, class FString> CustomData;                                    // 0x05A0 (size: 0x50)

    void GetCurrency(FText& Currency);
    void GetPrice(double& Price);
    void UpdateCustom Data(TArray<FString>& Key, TArray<FString>& Value);
    void UpdateMaterialRarity();
    void GetSlotDimUnrotated(FVector2D& SlotDimension);
    void OnItemMoved();
    void IsALTDown(bool& CTRL_Down);
    void IsSHIFTDown(bool& CTRL_Down);
    void IsCTRLDown(bool& CTRL_Down);
    FEventReply ProcessMouseDown(FGeometry& Geo, FPointerEvent& Pointer);
    FEventReply OnFocusReceived(FGeometry MyGeometry, FFocusEvent InFocusEvent);
    void GetContainers(TArray<class UJSIContainer_C*>& Container);
    void SetControllerHighlight(bool Value);
    void GetJigDA(class UJigsawItem_DataAsset_C*& JigDataAsset);
    void GetMaxStack(bool CheckCapacityContainer?, int32& MaxStack);
    void SetItemDim(FVector2D ItemDim);
    void SetDynamicInfo(class UJigsawItem_DataAsset_C* JigDataAsset);
    void GetPendingAttachments(TArray<FName>& PendingSnapAttachements);
    void SetPendingAttachments(TArray<FName>& PendingSnapAttachements);
    void GetStatByName(FGameplayTag STAT_TAG, bool& Found, FS_ItemStat& Stat);
    void ShowHideDisplayName(bool Hide?);
    void UpdateStats(TArray<FS_ItemStat>& Stats);
    void GetDurability(double& Durability, double& MaxDurability);
    void GetContainerCapacity(int32& Cap, int32& MaxCapacity);
    void UpdateImage(class UTexture* NewTexture, bool SetInInfo?, bool& Updated);
    void DeepSearch_GetAllItems(TArray<class UJSI_Slot_C*>& ArrItemsByRef);
    bool AbleToStackTo?(class UJSI_Slot_C* ToItem);
    void GetUniqueID(FGuid& UniqueServerID);
    void CanStack?(bool& CanStack, bool& FreeSpaceAvailable);
    void GetStats(TArray<FS_ItemStat>& ItemStats);
    void GetSpecialContainerRef(class UUserWidget*& SpecialContainerRef);
    void OnVisualDragHighlight(bool Value);
    void CapacityContainer_GetLastItem(class UJSI_Slot_C*& Output);
    int32 GetFreeSpace();
    bool IsSpecialContainer?();
    void OptimizeFixedSlot();
    void SetJigBrushSize(class UImage* Target, FVector2D Image Size);
    void SetStatByName(FGameplayTag STAT_TAG, double MinVal, double MaxVal, bool& Found, FS_ItemStat& Stat);
    void IsSnapOnStandBy(bool& SnapIsOnStandby);
    void GetSlotSize(FVector2D& Size);
    void UpdateItemSnap(bool IsActionbar?);
    void GetSlotDim(FVector2D& SlotDimension);
    void RemoveCount(int32 RemoveNum, bool RemoveAllIfCount<Num?, bool CallOnStackChanged?, bool& Removed?, int32& NewCount);
    void CheckIfScaleIsNeeded();
    void ClearImageScale();
    void ShowWeaponMagSize(int32 CurrentCap, int32 MaxCap);
    void ClearPendingAttachments();
    void SetCapacity(int32 CurrentCap, bool Unknown?);
    void GetItemID(FName& ItemId);
    void AdjustImageScale();
    void Update_OnItemAdded(class UJSI_Slot_C* ItemRef);
    void SetSlotImageVisibility(ESlateVisibility InVisibility);
    void TryUseBullet(bool& Return, FGuid& UID, class UJSI_Slot_C*& ItemUsed);
    void SetSlotContainerRef(class UJSIContainer_C* SlotContainer, class UJSI_Slot_C*& Output_Get);
    void SetSnapOnStandBy(bool SnapIsOnStandby);
    void Rotate Image Default(bool Rotated?);
    void SetMaxCount(bool CallOnStackChanged?, int32& Count);
    void RotateEquippedImage();
    void SetSnapImagePending(bool PendingInspect);
    void AdaptImageToActionbar(class UTextureRenderTarget2D* RenderTargetSnap);
    void GetLenOfItemsInContainer(int32& Output_Get);
    bool HasPendingAttachments();
    void Update_OnCountChanged(class UJSI_Slot_C* ItemRef, int32 OldCount, int32 NewCount);
    void UpdateCapacity(FText UpdateCountText);
    void UpdateDurability(FVector2D NewDur);
    void Update_OnItemRemoved(class UJSI_Slot_C* ItemRef);
    void GetDefaultWeight(double& Weight);
    void IsPendingSnap?(bool& PendingSnap);
    void UpdateStockCount(int32 NewCount);
    void GetJigItemImage(class UTexture*& TextureRef);
    void GetCSlotFromHosted(class UJSIContainer_C*& SlotContainer);
    double GetWeight();
    void LockSlot();
    void UnlockSlot();
    void HideCapacity();
    void IsItemPending?(bool& IsPending);
    void GetContainerRef(class UUserWidget*& SpecialContainerRef, class UJSIContainer_C*& SlotContainer, bool& IsSpecialContainer);
    void RemoveFromJSIParent(bool& Removed);
    bool ISCapacityContainer();
    void UnhandledGetTargetContainer(class UJSIContainer_C* Container1, class UJSIContainer_C* Container2, class UJSIContainer_C*& Target);
    void CheckRootContainer(class UJSI_Slot_C* DraggedItem, bool& CanAdd?);
    void SetSpecialContainer(class UUserWidget* SpecialContainerRef);
    void GetContainerOneRef(class UUserWidget*& ContainerRef);
    void CanOpenSpecialContainer?(bool& Return value);
    void SetSortHighlight(bool IsFaded, bool AllowInteractionOnSort);
    void GetType(FGameplayTag& Type);
    void AddToXY(int32 X, int32 Y, FVector2D& SlotDimension);
    void DetectChange(class UJSI_Slot_C* DroppedSlotRef, bool DraggedRotated?, bool& NOChange);
    void GetClickLocation(const FGeometry& Geometry, const FPointerEvent& MouseEvents, FVector2D& Output_Get);
    void HandleSizeBox();
    void IsStillValid(class UJSI_Slot_C* Slot, bool& Valid);
    void IsContainer?(bool& IsContainer?);
    void SetUniqueID(FGuid ID, FGuid& UniqueID);
    void GetItemName(FText& Name);
    void ChangePrice(double Price);
    void ChangeRarity(FGameplayTag Rarity, bool& Changed?);
    void ResizeSlot(FVector2D NewSize, bool ResizeImage?, FVector2D& Size);
    class UWidget* Get_SlotBorder_ToolTipWidget_0();
    FEventReply On_MouseDoubleClick(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    bool isRotated?();
    void GetCount(int32& Count);
    void UpdateCount(int32 NewCount, bool CallOnStackChanged?, bool SkipIfCapContainer, int32& Count);
    void GetSlotIndexOnDrop(int32& SlotIndex);
    void GetBorderColorByRarity(FGameplayTag Rarity, FLinearColor& Color);
    void SetHighlight(bool Highlight?, FSlateColor HighlightColor, bool HideImage?, bool& Highlight);
    void SetOverlaySlot(class UOverlaySlot* OverlayRef);
    void SetHostingArray(TArray<class UJSI_Slot_C*>& ArrayOfHostingItem);
    void GetHostedSlot(class UJSI_Slot_C*& HostingItem);
    void SetHostedSlot(class UJSI_Slot_C* HostingItem);
    FEventReply OnMouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    bool OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, class UDragDropOperation* Operation);
    void OnDragDetected(FGeometry MyGeometry, const FPointerEvent& PointerEvent, class UDragDropOperation*& Operation);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void TogleHighlight();
    void IsEmptySlot(bool& IsEmpty);
    void ClearSlot();
    void UpdateInfo(class UObject* Image, FVector2D Image Size);
    void Initialize();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void OnDragEnter(FGeometry MyGeometry, FPointerEvent PointerEvent, class UDragDropOperation* Operation);
    void OnDragCancelled(const FPointerEvent& PointerEvent, class UDragDropOperation* Operation);
    void PreConstruct(bool IsDesignTime);
    void RefreshOnDragEnter(class UDragDropOperation* Op);
    void RotateSlot(bool Rotate?, bool FlipVector?);
    void ConstructInitialize();
    void Destruct();
    void SetPending(bool Pending?, double PlaybackSpeed, double TimerInSec);
    void SetRarityColor();
    void Construct();
    void ResetOpacity();
    void OnDone_Event_0();
    void ExecuteUbergraph_JSI_Slot(int32 EntryPoint);
    void OnItemDestructEvent__DelegateSignature();
}; // Size: 0x5F0

#endif

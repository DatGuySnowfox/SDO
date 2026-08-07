#ifndef UE4SS_SDK_ContainerWindowWidget_HPP
#define UE4SS_SDK_ContainerWindowWidget_HPP

class UContainerWindowWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* CloseBtn;                                                          // 0x02C8 (size: 0x8)
    class UTextBlock* ContainerName;                                                  // 0x02D0 (size: 0x8)
    class UHorizontalBox* HorizontalBox_1;                                            // 0x02D8 (size: 0x8)
    class UImage* Image_115;                                                          // 0x02E0 (size: 0x8)
    class UBorder* MainBorder;                                                        // 0x02E8 (size: 0x8)
    class UBorder* TitleBorder;                                                       // 0x02F0 (size: 0x8)
    class UVerticalBox* VBox;                                                         // 0x02F8 (size: 0x8)
    class UTextBlock* Weight;                                                         // 0x0300 (size: 0x8)
    FText CName;                                                                      // 0x0308 (size: 0x18)
    bool MouseBtnDown;                                                                // 0x0320 (size: 0x1)
    FVector2D LastBtnPressLoc;                                                        // 0x0328 (size: 0x10)
    FVector2D GeoClick;                                                               // 0x0338 (size: 0x10)
    FKey KeyToClose;                                                                  // 0x0348 (size: 0x18)
    class UJSI_Slot_C* ItemRef;                                                       // 0x0360 (size: 0x8)
    FKey NoneKey;                                                                     // 0x0368 (size: 0x18)
    class UCanvasPanelSlot* CanvasSlot;                                               // 0x0380 (size: 0x8)
    class UBP_InspectorCanvas_C* CanvasBPRef;                                         // 0x0388 (size: 0x8)
    double TotalWeight;                                                               // 0x0390 (size: 0x8)
    FVector2D DefaultPos;                                                             // 0x0398 (size: 0x10)

    void GetLootContent(class UUserWidget*& Widget);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void GetAllAttachments(TArray<FName>& Attachments);
    void GetListOfNonAddContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetListOfContainers(TArray<class UJSIContainer_C*>& Containers);
    FVector2D GetPosInViewport();
    FVector2D GetPos(class UWidget* Target);
    void GetWeight();
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseMove(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply On_TitleBorder_MouseButtonDown_0(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void JSIOnWeightUpdated(double NewWeight);
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void ForceInitSpecialcontainer();
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void OnCreatedFromUtility();
    void BndEvt__CloseBtn_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void JSICheckStatus();
    void SetItemReference(class UJSI_Slot_C* ItemRef);
    void SetActorOwner(class AActor* ActorRef);
    void SetContainerWindowLocation();
    void ExecuteUbergraph_ContainerWindowWidget(int32 EntryPoint);
}; // Size: 0x3A8

#endif

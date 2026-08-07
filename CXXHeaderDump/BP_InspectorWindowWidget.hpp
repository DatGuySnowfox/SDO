#ifndef UE4SS_SDK_BP_InspectorWindowWidget_HPP
#define UE4SS_SDK_BP_InspectorWindowWidget_HPP

class UBP_InspectorWindowWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBP_InspectorHelper_C* BP_InspectorHelper;                                  // 0x02C8 (size: 0x8)
    class UButton* CloseBtn;                                                          // 0x02D0 (size: 0x8)
    class UHorizontalBox* HorizontalBox_1;                                            // 0x02D8 (size: 0x8)
    class UImage* Image_115;                                                          // 0x02E0 (size: 0x8)
    class UTextBlock* InfoTxt;                                                        // 0x02E8 (size: 0x8)
    class UImage* InspectImage;                                                       // 0x02F0 (size: 0x8)
    class UTextBlock* ItemName;                                                       // 0x02F8 (size: 0x8)
    class USizeBox* MainSizeBox;                                                      // 0x0300 (size: 0x8)
    class UVerticalBox* MainVB;                                                       // 0x0308 (size: 0x8)
    class UTextBlock* PriceText;                                                      // 0x0310 (size: 0x8)
    class UBorder* SP_Container;                                                      // 0x0318 (size: 0x8)
    class UVerticalBox* StatsVB;                                                      // 0x0320 (size: 0x8)
    class UUniformGridPanel* TextStatsGrid;                                           // 0x0328 (size: 0x8)
    class UBorder* TitleBorder;                                                       // 0x0330 (size: 0x8)
    class UBorder* TxtBorder;                                                         // 0x0338 (size: 0x8)
    class UTextBlock* Weight;                                                         // 0x0340 (size: 0x8)
    class ABP_ItemInspector_C* ItemInspectorActor;                                    // 0x0348 (size: 0x8)
    bool BtnItemMouseDown;                                                            // 0x0350 (size: 0x1)
    FVector2D CurrentMousePos;                                                        // 0x0358 (size: 0x10)
    bool TitleBtnMouseDown;                                                           // 0x0368 (size: 0x1)
    class UBP_InspectorCanvas_C* Inspector Canvas;                                    // 0x0370 (size: 0x8)
    double TotalWeight;                                                               // 0x0378 (size: 0x8)
    FBP_InspectorWindowWidget_COnClosed OnClosed;                                     // 0x0380 (size: 0x10)
    void OnClosed();
    int32 GridRow;                                                                    // 0x0390 (size: 0x4)
    int32 GridColumn;                                                                 // 0x0394 (size: 0x4)

    FText Get_PriceText_Text();
    void GetPrice(double& Price, double& ExtraPrice);
    void SetWeight();
    void ManageInspectorActiveState();
    FEventReply On_TitleBorder_MouseButtonDown_0(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseWheel(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseMove(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply On_InspectImage_MouseButtonDown_0(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseButtonDoubleClick(FGeometry InMyGeometry, const FPointerEvent& InMouseEvent);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void SetInitPosition();
    void Destruct();
    void BndEvt__CloseBtn_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void OnInitialized();
    void RefreshStats();
    void ExecuteUbergraph_BP_InspectorWindowWidget(int32 EntryPoint);
    void OnClosed__DelegateSignature();
}; // Size: 0x398

#endif

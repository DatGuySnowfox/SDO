#ifndef UE4SS_SDK_BP_CharPreview_HPP
#define UE4SS_SDK_BP_CharPreview_HPP

class UBP_CharPreview_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBP_EquipToContent_C* BP_EquipToContent;                                    // 0x02C8 (size: 0x8)
    class UBorder* MainBord;                                                          // 0x02D0 (size: 0x8)
    class UImage* MainImage;                                                          // 0x02D8 (size: 0x8)
    FVector2D CurrentMousePos;                                                        // 0x02E0 (size: 0x10)
    class AActor* PrevCharBP;                                                         // 0x02F0 (size: 0x8)
    bool BtnItemMouseDown;                                                            // 0x02F8 (size: 0x1)
    class UTextureRenderTarget2D* RenderTargetToUse;                                  // 0x0300 (size: 0x8)

    void CreateNewRenderTarget(class UTextureRenderTarget2D*& Output_Get);
    FEventReply OnMouseWheel(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseMove(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnInitialized();
    void OnShown_Event_0();
    void OnCollapsed_Event_0();
    void ExecuteUbergraph_BP_CharPreview(int32 EntryPoint);
}; // Size: 0x308

#endif

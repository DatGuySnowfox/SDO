#ifndef UE4SS_SDK_SaveGameSlot_HPP
#define UE4SS_SDK_SaveGameSlot_HPP

class USaveGameSlot_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x0348 (size: 0x8)
    class USaveGameThumbnail_C* SaveGameThumbnail;                                    // 0x0350 (size: 0x8)
    class UTextBlock* IsCurrentSaveText;                                              // 0x0358 (size: 0x8)
    class UImage* Image_112;                                                          // 0x0360 (size: 0x8)
    class UButton* DeleteSaveButton;                                                  // 0x0368 (size: 0x8)
    class UImage* BG;                                                                 // 0x0370 (size: 0x8)
    FString SaveName;                                                                 // 0x0378 (size: 0x10)
    bool IsSelected;                                                                  // 0x0388 (size: 0x1)
    FSaveGameSlot_CSetSelected SetSelected;                                           // 0x0390 (size: 0x10)
    void SetSelected(class USaveGameSlot_C* SaveSlot);
    FLinearColor Color;                                                               // 0x03A0 (size: 0x10)
    FSaveGameSlot_CDeleteSaveSelected DeleteSaveSelected;                             // 0x03B0 (size: 0x10)
    void DeleteSaveSelected(class USaveGameSlot_C* SaveSlot);

    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void Deselect();
    void Select(bool AutoSelect);
    void PreConstruct(bool IsDesignTime);
    void Construct();
    void SetCurrent();
    void BndEvt__SaveMenu_DeleteSaveButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_SaveGameSlot(int32 EntryPoint);
    void DeleteSaveSelected__DelegateSignature(class USaveGameSlot_C* SaveSlot);
    void SetSelected__DelegateSignature(class USaveGameSlot_C* SaveSlot);
}; // Size: 0x3C0

#endif

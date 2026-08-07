#ifndef UE4SS_SDK_SaveGameSlot_HPP
#define UE4SS_SDK_SaveGameSlot_HPP

class USaveGameSlot_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* BG;                                                                 // 0x02C8 (size: 0x8)
    class UTextBlock* IsCurrentSave?;                                                 // 0x02D0 (size: 0x8)
    class USaveGameThumbnail_C* SaveGameThumbnail;                                    // 0x02D8 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x02E0 (size: 0x8)
    FString SaveName;                                                                 // 0x02E8 (size: 0x10)
    bool IsSelected;                                                                  // 0x02F8 (size: 0x1)
    FSaveGameSlot_CSetSelected SetSelected;                                           // 0x0300 (size: 0x10)
    void SetSelected(class USaveGameSlot_C* SaveSlot);
    FLinearColor Color;                                                               // 0x0310 (size: 0x10)

    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void Deselect();
    void Select(bool AutoSelect);
    void PreConstruct(bool IsDesignTime);
    void Construct();
    void SetCurrent();
    void ExecuteUbergraph_SaveGameSlot(int32 EntryPoint);
    void SetSelected__DelegateSignature(class USaveGameSlot_C* SaveSlot);
}; // Size: 0x320

#endif

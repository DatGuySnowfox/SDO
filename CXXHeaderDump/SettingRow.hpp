#ifndef UE4SS_SDK_SettingRow_HPP
#define UE4SS_SDK_SettingRow_HPP

class USettingRow_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* AlternatingRowBorder;                                              // 0x02C8 (size: 0x8)
    class USizeBox* ContentSize;                                                      // 0x02D0 (size: 0x8)
    class UTextBlock* LabelText;                                                      // 0x02D8 (size: 0x8)
    class UNamedSlot* SettingContent;                                                 // 0x02E0 (size: 0x8)
    FText Label;                                                                      // 0x02E8 (size: 0x18)
    double MinDesiredWidth;                                                           // 0x0300 (size: 0x8)
    int32 Index;                                                                      // 0x0308 (size: 0x4)

    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_SettingRow(int32 EntryPoint);
}; // Size: 0x30C

#endif

#ifndef UE4SS_SDK_SettingRow_HPP
#define UE4SS_SDK_SettingRow_HPP

class USettingRow_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UNamedSlot* SettingContent;                                                 // 0x0348 (size: 0x8)
    class UTextBlock* LabelText;                                                      // 0x0350 (size: 0x8)
    class USizeBox* ContentSize;                                                      // 0x0358 (size: 0x8)
    FText Label;                                                                      // 0x0360 (size: 0x10)
    double MinDesiredWidth;                                                           // 0x0370 (size: 0x8)
    int32 Index;                                                                      // 0x0378 (size: 0x4)

    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_SettingRow(int32 EntryPoint);
}; // Size: 0x37C

#endif

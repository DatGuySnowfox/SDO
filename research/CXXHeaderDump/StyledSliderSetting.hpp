#ifndef UE4SS_SDK_StyledSliderSetting_HPP
#define UE4SS_SDK_StyledSliderSetting_HPP

class UStyledSliderSetting_C : public UNativeSliderSetting
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x03A8 (size: 0x8)
    class UEditableTextBox* TextBox;                                                  // 0x03B0 (size: 0x8)
    double LabelMultiplier;                                                           // 0x03B8 (size: 0x8)
    int32 LabelFractionalDigits;                                                      // 0x03C0 (size: 0x4)

    void OnSliderValueUpdated(float NormalizedValue, float RawValue);
    void Construct();
    void TextCommitted(const FText& Text, TEnumAsByte<ETextCommit::Type> CommitMethod);
    void ExecuteUbergraph_StyledSliderSetting(int32 EntryPoint);
}; // Size: 0x3C4

#endif

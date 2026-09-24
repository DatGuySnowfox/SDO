#ifndef UE4SS_SDK_WB_Image_Raw_HPP
#define UE4SS_SDK_WB_Image_Raw_HPP

class UWB_Image_Raw_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* Naked_Image;                                                        // 0x0348 (size: 0x8)
    class UBackgroundBlur* Blur;                                                      // 0x0350 (size: 0x8)

    void SetColor(FLinearColor Color);
    void SetBackgroundBlur(bool IsEnabled, double BlurStrength);
    void ExecuteUbergraph_WB_Image_Raw(int32 EntryPoint);
}; // Size: 0x358

#endif

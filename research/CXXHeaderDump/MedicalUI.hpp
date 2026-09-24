#ifndef UE4SS_SDK_MedicalUI_HPP
#define UE4SS_SDK_MedicalUI_HPP

class UMedicalUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* Icon;                                                               // 0x0348 (size: 0x8)
    class UWidgetAnimation* Shake;                                                    // 0x0350 (size: 0x8)
    class UTexture2D* Texture;                                                        // 0x0358 (size: 0x8)

    void SetupUI();
    void Construct();
    void ExecuteUbergraph_MedicalUI(int32 EntryPoint);
}; // Size: 0x360

#endif

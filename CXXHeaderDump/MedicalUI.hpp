#ifndef UE4SS_SDK_MedicalUI_HPP
#define UE4SS_SDK_MedicalUI_HPP

class UMedicalUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Shake;                                                    // 0x02C8 (size: 0x8)
    class UImage* Icon;                                                               // 0x02D0 (size: 0x8)
    class UTexture2D* Texture;                                                        // 0x02D8 (size: 0x8)

    void SetupUI();
    void Construct();
    void ExecuteUbergraph_MedicalUI(int32 EntryPoint);
}; // Size: 0x2E0

#endif

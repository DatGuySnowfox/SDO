#ifndef UE4SS_SDK_AIMarkerUI_HPP
#define UE4SS_SDK_AIMarkerUI_HPP

class UAIMarkerUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* MarkerImg;                                                          // 0x0348 (size: 0x8)
    FAIMarkerUI_CVisible Visible;                                                     // 0x0350 (size: 0x10)
    void Visible();
    FAIMarkerUI_CHidden Hidden;                                                       // 0x0360 (size: 0x10)
    void Hidden();
    class UTexture2D* Texture;                                                        // 0x0370 (size: 0x8)
    FLinearColor Color;                                                               // 0x0378 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_AIMarkerUI(int32 EntryPoint);
    void Hidden__DelegateSignature();
    void Visible__DelegateSignature();
}; // Size: 0x388

#endif

#ifndef UE4SS_SDK_AIMarkerUI_HPP
#define UE4SS_SDK_AIMarkerUI_HPP

class UAIMarkerUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* MarkerImg;                                                          // 0x02C8 (size: 0x8)
    FAIMarkerUI_CVisible Visible;                                                     // 0x02D0 (size: 0x10)
    void Visible();
    FAIMarkerUI_CHidden Hidden;                                                       // 0x02E0 (size: 0x10)
    void Hidden();
    class UTexture2D* Texture;                                                        // 0x02F0 (size: 0x8)
    FLinearColor Color;                                                               // 0x02F8 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_AIMarkerUI(int32 EntryPoint);
    void Hidden__DelegateSignature();
    void Visible__DelegateSignature();
}; // Size: 0x308

#endif

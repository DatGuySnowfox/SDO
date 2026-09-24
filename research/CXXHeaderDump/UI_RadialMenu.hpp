#ifndef UE4SS_SDK_UI_RadialMenu_HPP
#define UE4SS_SDK_UI_RadialMenu_HPP

class UUI_RadialMenu_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UUMG_RadialMenu_C* UMG_RadialMenu;                                          // 0x0348 (size: 0x8)

    void GetSelectedOption(int32& OutIndex, bool& Valid);
    void Construct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void AddRadialMenuItem(class UTexture2D* IconImage, FFBasicIconSettings Settings);
    void ExecuteUbergraph_UI_RadialMenu(int32 EntryPoint);
}; // Size: 0x350

#endif

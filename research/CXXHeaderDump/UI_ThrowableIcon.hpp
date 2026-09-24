#ifndef UE4SS_SDK_UI_ThrowableIcon_HPP
#define UE4SS_SDK_UI_ThrowableIcon_HPP

class UUI_ThrowableIcon_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* Image_52;                                                           // 0x0348 (size: 0x8)
    class UObject* Image;                                                             // 0x0350 (size: 0x8)

    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_UI_ThrowableIcon(int32 EntryPoint);
}; // Size: 0x358

#endif

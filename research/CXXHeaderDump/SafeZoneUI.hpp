#ifndef UE4SS_SDK_SafeZoneUI_HPP
#define UE4SS_SDK_SafeZoneUI_HPP

class USafeZoneUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* Image_58;                                                           // 0x0348 (size: 0x8)
    class UWidgetAnimation* Anim;                                                     // 0x0350 (size: 0x8)
    int32 Zoom;                                                                       // 0x0358 (size: 0x4)
    class AUltra_Dynamic_Sky_C* UDS;                                                  // 0x0360 (size: 0x8)

    void Construct();
    void Destroy();
    void ExecuteUbergraph_SafeZoneUI(int32 EntryPoint);
}; // Size: 0x368

#endif

#ifndef UE4SS_SDK_SafeZoneUI_HPP
#define UE4SS_SDK_SafeZoneUI_HPP

class USafeZoneUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Anim;                                                     // 0x02C8 (size: 0x8)
    class UImage* Image_58;                                                           // 0x02D0 (size: 0x8)
    int32 Zoom;                                                                       // 0x02D8 (size: 0x4)
    class AUltra_Dynamic_Sky_C* UDS;                                                  // 0x02E0 (size: 0x8)

    void Construct();
    void Destroy();
    void ExecuteUbergraph_SafeZoneUI(int32 EntryPoint);
}; // Size: 0x2E8

#endif

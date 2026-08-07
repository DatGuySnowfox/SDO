#ifndef UE4SS_SDK_CompassMarkerUI_HPP
#define UE4SS_SDK_CompassMarkerUI_HPP

class UCompassMarkerUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* Icon;                                                               // 0x02C8 (size: 0x8)
    class UTexture2D* IconTexture;                                                    // 0x02D0 (size: 0x8)
    class AActor* NavigatableActor;                                                   // 0x02D8 (size: 0x8)
    FLinearColor IconColor;                                                           // 0x02E0 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_CompassMarkerUI(int32 EntryPoint);
}; // Size: 0x2F0

#endif

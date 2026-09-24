#ifndef UE4SS_SDK_CompassMarkerUI_HPP
#define UE4SS_SDK_CompassMarkerUI_HPP

class UCompassMarkerUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* Icon;                                                               // 0x0348 (size: 0x8)
    class UTexture2D* IconTexture;                                                    // 0x0350 (size: 0x8)
    class AActor* NavigatableActor;                                                   // 0x0358 (size: 0x8)
    FLinearColor IconColor;                                                           // 0x0360 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_CompassMarkerUI(int32 EntryPoint);
}; // Size: 0x370

#endif

#ifndef UE4SS_SDK_BP_CompassMarker_HPP
#define UE4SS_SDK_BP_CompassMarker_HPP

class ABP_CompassMarker_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A0 (size: 0x8)
    class UTexture2D* Icon;                                                           // 0x02A8 (size: 0x8)
    FLinearColor IconColor;                                                           // 0x02B0 (size: 0x10)
    class UCompassMarkerUI_C* WidgetRef;                                              // 0x02C0 (size: 0x8)

    void ReceiveDestroyed();
    void ExecuteUbergraph_BP_CompassMarker(int32 EntryPoint);
}; // Size: 0x2C8

#endif

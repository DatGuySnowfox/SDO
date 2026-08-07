#ifndef UE4SS_SDK_BP_Marker_HPP
#define UE4SS_SDK_BP_Marker_HPP

class ABP_Marker_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A0 (size: 0x8)
    class UTexture2D* MarkerTexture;                                                  // 0x02A8 (size: 0x8)
    FText MarkerTitle;                                                                // 0x02B0 (size: 0x18)
    FText MarkerDescription;                                                          // 0x02C8 (size: 0x18)
    FVector2D Texture Size;                                                           // 0x02E0 (size: 0x10)
    FLinearColor Color;                                                               // 0x02F0 (size: 0x10)
    TArray<class UW_StandardMarker_C*> Markers;                                       // 0x0300 (size: 0x10)
    bool AddMarkerToCompass?;                                                         // 0x0310 (size: 0x1)
    FLinearColor TooltipBorderColor;                                                  // 0x0314 (size: 0x10)
    class UW_WorldMarker_C* Marker;                                                   // 0x0328 (size: 0x8)

    void AddMarker();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_Marker(int32 EntryPoint);
}; // Size: 0x330

#endif

#ifndef UE4SS_SDK_Weather_Mask_Brush_Component_HPP
#define UE4SS_SDK_Weather_Mask_Brush_Component_HPP

class UWeather_Mask_Brush_Component_C : public UWeatherMask_C
{
    TEnumAsByte<UDS_Weather_Mask_Brush::Type> Brush;                                  // 0x0692 (size: 0x1)
    bool Cancel Masks Above;                                                          // 0x0693 (size: 0x1)
    class UTexture2D* Brush Texture;                                                  // 0x0698 (size: 0x8)
    FLinearColor Brush Color;                                                         // 0x06A0 (size: 0x10)

    void Calculate Masking At Location(FVector Location, FVector2D& Mask, bool& Cancel All Masks);
    void Prepare for Drawing(bool Runtime, class AUltra_Dynamic_Weather_C* UDW);
}; // Size: 0x6B0

#endif

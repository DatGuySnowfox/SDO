#ifndef UE4SS_SDK_UDS_Modifier_HPP
#define UE4SS_SDK_UDS_Modifier_HPP

class UUDS_Modifier_C : public UPrimaryDataAsset
{
    TMap<TEnumAsByte<UDS_Modifier_Float_Property::Type>, double> Float Property Overrides; // 0x0030 (size: 0x50)
    TMap<TEnumAsByte<UDS_Modifier_Color_Property::Type>, FLinearColor> Color Property Overrides; // 0x0080 (size: 0x50)
    TEnumAsByte<EEasingFunc::Type> Easing Function;                                   // 0x00D0 (size: 0x1)
    double Easing Blend Exponent;                                                     // 0x00D8 (size: 0x8)
    double Max Alpha;                                                                 // 0x00E0 (size: 0x8)
    TArray<TEnumAsByte<UDS_Modifier_Float_Property::Type>> Float Property Keys;       // 0x00E8 (size: 0x10)
    TArray<double> Float Property Values;                                             // 0x00F8 (size: 0x10)
    TArray<TEnumAsByte<UDS_Modifier_Color_Property::Type>> Color Property Keys;       // 0x0108 (size: 0x10)
    TArray<FLinearColor> Color Property Values;                                       // 0x0118 (size: 0x10)

    void Parse Maps into Arrays();
}; // Size: 0x128

#endif

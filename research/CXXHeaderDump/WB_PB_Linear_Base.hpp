#ifndef UE4SS_SDK_WB_PB_Linear_Base_HPP
#define UE4SS_SDK_WB_PB_Linear_Base_HPP

class UWB_PB_Linear_Base_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWB_BaseProgressBar_C* BasePB;                                              // 0x02C8 (size: 0x8)
    class UImage* DesignTimeGradient;                                                 // 0x02D0 (size: 0x8)
    class UWB_BaseProgressBar_C* GradientPB;                                          // 0x02D8 (size: 0x8)
    class UOverlay* OV_DesignTimeGradient;                                            // 0x02E0 (size: 0x8)
    class URetainerBox* RB_Gradient;                                                  // 0x02E8 (size: 0x8)
    bool bIsDesignTime;                                                               // 0x02F0 (size: 0x1)
    FVector2D Size;                                                                   // 0x02F8 (size: 0x10)
    FLinearColor FillColor;                                                           // 0x0308 (size: 0x10)
    FLinearColor GradientColor;                                                       // 0x0318 (size: 0x10)
    FLinearColor ProgressChangeColor;                                                 // 0x0328 (size: 0x10)
    bool bIsChanging;                                                                 // 0x0338 (size: 0x1)

    FVector2D FindFillSize(class UObject* Object);
    void Rotate_DesignRetainerGradient(double Rotation);
    void Rotate_DesignTimeGradient(double Angle);
    void Mirror_DesignTimeGradient(bool Mirror);
    void GetPercent(double& Percent);
    void SetPercent(double InPercent);
    void SetFillColor(FLinearColor InColor, double GradientPower, TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void SetSize(FVector2D Size);
    void SetGradientPercent(double Value);
    void SetUseGradient(bool UseGradient);
    void SetFillColorMask(class UObject* Value);
    void SetGradientMask(class UTexture2D* GradientTexture);
    void UpdateProgressChangeColor(FLinearColor NewColor, double InterpSpeed, bool IsChanging);
    void SetBarFillType(TEnumAsByte<EProgressBarFillType::Type> FillType, bool bUseShader);
    void PreConstruct(bool IsDesignTime);
    void Reconstruct();
    void Construct();
    void ExecuteUbergraph_WB_PB_Linear_Base(int32 EntryPoint);
}; // Size: 0x339

#endif

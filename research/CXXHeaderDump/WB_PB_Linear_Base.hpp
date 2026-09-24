#ifndef UE4SS_SDK_WB_PB_Linear_Base_HPP
#define UE4SS_SDK_WB_PB_Linear_Base_HPP

class UWB_PB_Linear_Base_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class URetainerBox* RB_Gradient;                                                  // 0x0348 (size: 0x8)
    class UOverlay* OV_DesignTimeGradient;                                            // 0x0350 (size: 0x8)
    class UWB_BaseProgressBar_C* GradientPB;                                          // 0x0358 (size: 0x8)
    class UImage* DesignTimeGradient;                                                 // 0x0360 (size: 0x8)
    class UWB_BaseProgressBar_C* BasePB;                                              // 0x0368 (size: 0x8)
    bool bIsDesignTime;                                                               // 0x0370 (size: 0x1)
    FVector2D Size;                                                                   // 0x0378 (size: 0x10)
    FLinearColor FillColor;                                                           // 0x0388 (size: 0x10)
    FLinearColor GradientColor;                                                       // 0x0398 (size: 0x10)
    FLinearColor ProgressChangeColor;                                                 // 0x03A8 (size: 0x10)
    bool bIsChanging;                                                                 // 0x03B8 (size: 0x1)

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
}; // Size: 0x3B9

#endif

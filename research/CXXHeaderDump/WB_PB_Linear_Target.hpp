#ifndef UE4SS_SDK_WB_PB_Linear_Target_HPP
#define UE4SS_SDK_WB_PB_Linear_Target_HPP

class UWB_PB_Linear_Target_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UWB_PB_Raw_C* WB_PB_Raw;                                                    // 0x0348 (size: 0x8)
    class UProgressBar* PB_Base;                                                      // 0x0350 (size: 0x8)

    void GetPercent(double& Percent);
    void SetFillColor(FLinearColor InColor);
    void SetBarFillType(TEnumAsByte<EProgressBarFillType::Type> BarFillType, bool bUseShader);
    void SetPercent(double InPercent);
    void SetDefaultValues();
    void ExecuteUbergraph_WB_PB_Linear_Target(int32 EntryPoint);
}; // Size: 0x358

#endif

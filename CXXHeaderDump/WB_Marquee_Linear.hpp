#ifndef UE4SS_SDK_WB_Marquee_Linear_HPP
#define UE4SS_SDK_WB_Marquee_Linear_HPP

class UWB_Marquee_Linear_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* Marquee;                                                            // 0x02C8 (size: 0x8)
    class USizeBox* SB_Marquee;                                                       // 0x02D0 (size: 0x8)
    FVector2D Size;                                                                   // 0x02D8 (size: 0x10)

    void SetMarqueeSize(FVector2D Size);
    void SetMarqueeMask(class UTexture2D* Value);
    void SetMarqueeTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void SetMarqueeSpeed(double Value);
    void SetPercent(double Percent);
    void SetMarqueeMethod(TEnumAsByte<EMarqueeMethod::Type> MarqueeMethod);
    void ExecuteUbergraph_WB_Marquee_Linear(int32 EntryPoint);
}; // Size: 0x2E8

#endif

#ifndef UE4SS_SDK_BP_FuelWidget_HPP
#define UE4SS_SDK_BP_FuelWidget_HPP

class UBP_FuelWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UProgressBar* ProgressBar_62;                                               // 0x0348 (size: 0x8)
    class UTextBlock* Percentage;                                                     // 0x0350 (size: 0x8)
    class UImage* Image_272;                                                          // 0x0358 (size: 0x8)

    void UpdateFuel(double Fuel, double MaxFuel);
    void ExecuteUbergraph_BP_FuelWidget(int32 EntryPoint);
}; // Size: 0x360

#endif

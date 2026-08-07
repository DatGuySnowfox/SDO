#ifndef UE4SS_SDK_BP_RepairWidget_HPP
#define UE4SS_SDK_BP_RepairWidget_HPP

class UBP_RepairWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* Image_272;                                                          // 0x02C8 (size: 0x8)
    class UTextBlock* Percentage;                                                     // 0x02D0 (size: 0x8)
    class UProgressBar* ProgressBar_62;                                               // 0x02D8 (size: 0x8)

    void UpdateHealth(double Health, double MaxHealth);
    void ExecuteUbergraph_BP_RepairWidget(int32 EntryPoint);
}; // Size: 0x2E0

#endif

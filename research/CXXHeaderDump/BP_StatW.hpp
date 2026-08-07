#ifndef UE4SS_SDK_BP_StatW_HPP
#define UE4SS_SDK_BP_StatW_HPP

class UBP_StatW_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* ExtraTxt;                                                       // 0x02C8 (size: 0x8)
    class UTextBlock* PrefixTxt;                                                      // 0x02D0 (size: 0x8)
    class UProgressBar* ProgressBar_76;                                               // 0x02D8 (size: 0x8)
    class USizeBox* ProgressBarBox;                                                   // 0x02E0 (size: 0x8)
    class UTextBlock* TextBlock;                                                      // 0x02E8 (size: 0x8)
    class UTextBlock* TextBlock_56;                                                   // 0x02F0 (size: 0x8)
    FGameplayTag StatName;                                                            // 0x02F8 (size: 0x8)
    FVector2D VectValue;                                                              // 0x0300 (size: 0x10)
    FName Prefix;                                                                     // 0x0310 (size: 0x8)
    FName ExtraText;                                                                  // 0x0318 (size: 0x8)
    FVector2D MinMax;                                                                 // 0x0320 (size: 0x10)
    bool BothMinMaxRolled?;                                                           // 0x0330 (size: 0x1)
    bool NoRandomisation?;                                                            // 0x0331 (size: 0x1)

    void GetStat(FGameplayTag Tag, FText& Name, bool& ProgressBar?);
    void Construct();
    void ExecuteUbergraph_BP_StatW(int32 EntryPoint);
}; // Size: 0x332

#endif

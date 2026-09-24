#ifndef UE4SS_SDK_BP_StatW_HPP
#define UE4SS_SDK_BP_StatW_HPP

class UBP_StatW_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* TextBlock_56;                                                   // 0x0348 (size: 0x8)
    class UTextBlock* TextBlock;                                                      // 0x0350 (size: 0x8)
    class USizeBox* ProgressBarBox;                                                   // 0x0358 (size: 0x8)
    class UProgressBar* ProgressBar_76;                                               // 0x0360 (size: 0x8)
    class UTextBlock* PrefixTxt;                                                      // 0x0368 (size: 0x8)
    class UTextBlock* ExtraTxt;                                                       // 0x0370 (size: 0x8)
    FGameplayTag StatName;                                                            // 0x0378 (size: 0x8)
    FVector2D VectValue;                                                              // 0x0380 (size: 0x10)
    FName Prefix;                                                                     // 0x0390 (size: 0x8)
    FName ExtraText;                                                                  // 0x0398 (size: 0x8)
    FVector2D MinMax;                                                                 // 0x03A0 (size: 0x10)
    bool BothMinMaxRolled?;                                                           // 0x03B0 (size: 0x1)
    bool NoRandomisation?;                                                            // 0x03B1 (size: 0x1)
    bool InvertPercent?;                                                              // 0x03B2 (size: 0x1)

    void GetStat(FGameplayTag Tag, FText& Name, bool& ProgressBar?);
    void Construct();
    void ExecuteUbergraph_BP_StatW(int32 EntryPoint);
}; // Size: 0x3B3

#endif

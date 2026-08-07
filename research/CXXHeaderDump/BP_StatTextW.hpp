#ifndef UE4SS_SDK_BP_StatTextW_HPP
#define UE4SS_SDK_BP_StatTextW_HPP

class UBP_StatTextW_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* TextBlock;                                                      // 0x02C8 (size: 0x8)
    class UTextBlock* TextBlock_56;                                                   // 0x02D0 (size: 0x8)
    FName StatName;                                                                   // 0x02D8 (size: 0x8)
    FName StatValue;                                                                  // 0x02E0 (size: 0x8)

    void GetStat(FGameplayTag Tag, FText& Name);
    void Construct();
    void ExecuteUbergraph_BP_StatTextW(int32 EntryPoint);
}; // Size: 0x2E8

#endif

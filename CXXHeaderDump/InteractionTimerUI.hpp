#ifndef UE4SS_SDK_InteractionTimerUI_HPP
#define UE4SS_SDK_InteractionTimerUI_HPP

class UInteractionTimerUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* ActionText;                                                     // 0x02C8 (size: 0x8)
    class USquareProgressBar_C* SquareProgressBar;                                    // 0x02D0 (size: 0x8)
    class UTextBlock* TextBlock_196;                                                  // 0x02D8 (size: 0x8)
    double CurrentTime;                                                               // 0x02E0 (size: 0x8)
    double MaxTime;                                                                   // 0x02E8 (size: 0x8)
    FText InteractionText;                                                            // 0x02F0 (size: 0x18)

    float GetPercent();
    FText GetPercentText();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_InteractionTimerUI(int32 EntryPoint);
}; // Size: 0x308

#endif

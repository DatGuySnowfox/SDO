#ifndef UE4SS_SDK_InteractionTimerUI_HPP
#define UE4SS_SDK_InteractionTimerUI_HPP

class UInteractionTimerUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* TextBlock_196;                                                  // 0x0348 (size: 0x8)
    class USquareProgressBar_C* SquareProgressBar;                                    // 0x0350 (size: 0x8)
    class UTextBlock* ActionText;                                                     // 0x0358 (size: 0x8)
    double CurrentTime;                                                               // 0x0360 (size: 0x8)
    double MaxTime;                                                                   // 0x0368 (size: 0x8)
    FText InteractionText;                                                            // 0x0370 (size: 0x10)

    float GetPercent();
    FText GetPercentText();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_InteractionTimerUI(int32 EntryPoint);
}; // Size: 0x380

#endif

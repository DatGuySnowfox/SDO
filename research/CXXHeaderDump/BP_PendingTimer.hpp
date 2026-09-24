#ifndef UE4SS_SDK_BP_PendingTimer_HPP
#define UE4SS_SDK_BP_PendingTimer_HPP

class UBP_PendingTimer_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* TextBlock_41;                                                   // 0x0348 (size: 0x8)
    double TimeInSec;                                                                 // 0x0350 (size: 0x8)
    FBP_PendingTimer_COnDone OnDone;                                                  // 0x0358 (size: 0x10)
    void OnDone();

    void UpdateText();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_BP_PendingTimer(int32 EntryPoint);
    void OnDone__DelegateSignature();
}; // Size: 0x368

#endif

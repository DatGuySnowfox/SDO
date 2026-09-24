#ifndef UE4SS_SDK_LockpickUI_HPP
#define UE4SS_SDK_LockpickUI_HPP

class ULockpickUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class ULockWidget_C* LockWidget;                                                  // 0x0348 (size: 0x8)
    class UImage* LockStatic;                                                         // 0x0350 (size: 0x8)
    class UTextBlock* LockpickTxt;                                                    // 0x0358 (size: 0x8)
    class UTextBlock* LockpickDiff;                                                   // 0x0360 (size: 0x8)
    class UTextBlock* ExitTxt;                                                        // 0x0368 (size: 0x8)
    class ABP_LockPickObject_C* Object;                                               // 0x0370 (size: 0x8)
    class UBP_JigComponent_C* JigRef;                                                 // 0x0378 (size: 0x8)
    class ULockPickingComponent_C* LPRef;                                             // 0x0380 (size: 0x8)
    bool Successful;                                                                  // 0x0388 (size: 0x1)
    bool UseLockpick;                                                                 // 0x0389 (size: 0x1)
    double PinsAngle;                                                                 // 0x0390 (size: 0x8)

    FText GetLockpickDiffText();
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FText ExitText();
    void BreakLockpick();
    FText GetLockpickAmountText();
    FText GetLockpickAmount(int32& Lockpicks);
    FEventReply OnMouseMove(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void OnInitialized();
    void PausePressed();
    void Construct();
    void ExecuteUbergraph_LockpickUI(int32 EntryPoint);
}; // Size: 0x398

#endif

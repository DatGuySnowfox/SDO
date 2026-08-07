#ifndef UE4SS_SDK_LockpickUI_HPP
#define UE4SS_SDK_LockpickUI_HPP

class ULockpickUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* ExitTxt;                                                        // 0x02C8 (size: 0x8)
    class UTextBlock* LockpickDiff;                                                   // 0x02D0 (size: 0x8)
    class UTextBlock* LockpickTxt;                                                    // 0x02D8 (size: 0x8)
    class UImage* LockStatic;                                                         // 0x02E0 (size: 0x8)
    class ULockWidget_C* LockWidget;                                                  // 0x02E8 (size: 0x8)
    class ABP_LockPickObject_C* Object;                                               // 0x02F0 (size: 0x8)
    class UBP_JigMultiplayer_C* JigRef;                                               // 0x02F8 (size: 0x8)
    class ULockPickingComponent_C* LPRef;                                             // 0x0300 (size: 0x8)
    bool Successful;                                                                  // 0x0308 (size: 0x1)
    bool UseLockpick;                                                                 // 0x0309 (size: 0x1)
    double PinsAngle;                                                                 // 0x0310 (size: 0x8)

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
}; // Size: 0x318

#endif

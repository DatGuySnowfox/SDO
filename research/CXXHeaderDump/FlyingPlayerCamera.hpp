#ifndef UE4SS_SDK_FlyingPlayerCamera_HPP
#define UE4SS_SDK_FlyingPlayerCamera_HPP

class AFlyingPlayerCamera_C : public ASpectatorPawn
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0348 (size: 0x8)
    class UCameraComponent* Camera;                                                   // 0x0350 (size: 0x8)
    class UAIOInvokerComponent* AIOInvoker;                                           // 0x0358 (size: 0x8)
    class USpringArmComponent* SpringArm;                                             // 0x0360 (size: 0x8)
    FFlyingPlayerCamera_CMovingFreeCamera MovingFreeCamera;                           // 0x0368 (size: 0x10)
    void MovingFreeCamera();
    FFlyingPlayerCamera_CNotMovingFreeCamera NotMovingFreeCamera;                     // 0x0378 (size: 0x10)
    void NotMovingFreeCamera();
    double FocusLoc;                                                                  // 0x0388 (size: 0x8)
    class APawn* Pawn;                                                                // 0x0390 (size: 0x8)

    void InpActEvt_MouseScrollUp_K2Node_InputKeyEvent_2(FKey Key);
    void InpActEvt_MouseScrollDown_K2Node_InputKeyEvent_1(FKey Key);
    void InpActEvt_Escape_K2Node_InputKeyEvent_0(FKey Key);
    void ReceiveTick(float DeltaSeconds);
    void AutoDOF();
    void ReceiveBeginPlay();
    void ReceiveDestroyed();
    void ExecuteUbergraph_FlyingPlayerCamera(int32 EntryPoint);
    void NotMovingFreeCamera__DelegateSignature();
    void MovingFreeCamera__DelegateSignature();
}; // Size: 0x398

#endif

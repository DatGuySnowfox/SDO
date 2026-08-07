#ifndef UE4SS_SDK_PhotoModeFreeCamera_HPP
#define UE4SS_SDK_PhotoModeFreeCamera_HPP

class APhotoModeFreeCamera_C : public ASpectatorPawn
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0348 (size: 0x8)
    class UCameraComponent* Camera;                                                   // 0x0350 (size: 0x8)
    class USpringArmComponent* SpringArm;                                             // 0x0358 (size: 0x8)
    double FreeCameraMaximumDistance;                                                 // 0x0360 (size: 0x8)
    class ACharacter* PlayerPawn;                                                     // 0x0368 (size: 0x8)
    FPhotoModeFreeCamera_CMovingFreeCamera MovingFreeCamera;                          // 0x0370 (size: 0x10)
    void MovingFreeCamera();
    FPhotoModeFreeCamera_CNotMovingFreeCamera NotMovingFreeCamera;                    // 0x0380 (size: 0x10)
    void NotMovingFreeCamera();
    double CameraSensitivity;                                                         // 0x0390 (size: 0x8)
    double TimeDilation;                                                              // 0x0398 (size: 0x8)
    double StartingFOV;                                                               // 0x03A0 (size: 0x8)
    FRotator StartingRotation;                                                        // 0x03A8 (size: 0x18)
    bool UseMaximumDistanceLimit;                                                     // 0x03C0 (size: 0x1)

    void UserConstructionScript();
    void InpActEvt_MouseScrollUp_K2Node_InputKeyEvent_1(FKey Key);
    void InpActEvt_MouseScrollDown_K2Node_InputKeyEvent_0(FKey Key);
    void ReceiveTick(float DeltaSeconds);
    void ExecuteUbergraph_PhotoModeFreeCamera(int32 EntryPoint);
    void NotMovingFreeCamera__DelegateSignature();
    void MovingFreeCamera__DelegateSignature();
}; // Size: 0x3C1

#endif

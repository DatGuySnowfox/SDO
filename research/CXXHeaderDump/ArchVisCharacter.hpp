#ifndef UE4SS_SDK_ArchVisCharacter_HPP
#define UE4SS_SDK_ArchVisCharacter_HPP

class AArchVisCharacter : public ACharacter
{
    FString LookUpAxisName;                                                           // 0x0648 (size: 0x10)
    FString LookUpAtRateAxisName;                                                     // 0x0658 (size: 0x10)
    FString TurnAxisName;                                                             // 0x0668 (size: 0x10)
    FString TurnAtRateAxisName;                                                       // 0x0678 (size: 0x10)
    FString MoveForwardAxisName;                                                      // 0x0688 (size: 0x10)
    FString MoveRightAxisName;                                                        // 0x0698 (size: 0x10)
    float MouseSensitivityScale_Pitch;                                                // 0x06A8 (size: 0x4)
    float MouseSensitivityScale_Yaw;                                                  // 0x06AC (size: 0x4)

}; // Size: 0x6B0

class UArchVisCharMovementComponent : public UCharacterMovementComponent
{
    FRotator RotationalAcceleration;                                                  // 0x0FC8 (size: 0x18)
    FRotator RotationalDeceleration;                                                  // 0x0FE0 (size: 0x18)
    FRotator MaxRotationalVelocity;                                                   // 0x0FF8 (size: 0x18)
    float MinPitch;                                                                   // 0x1010 (size: 0x4)
    float MaxPitch;                                                                   // 0x1014 (size: 0x4)
    float WalkingFriction;                                                            // 0x1018 (size: 0x4)
    float WalkingSpeed;                                                               // 0x101C (size: 0x4)
    float WalkingAcceleration;                                                        // 0x1020 (size: 0x4)

}; // Size: 0x1060

#endif

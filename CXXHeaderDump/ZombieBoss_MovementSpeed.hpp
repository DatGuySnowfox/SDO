#ifndef UE4SS_SDK_ZombieBoss_MovementSpeed_HPP
#define UE4SS_SDK_ZombieBoss_MovementSpeed_HPP

class UZombieBoss_MovementSpeed_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    double Speed;                                                                     // 0x00A0 (size: 0x8)

    void ReceiveActivationAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_ZombieBoss_MovementSpeed(int32 EntryPoint);
}; // Size: 0xA8

#endif

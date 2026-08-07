#ifndef UE4SS_SDK_Zombie_MovementSpeed_HPP
#define UE4SS_SDK_Zombie_MovementSpeed_HPP

class UZombie_MovementSpeed_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    TEnumAsByte<Enum_ZombieMovementType::Type> MovementType;                          // 0x00A0 (size: 0x1)
    class ABP_MasterZombie_C* Zombie;                                                 // 0x00A8 (size: 0x8)

    void ReceiveActivationAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_Zombie_MovementSpeed(int32 EntryPoint);
}; // Size: 0xB0

#endif

#ifndef UE4SS_SDK_BTService_ChangeMovementSpeed_HPP
#define UE4SS_SDK_BTService_ChangeMovementSpeed_HPP

class UBTService_ChangeMovementSpeed_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    TEnumAsByte<Enum_ZombieMovementType::Type> MovementType;                          // 0x00A0 (size: 0x1)

    void ReceiveActivationAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTService_ChangeMovementSpeed(int32 EntryPoint);
}; // Size: 0xA1

#endif

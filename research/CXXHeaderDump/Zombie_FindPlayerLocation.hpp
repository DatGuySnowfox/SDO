#ifndef UE4SS_SDK_Zombie_FindPlayerLocation_HPP
#define UE4SS_SDK_Zombie_FindPlayerLocation_HPP

class UZombie_FindPlayerLocation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Actor;                                                     // 0x00B0 (size: 0x28)
    class AActor* TargetActor;                                                        // 0x00D8 (size: 0x8)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_Zombie_FindPlayerLocation(int32 EntryPoint);
}; // Size: 0xE0

#endif

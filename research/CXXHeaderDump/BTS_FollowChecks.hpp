#ifndef UE4SS_SDK_BTS_FollowChecks_HPP
#define UE4SS_SDK_BTS_FollowChecks_HPP

class UBTS_FollowChecks_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00A0 (size: 0x8)
    FVector Last Follow Location;                                                     // 0x00A8 (size: 0x18)
    class AActor* Follow Attacker;                                                    // 0x00C0 (size: 0x8)
    class AActor* Follow Attack Target;                                               // 0x00C8 (size: 0x8)
    class UEnvQueryInstanceBlueprintWrapper* Follow EQS;                              // 0x00D0 (size: 0x8)

    void ReceiveTick(class AActor* OwnerActor, float DeltaSeconds);
    void Follow Actor Facing Direction();
    void ReceiveActivationAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void Follow Target Damage Taken(class AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void ExecuteUbergraph_BTS_FollowChecks(int32 EntryPoint);
}; // Size: 0xD8

#endif

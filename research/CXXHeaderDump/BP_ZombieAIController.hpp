#ifndef UE4SS_SDK_BP_ZombieAIController_HPP
#define UE4SS_SDK_BP_ZombieAIController_HPP

class ABP_ZombieAIController_C : public ABP_MainEnemyAIController_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x03D0 (size: 0x8)
    class UAIPerceptionComponent* AIPerception;                                       // 0x03D8 (size: 0x8)
    class UBehaviorTree* BehaviourTree;                                               // 0x03E0 (size: 0x8)
    class USD_AIComponent_C* AIComp;                                                  // 0x03E8 (size: 0x8)
    bool PossessionComplete;                                                          // 0x03F0 (size: 0x1)
    bool CanSeeActor;                                                                 // 0x03F1 (size: 0x1)

    void InCombat?(bool& InCombat);
    void CheckSoundLocation(const FVector SoundLocation, bool IgnoreMeshStandingOn?, bool& SoundReceived?);
    void PerceptionUpdate(class AActor* Actor, const FAIStimulus& Stimulus);
    void PerceptionUpdateOLD(class AActor* Actor, const FAIStimulus& Stimulus);
    void ReceivePossess(class APawn* PossessedPawn);
    void BndEvt__BP_ZombieAIController_AIPerception_K2Node_ComponentBoundEvent_4_ActorPerceptionUpdatedDelegate__DelegateSignature(class AActor* Actor, FAIStimulus Stimulus);
    void ExecuteUbergraph_BP_ZombieAIController(int32 EntryPoint);
}; // Size: 0x3F2

#endif

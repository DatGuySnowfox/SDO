#ifndef UE4SS_SDK_BP_AnimalDeer_AIController_HPP
#define UE4SS_SDK_BP_AnimalDeer_AIController_HPP

class ABP_AnimalDeer_AIController_C : public ABP_MainNeutralAIController_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x03C0 (size: 0x8)
    class UAIPerceptionComponent* AIPerception;                                       // 0x03C8 (size: 0x8)
    class USD_AIComponent_C* AIComp;                                                  // 0x03D0 (size: 0x8)
    bool PossessionComplete;                                                          // 0x03D8 (size: 0x1)

    void PerceptionUpdate(class AActor* Actor, const FAIStimulus& Stimulus);
    void BndEvt__BP_Zombie_AIController_AIPerceptionSight_K2Node_ComponentBoundEvent_1_ActorPerceptionUpdatedDelegate__DelegateSignature(class AActor* Actor, FAIStimulus Stimulus);
    void ReceivePossess(class APawn* PossessedPawn);
    void ExecuteUbergraph_BP_AnimalDeer_AIController(int32 EntryPoint);
}; // Size: 0x3D9

#endif

#ifndef UE4SS_SDK_BP_MasterAIController_HPP
#define UE4SS_SDK_BP_MasterAIController_HPP

class ABP_MasterAIController_C : public ADetourCrowdAIController
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x03C0 (size: 0x8)
    class UAIPerceptionComponent* AIPerception;                                       // 0x03C8 (size: 0x8)
    class UBP_SmartAIComponent_C* Smart AI Component;                                 // 0x03D0 (size: 0x8)
    TArray<FS_ReactionTime> React Time Actors;                                        // 0x03D8 (size: 0x10)
    TArray<class AActor*> Sight Actors;                                               // 0x03E8 (size: 0x10)
    TArray<class AActor*> Starting Perceved Actors;                                   // 0x03F8 (size: 0x10)
    double Starting Sight Radius;                                                     // 0x0408 (size: 0x8)
    bool Respawn;                                                                     // 0x0410 (size: 0x1)

    void Actor Attack Target(class AActor*& Attack Target);
    void AI Can Interact?(bool& Can Interact);
    void AI Is Dead?(bool& Dead);
    void Find Reaction Time(class AActor* Actor, int32& Index);
    void Reaction Time Update();
    void Perception Update Check(class AActor* Actor, FAIStimulus AI Stimulus, bool Test Sight);
    void AI Dead();
    void Actor Aim Focus(class AActor* Instigator);
    void Alert Actor Defenders(class AActor* Attacked, class AActor* Attacker);
    void Turret Destroyed Effect();
    void Turret Idle Start();
    void Turret Idle Stop();
    void Turret Start Reload();
    void Turret End Reload();
    void AI Alert(class AActor* Alert Actor);
    void AI End Alert();
    void AI Animation Switch(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void AI Camera Activate(bool Activate);
    void AI Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void ReceivePossess(class APawn* PossessedPawn);
    void BndEvt__AIPerception_K2Node_ComponentBoundEvent_0_ActorPerceptionUpdatedDelegate__DelegateSignature(class AActor* Actor, FAIStimulus Stimulus);
    void Starting Perception();
    void ExecuteUbergraph_BP_MasterAIController(int32 EntryPoint);
}; // Size: 0x411

#endif

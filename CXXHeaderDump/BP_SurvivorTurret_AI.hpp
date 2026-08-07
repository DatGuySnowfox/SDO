#ifndef UE4SS_SDK_BP_SurvivorTurret_AI_HPP
#define UE4SS_SDK_BP_SurvivorTurret_AI_HPP

class ABP_SurvivorTurret_AI_C : public ABP_MasterTurret_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0318 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x0320 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x0328 (size: 0x8)
    float Turret_Rotate_Animation_Rotate_Alpha_B8049F6848C826A27A9CD39B7C60D803;      // 0x0330 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Turret_Rotate_Animation__Direction_B8049F6848C826A27A9CD39B7C60D803; // 0x0334 (size: 0x1)
    class UTimelineComponent* Turret Rotate Animation;                                // 0x0338 (size: 0x8)

    void IsObjectDamageable?(bool& Damageable?);
    void Turret Rotate Animation__FinishedFunc();
    void Turret Rotate Animation__UpdateFunc();
    void Multicast Turret Idle Stop();
    void Multicast Turret Idle Start();
    void Turret Idle Stop();
    void Turret Idle Start();
    void Turret Destroyed Effect();
    void ReceiveBeginPlay();
    void Multicast Spawn Emitter();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_SurvivorTurret_AI(int32 EntryPoint);
}; // Size: 0x340

#endif

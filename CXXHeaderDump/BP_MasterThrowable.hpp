#ifndef UE4SS_SDK_BP_MasterThrowable_HPP
#define UE4SS_SDK_BP_MasterThrowable_HPP

class ABP_MasterThrowable_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02A0 (size: 0x8)
    class UCapsuleComponent* Capsule;                                                 // 0x02A8 (size: 0x8)
    double ExplosionDelay;                                                            // 0x02B0 (size: 0x8)
    class UTechTreeComponent_C* Tech Tree;                                            // 0x02B8 (size: 0x8)
    FBP_MasterThrowable_CEvent_Throw Event_Throw;                                     // 0x02C0 (size: 0x10)
    void Event_Throw();

    void SkillCheck(double Damage, double& Multiplier);
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_MasterThrowable(int32 EntryPoint);
    void Event_Throw__DelegateSignature();
}; // Size: 0x2D0

#endif

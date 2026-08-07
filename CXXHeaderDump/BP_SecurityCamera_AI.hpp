#ifndef UE4SS_SDK_BP_SecurityCamera_AI_HPP
#define UE4SS_SDK_BP_SecurityCamera_AI_HPP

class ABP_SecurityCamera_AI_C : public ABP_MasterCamera_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0330 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x0338 (size: 0x8)
    class UStaticMeshComponent* Cube;                                                 // 0x0340 (size: 0x8)

    void IsObjectDamageable?(bool& Damageable?);
    void Turret Destroyed Effect();
    void Multicast Spawn Emitter(FVector Scale);
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_SecurityCamera_AI(int32 EntryPoint);
}; // Size: 0x348

#endif

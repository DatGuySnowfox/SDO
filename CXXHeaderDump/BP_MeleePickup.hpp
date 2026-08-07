#ifndef UE4SS_SDK_BP_MeleePickup_HPP
#define UE4SS_SDK_BP_MeleePickup_HPP

class ABP_MeleePickup_C : public ABP_StaticMeshPickup_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0310 (size: 0x8)
    class UBP_WeaponsPickupComponent_C* BP_WeaponsPickupComponent;                    // 0x0318 (size: 0x8)
    class UPassiveSkillsComponent_C* PassiveSkillsComponent;                          // 0x0320 (size: 0x8)

    void DamageCalc(double Dmg, bool Power?, bool Headshot?, class AActor* Target, double& NewDmg, FLinearColor& FloatingDmdColour);
    void DurabilityCheck(double Dmg, double& NewDmg);
    void MaxMeleeHits(class UBP_WeaponsPickupComponent_C* Comp, int32& MaxActorsToHitInSwing?);
    void ReduceDurability(double Multiplier);
    void HitObjectWorld(class AActor* Object);
    void HitHead(class AActor* DamagedActor, FVector HitLoc, bool Power?);
    void HitBody(class AActor* DamagedActor, FVector HitLoc, bool Power?);
    void MeleeTracePower();
    void MeleeTrace();
    void NightCreatureCheck(double Input, double& Ouput);
    void HitTrashObject(class AActor* Actor, double Dmg, bool& Hit?);
    void HitFoliageRock(class UObject* Object, FVector SpawnTransform_Location, class UPrimitiveComponent* PrimitiveComponent, int32 Instance, int32 Hits, bool& Hit?);
    void HitFoliageTree(class UObject* Object, FVector SpawnTransform_Location, class UPrimitiveComponent* PrimitiveComponent, int32 Instance, int32 Hits, bool& Hit?);
    void CriticalHit(double Damage, double& NewDamage, FLinearColor& FloatingDmgColour);
    void FindStat(FGameplayTag StatTag, bool& Found, float& MinStat, double& MaxStat);
    void ReceiveBeginPlay();
    void Event_OnHitNormal(const FHitResult& Hit);
    void Event_OnHitPower(const FHitResult& Hit);
    void Client_StrengthXP(double float);
    void Client_HitUI(double Damage, FLinearColor Colour);
    void ExecuteUbergraph_BP_MeleePickup(int32 EntryPoint);
}; // Size: 0x328

#endif

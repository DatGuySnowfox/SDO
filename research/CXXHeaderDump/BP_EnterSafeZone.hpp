#ifndef UE4SS_SDK_BP_EnterSafeZone_HPP
#define UE4SS_SDK_BP_EnterSafeZone_HPP

class ABP_EnterSafeZone_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x02A8 (size: 0x8)
    FText Name;                                                                       // 0x02B0 (size: 0x18)
    FTimerHandle OverlapTimer;                                                        // 0x02C8 (size: 0x8)

    void InRadArea(bool InRadArea);
    void ReceiveBeginPlay();
    void Event_EnterZone(class ABP_PlayerCharacter_C* Actor);
    void Event_ExitZone(class ABP_PlayerCharacter_C* Actor);
    void BndEvt__BP_EnterSafeZone_Sphere_K2Node_ComponentBoundEvent_4_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_EnterSafeZone_Sphere_K2Node_ComponentBoundEvent_5_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void Event_Overlap();
    void ExecuteUbergraph_BP_EnterSafeZone(int32 EntryPoint);
}; // Size: 0x2D0

#endif

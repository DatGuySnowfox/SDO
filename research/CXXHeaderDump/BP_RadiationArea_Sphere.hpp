#ifndef UE4SS_SDK_BP_RadiationArea_Sphere_HPP
#define UE4SS_SDK_BP_RadiationArea_Sphere_HPP

class ABP_RadiationArea_Sphere_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UNiagaraComponent* NS_RadiationDust;                                        // 0x02A8 (size: 0x8)
    class UPostProcessComponent* PostProcess;                                         // 0x02B0 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x02B8 (size: 0x8)
    float ZoneRadius;                                                                 // 0x02C0 (size: 0x4)
    FText Name;                                                                       // 0x02C8 (size: 0x18)
    FTimerHandle OverlapTimer;                                                        // 0x02E0 (size: 0x8)
    bool AdditionalNotification?;                                                     // 0x02E8 (size: 0x1)
    FText Notification Details Message;                                               // 0x02F0 (size: 0x18)
    class UTexture2D* Notification Details Icon;                                      // 0x0308 (size: 0x8)
    FLinearColor Notification Details Icon Colour;                                    // 0x0310 (size: 0x10)
    TEnumAsByte<Enum_RadiationLevel::Type> RadiationLevel;                            // 0x0320 (size: 0x1)
    float ParticleSpawnRate;                                                          // 0x0324 (size: 0x4)
    float ParticlesMaxHeight;                                                         // 0x0328 (size: 0x4)

    void InRadArea(class AActor* Actor, bool InRadArea);
    void UserConstructionScript();
    void BndEvt__BP_RadiationArea_Sphere_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_RadiationArea_Sphere_K2Node_ComponentBoundEvent_1_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void Event_EnteredRadZone(class AActor* Actor);
    void Event_ExitedRadZone(class AActor* Actor);
    void ReceiveBeginPlay();
    void Event_Overlap();
    void Event_Respawn();
    void ExecuteUbergraph_BP_RadiationArea_Sphere(int32 EntryPoint);
}; // Size: 0x32C

#endif

#ifndef UE4SS_SDK_BP_EasterEgg_Campfire_HPP
#define UE4SS_SDK_BP_EasterEgg_Campfire_HPP

class ABP_EasterEgg_Campfire_C : public ABP_MasterLight_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02A8 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x02B0 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x02B8 (size: 0x8)
    class UParticleSystemComponent* ParticleSystem;                                   // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C8 (size: 0x8)
    FTimerHandle Timer;                                                               // 0x02D0 (size: 0x8)

    void ReceiveBeginPlay();
    void CustomEvent_0();
    void EventTimer();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_EasterEgg_Campfire(int32 EntryPoint);
}; // Size: 0x2D8

#endif

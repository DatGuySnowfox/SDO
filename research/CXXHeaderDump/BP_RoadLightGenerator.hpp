#ifndef UE4SS_SDK_BP_RoadLightGenerator_HPP
#define UE4SS_SDK_BP_RoadLightGenerator_HPP

class ABP_RoadLightGenerator_C : public ABP_MasterLight_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* Plane3;                                               // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Plane2;                                               // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Plane1;                                               // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Plane;                                                // 0x02C0 (size: 0x8)
    class UAudioComponent* LightHum_Cue;                                              // 0x02C8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02D0 (size: 0x8)
    class USpotLightComponent* SpotLight1;                                            // 0x02D8 (size: 0x8)
    class UStaticMeshComponent* Pole;                                                 // 0x02E0 (size: 0x8)

    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_RoadLightGenerator(int32 EntryPoint);
}; // Size: 0x2E8

#endif

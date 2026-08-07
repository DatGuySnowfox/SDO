#ifndef UE4SS_SDK_BP_Generator_HPP
#define UE4SS_SDK_BP_Generator_HPP

class ABP_Generator_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAudioComponent* LightHum_Cue;                                              // 0x02A8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Mesh;                                                 // 0x02B8 (size: 0x8)

    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_Generator(int32 EntryPoint);
}; // Size: 0x2C0

#endif

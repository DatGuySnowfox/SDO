#ifndef UE4SS_SDK_Buildable_BaseMarker_HPP
#define UE4SS_SDK_Buildable_BaseMarker_HPP

class ABuildable_BaseMarker_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x0448 (size: 0x8)
    class UMaterialInstance* Marker Material;                                         // 0x0450 (size: 0x8)
    class UW_StandardMarker_C* MarkerRef;                                             // 0x0458 (size: 0x8)
    class ABP_Marker_C* Marker;                                                       // 0x0460 (size: 0x8)

    void ReceiveBeginPlay();
    void AddMarker();
    void ReceiveDestroyed();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_BaseMarker(int32 EntryPoint);
}; // Size: 0x468

#endif

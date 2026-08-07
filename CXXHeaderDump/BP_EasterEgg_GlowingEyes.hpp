#ifndef UE4SS_SDK_BP_EasterEgg_GlowingEyes_HPP
#define UE4SS_SDK_BP_EasterEgg_GlowingEyes_HPP

class ABP_EasterEgg_GlowingEyes_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh1;                                          // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* SM_Prop_DeadBody_Hanging_Male_02;                     // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C0 (size: 0x8)
    float HoverTL_Hover_E47516984B76AFD9C32EA091E99F7074;                             // 0x02C8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> HoverTL__Direction_E47516984B76AFD9C32EA091E99F7074; // 0x02CC (size: 0x1)
    class UTimelineComponent* HoverTL;                                                // 0x02D0 (size: 0x8)
    bool IsNight?;                                                                    // 0x02D8 (size: 0x1)
    FTimerHandle Timer;                                                               // 0x02E0 (size: 0x8)

    void FacePlayer(class AActor* Target);
    void HoverTL__FinishedFunc();
    void HoverTL__UpdateFunc();
    void CustomEvent_0();
    void ReceiveBeginPlay();
    void EventTimer();
    void Svr_SetHidden(bool bNewHidden);
    void BndEvt__BP_MPLootContainer_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_EasterEgg_GlowingEyes(int32 EntryPoint);
}; // Size: 0x2E8

#endif

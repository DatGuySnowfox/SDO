#ifndef UE4SS_SDK_BP_MilitaryTrader_HPP
#define UE4SS_SDK_BP_MilitaryTrader_HPP

class ABP_MilitaryTrader_C : public ABP_TraderMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0388 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x0390 (size: 0x8)

    void GetInteractOptions(TMap<FGameplayTag, FText>& Options);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<FGameplayTag, FText> Options);
    void BndEvt__BP_MPVendor_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_MilitaryTrader(int32 EntryPoint);
}; // Size: 0x398

#endif

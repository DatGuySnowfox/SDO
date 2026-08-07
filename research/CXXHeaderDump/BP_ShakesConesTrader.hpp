#ifndef UE4SS_SDK_BP_ShakesConesTrader_HPP
#define UE4SS_SDK_BP_ShakesConesTrader_HPP

class ABP_ShakesConesTrader_C : public ABP_TraderMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0350 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x0358 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh2;                                      // 0x0360 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh1;                                      // 0x0368 (size: 0x8)

    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void ReceiveBeginPlay();
    void Sounds(bool NewValue);
    void BndEvt__BP_MPVendor_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_ShakesConesTrader(int32 EntryPoint);
}; // Size: 0x370

#endif

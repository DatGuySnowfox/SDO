#ifndef UE4SS_SDK_Buildable_RazorWire_HPP
#define UE4SS_SDK_Buildable_RazorWire_HPP

class ABuildable_RazorWire_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UBoxComponent* Box_0;                                                       // 0x0438 (size: 0x8)
    TArray<class ACharacter*> AIArray;                                                // 0x0440 (size: 0x10)

    void BndEvt__Buildable_BearTrap_Box_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_RazorWire(int32 EntryPoint);
}; // Size: 0x450

#endif

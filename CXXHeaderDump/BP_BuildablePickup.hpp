#ifndef UE4SS_SDK_BP_BuildablePickup_HPP
#define UE4SS_SDK_BP_BuildablePickup_HPP

class ABP_BuildablePickup_C : public ABP_StaticMeshPickup_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0310 (size: 0x8)

    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void PickupBuildFromGround();
    void ExecuteUbergraph_BP_BuildablePickup(int32 EntryPoint);
}; // Size: 0x318

#endif

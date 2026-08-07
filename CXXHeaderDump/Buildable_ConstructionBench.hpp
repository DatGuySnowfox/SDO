#ifndef UE4SS_SDK_Buildable_ConstructionBench_HPP
#define UE4SS_SDK_Buildable_ConstructionBench_HPP

class ABuildable_ConstructionBench_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x0438 (size: 0x8)
    TEnumAsByte<Enum_CraftingTableTypes::Type> CurrentCraftingTable;                  // 0x0440 (size: 0x1)

    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_ConstructionBench(int32 EntryPoint);
}; // Size: 0x441

#endif

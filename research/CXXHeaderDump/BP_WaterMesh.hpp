#ifndef UE4SS_SDK_BP_WaterMesh_HPP
#define UE4SS_SDK_BP_WaterMesh_HPP

class ABP_WaterMesh_C : public ABP_MasterWaterActor_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0330 (size: 0x8)
    class UStaticMeshComponent* Mesh;                                                 // 0x0338 (size: 0x8)

    void UserConstructionScript();
    void ReceiveBeginPlay();
    void BndEvt__BP_WaterBox_Cube_K2Node_ComponentBoundEvent_5_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_WaterBox_Cube_K2Node_ComponentBoundEvent_6_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void ExecuteUbergraph_BP_WaterMesh(int32 EntryPoint);
}; // Size: 0x340

#endif

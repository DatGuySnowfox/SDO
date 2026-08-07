#ifndef UE4SS_SDK_BP_WaterBox_HPP
#define UE4SS_SDK_BP_WaterBox_HPP

class ABP_WaterBox_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* Cube;                                                 // 0x02A8 (size: 0x8)
    bool ShallowWater?;                                                               // 0x02B0 (size: 0x1)
    class UMaterialInterface* Material;                                               // 0x02B8 (size: 0x8)
    class USplineMeshComponent* SplineComp;                                           // 0x02C0 (size: 0x8)
    double Scale X;                                                                   // 0x02C8 (size: 0x8)
    double Scale Y;                                                                   // 0x02D0 (size: 0x8)
    double Scale Z;                                                                   // 0x02D8 (size: 0x8)
    bool Radiated?;                                                                   // 0x02E0 (size: 0x1)

    void SetCollisionType();
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void BndEvt__BP_WaterBox_Cube_K2Node_ComponentBoundEvent_5_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_WaterBox_Cube_K2Node_ComponentBoundEvent_6_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void ExecuteUbergraph_BP_WaterBox(int32 EntryPoint);
}; // Size: 0x2E1

#endif

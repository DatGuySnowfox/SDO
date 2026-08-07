#ifndef UE4SS_SDK_BP_Ladder_HPP
#define UE4SS_SDK_BP_Ladder_HPP

class ABP_Ladder_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UArrowComponent* PlayerRotation;                                            // 0x02A8 (size: 0x8)
    class USceneComponent* PlayerPostition;                                           // 0x02B0 (size: 0x8)
    class UBoxComponent* LadderTopCollision;                                          // 0x02B8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02C0 (size: 0x8)
    class UBoxComponent* LadderCollision;                                             // 0x02C8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02D0 (size: 0x8)
    bool Mesh?;                                                                       // 0x02D8 (size: 0x1)
    FVector MeshBottom;                                                               // 0x02E0 (size: 0x18)
    double PieceHeight;                                                               // 0x02F8 (size: 0x8)
    double OffsetBox;                                                                 // 0x0300 (size: 0x8)
    class UStaticMeshComponent* StaticMeshComponents;                                 // 0x0308 (size: 0x8)
    bool CanClimb?;                                                                   // 0x0310 (size: 0x1)

    void UserConstructionScript();
    void BndEvt__BP_Ladder_LadderCollision_K2Node_ComponentBoundEvent_6_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_Ladder_LadderCollision_K2Node_ComponentBoundEvent_7_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void BndEvt__BP_LadderNew_LadderTopCollision_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_Ladder(int32 EntryPoint);
}; // Size: 0x311

#endif

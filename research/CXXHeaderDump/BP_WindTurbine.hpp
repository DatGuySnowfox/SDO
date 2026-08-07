#ifndef UE4SS_SDK_BP_WindTurbine_HPP
#define UE4SS_SDK_BP_WindTurbine_HPP

class ABP_WindTurbine_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UActor_Weather_Status_C* Actor_Weather_Status;                              // 0x02A8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* SM_Prop_Powerpole_Single_01;                          // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C0 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x02C8 (size: 0x18)
    class AActor* CurrentActor;                                                       // 0x02E0 (size: 0x8)
    FText Name;                                                                       // 0x02E8 (size: 0x18)
    FTimerHandle Timer;                                                               // 0x0300 (size: 0x8)

    void SetInteractDistance(class AActor* Target);
    void CheckDistanceFromActor();
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void Event_Spin();
    void Event_StartSpin();
    void BndEvt__BP_MPLootContainer_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_WindTurbine(int32 EntryPoint);
}; // Size: 0x308

#endif

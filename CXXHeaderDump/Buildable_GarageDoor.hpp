#ifndef UE4SS_SDK_Buildable_GarageDoor_HPP
#define UE4SS_SDK_Buildable_GarageDoor_HPP

class ABuildable_GarageDoor_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x0438 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x0440 (size: 0x8)
    bool Opened?;                                                                     // 0x0448 (size: 0x1)
    bool DoorOpening?;                                                                // 0x0449 (size: 0x1)

    void OnNotifyEnd_3D2E1B16475F14FE5B531DA34A955BAC(FName NotifyName);
    void OnNotifyBegin_3D2E1B16475F14FE5B531DA34A955BAC(FName NotifyName);
    void OnInterrupted_3D2E1B16475F14FE5B531DA34A955BAC(FName NotifyName);
    void OnBlendOut_3D2E1B16475F14FE5B531DA34A955BAC(FName NotifyName);
    void OnCompleted_3D2E1B16475F14FE5B531DA34A955BAC(FName NotifyName);
    void OnNotifyEnd_567E4FB74634D1C193BBA6A29C554021(FName NotifyName);
    void OnNotifyBegin_567E4FB74634D1C193BBA6A29C554021(FName NotifyName);
    void OnInterrupted_567E4FB74634D1C193BBA6A29C554021(FName NotifyName);
    void OnBlendOut_567E4FB74634D1C193BBA6A29C554021(FName NotifyName);
    void OnCompleted_567E4FB74634D1C193BBA6A29C554021(FName NotifyName);
    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void On();
    void Off();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_GarageDoor(int32 EntryPoint);
}; // Size: 0x44A

#endif

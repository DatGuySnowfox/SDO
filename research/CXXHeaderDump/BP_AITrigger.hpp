#ifndef UE4SS_SDK_BP_AITrigger_HPP
#define UE4SS_SDK_BP_AITrigger_HPP

class ABP_AITrigger_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02A0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02A8 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02B0 (size: 0x8)
    FBP_AITrigger_CTrigger Activated Trigger Activated;                               // 0x02B8 (size: 0x10)
    void Trigger Activated(class AActor* Overlap Actor, class ABP_AITrigger_C* AI Trigger);
    FBP_AITrigger_CTrigger Deactivated Trigger Deactivated;                           // 0x02C8 (size: 0x10)
    void Trigger Deactivated(class AActor* End Overlap Actor, class ABP_AITrigger_C* AI Trigger);

    void ReceiveActorBeginOverlap(class AActor* OtherActor);
    void ReceiveActorEndOverlap(class AActor* OtherActor);
    void ExecuteUbergraph_BP_AITrigger(int32 EntryPoint);
    void Trigger Deactivated__DelegateSignature(class AActor* End Overlap Actor, class ABP_AITrigger_C* AI Trigger);
    void Trigger Activated__DelegateSignature(class AActor* Overlap Actor, class ABP_AITrigger_C* AI Trigger);
}; // Size: 0x2D8

#endif

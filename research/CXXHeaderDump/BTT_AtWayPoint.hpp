#ifndef UE4SS_SDK_BTT_AtWayPoint_HPP
#define UE4SS_SDK_BTT_AtWayPoint_HPP

class UBTT_AtWayPoint_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)
    class UAnimMontage* Current Montage;                                              // 0x00B8 (size: 0x8)
    FTimerHandle Next Montage Timer;                                                  // 0x00C0 (size: 0x8)
    FTimerHandle Way Point Timer;                                                     // 0x00C8 (size: 0x8)
    int32 Current Montage Index;                                                      // 0x00D0 (size: 0x4)

    void Next Montage();
    void ReceiveAbort(class AActor* OwnerActor);
    void End Way Point Wait();
    void Get Next Montage();
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_AtWayPoint(int32 EntryPoint);
}; // Size: 0xD4

#endif

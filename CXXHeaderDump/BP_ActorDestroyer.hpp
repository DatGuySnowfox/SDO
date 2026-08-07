#ifndef UE4SS_SDK_BP_ActorDestroyer_HPP
#define UE4SS_SDK_BP_ActorDestroyer_HPP

class ABP_ActorDestroyer_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02A0 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A8 (size: 0x8)
    TArray<class TSubclassOf<AActor>> IgnoreList;                                     // 0x02B0 (size: 0x10)

    void ReceiveBeginPlay();
    void Event_TimerDestroy();
    void ExecuteUbergraph_BP_ActorDestroyer(int32 EntryPoint);
}; // Size: 0x2C0

#endif

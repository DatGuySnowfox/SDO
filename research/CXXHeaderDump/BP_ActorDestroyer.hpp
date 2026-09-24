#ifndef UE4SS_SDK_BP_ActorDestroyer_HPP
#define UE4SS_SDK_BP_ActorDestroyer_HPP

class ABP_ActorDestroyer_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02B0 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02B8 (size: 0x8)
    TArray<TSubclassOf<class AActor>> IgnoreList;                                     // 0x02C0 (size: 0x10)

    void ReceiveBeginPlay();
    void Event_TimerDestroy();
    void ExecuteUbergraph_BP_ActorDestroyer(int32 EntryPoint);
}; // Size: 0x2D0

#endif

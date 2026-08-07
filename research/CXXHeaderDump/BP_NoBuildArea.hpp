#ifndef UE4SS_SDK_BP_NoBuildArea_HPP
#define UE4SS_SDK_BP_NoBuildArea_HPP

class ABP_NoBuildArea_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02A0 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A8 (size: 0x8)
    bool CheckForDifficultyOption?;                                                   // 0x02B0 (size: 0x1)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_NoBuildArea(int32 EntryPoint);
}; // Size: 0x2B1

#endif

#ifndef UE4SS_SDK_BPT_GoToLocation_HPP
#define UE4SS_SDK_BPT_GoToLocation_HPP

class UBPT_GoToLocation_C : public UNarrativeTask
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0088 (size: 0x8)
    FVector GoalLocation;                                                             // 0x0090 (size: 0x18)
    double DistanceTolerance;                                                         // 0x00A8 (size: 0x8)
    FText FriendlyLocationName;                                                       // 0x00B0 (size: 0x18)
    TSubclassOf<class AActor> GoalActorClass;                                         // 0x00C8 (size: 0x8)
    FName GoalActorTag;                                                               // 0x00D0 (size: 0x8)
    bool Invert?;                                                                     // 0x00D8 (size: 0x1)
    class AActor* GoalActor;                                                          // 0x00E0 (size: 0x8)
    class UBP_Narrative3Overlay_C* NarrativeUI;                                       // 0x00E8 (size: 0x8)

    void GetTaskLocationName(FText& Name);
    void GetTaskLocation(FVector& Location);
    FText GetTaskDescription();
    void GetGoalLocation(FVector& GoalLocation);
    void TickTask();
    void K2_BeginTask();
    void Configure Goal Actor();
    void ExecuteUbergraph_BPT_GoToLocation(int32 EntryPoint);
}; // Size: 0xF0

#endif

#ifndef UE4SS_SDK_BP_CustomWaypoint_HPP
#define UE4SS_SDK_BP_CustomWaypoint_HPP

class UBP_CustomWaypoint_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* Text_Distance;                                                  // 0x02C8 (size: 0x8)
    class UTextBlock* Text_LocName;                                                   // 0x02D0 (size: 0x8)
    class UNarrativeTask* Task;                                                       // 0x02D8 (size: 0x8)

    FText GetDistanceFromGoal();
    void Construct();
    void ExecuteUbergraph_BP_CustomWaypoint(int32 EntryPoint);
}; // Size: 0x2E0

#endif

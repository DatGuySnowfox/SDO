#ifndef UE4SS_SDK_Quest_Task_SpawnNPCsToKill_HPP
#define UE4SS_SDK_Quest_Task_SpawnNPCsToKill_HPP

class UQuest_Task_SpawnNPCsToKill_C : public UNarrativeTask
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0080 (size: 0x8)
    FText TargetToKill;                                                               // 0x0088 (size: 0x10)
    FName ActorQuestTag;                                                              // 0x0098 (size: 0x8)

    FText GetTaskDescription();
    void K2_BeginTask();
    void Event_AIDeath();
    void ExecuteUbergraph_Quest_Task_SpawnNPCsToKill(int32 EntryPoint);
}; // Size: 0xA0

#endif

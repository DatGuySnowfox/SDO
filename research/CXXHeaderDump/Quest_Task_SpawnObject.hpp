#ifndef UE4SS_SDK_Quest_Task_SpawnObject_HPP
#define UE4SS_SDK_Quest_Task_SpawnObject_HPP

class UQuest_Task_SpawnObject_C : public UNarrativeTask
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0080 (size: 0x8)
    FName ActorQuestTag;                                                              // 0x0088 (size: 0x8)

    void K2_BeginTask();
    void ExecuteUbergraph_Quest_Task_SpawnObject(int32 EntryPoint);
}; // Size: 0x90

#endif

#ifndef UE4SS_SDK_BPT_CompleteDataTask_HPP
#define UE4SS_SDK_BPT_CompleteDataTask_HPP

class UBPT_CompleteDataTask_C : public UNarrativeTask
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0088 (size: 0x8)
    class UNarrativeDataTask* DataTask;                                               // 0x0090 (size: 0x8)
    FString Argument;                                                                 // 0x0098 (size: 0x10)
    bool Retroactive?;                                                                // 0x00A8 (size: 0x1)

    FText GetTaskDescription();
    void K2_BeginTask();
    void OnTaskCompleted(const class UNarrativeDataTask* NarrativeTask, FString Name);
    void K2_EndTask();
    void ExecuteUbergraph_BPT_CompleteDataTask(int32 EntryPoint);
}; // Size: 0xA9

#endif

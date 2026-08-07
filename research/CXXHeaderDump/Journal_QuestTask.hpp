#ifndef UE4SS_SDK_Journal_QuestTask_HPP
#define UE4SS_SDK_Journal_QuestTask_HPP

class UJournal_QuestTask_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* Cross;                                                              // 0x02C8 (size: 0x8)
    class URichTextBlock* TaskDesc;                                                   // 0x02D0 (size: 0x8)
    FSlateColor TaskCompleteColor;                                                    // 0x02D8 (size: 0x14)
    FSlateColor TaskFailedColor;                                                      // 0x02EC (size: 0x14)
    FSlateColor TaskIncompleteColor;                                                  // 0x0300 (size: 0x14)
    FQuestTask QuestTask;                                                             // 0x0318 (size: 0x48)
    class UTexture2D* TaskCompleteIcon;                                               // 0x0360 (size: 0x8)
    class UTexture2D* TaskIncompleteIcon;                                             // 0x0368 (size: 0x8)
    bool Failed?;                                                                     // 0x0370 (size: 0x1)
    class UNarrativeTask* Task;                                                       // 0x0378 (size: 0x8)
    bool TaskComplete;                                                                // 0x0380 (size: 0x1)
    FText Description;                                                                // 0x0388 (size: 0x18)

    void Construct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_Journal_QuestTask(int32 EntryPoint);
}; // Size: 0x3A0

#endif

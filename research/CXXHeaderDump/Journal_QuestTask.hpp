#ifndef UE4SS_SDK_Journal_QuestTask_HPP
#define UE4SS_SDK_Journal_QuestTask_HPP

class UJournal_QuestTask_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class URichTextBlock* TaskDesc;                                                   // 0x0348 (size: 0x8)
    class UImage* Cross;                                                              // 0x0350 (size: 0x8)
    FSlateColor TaskCompleteColor;                                                    // 0x0358 (size: 0x14)
    FSlateColor TaskFailedColor;                                                      // 0x036C (size: 0x14)
    FSlateColor TaskIncompleteColor;                                                  // 0x0380 (size: 0x14)
    FQuestTask QuestTask;                                                             // 0x0398 (size: 0x40)
    class UTexture2D* TaskCompleteIcon;                                               // 0x03D8 (size: 0x8)
    class UTexture2D* TaskIncompleteIcon;                                             // 0x03E0 (size: 0x8)
    bool Failed?;                                                                     // 0x03E8 (size: 0x1)
    class UNarrativeTask* Task;                                                       // 0x03F0 (size: 0x8)
    bool TaskComplete;                                                                // 0x03F8 (size: 0x1)
    FText Description;                                                                // 0x0400 (size: 0x10)

    void Construct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_Journal_QuestTask(int32 EntryPoint);
}; // Size: 0x410

#endif

#ifndef UE4SS_SDK_BP_QuestTask_HPP
#define UE4SS_SDK_BP_QuestTask_HPP

class UBP_QuestTask_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* Check;                                                              // 0x02C8 (size: 0x8)
    class URichTextBlock* TaskDesc;                                                   // 0x02D0 (size: 0x8)
    FSlateColor TaskCompleteColor;                                                    // 0x02D8 (size: 0x14)
    FSlateColor TaskFailedColor;                                                      // 0x02EC (size: 0x14)
    FSlateColor TaskIncompleteColor;                                                  // 0x0300 (size: 0x14)
    class UTexture2D* TaskCompleteIcon;                                               // 0x0318 (size: 0x8)
    class UTexture2D* TaskIncompleteIcon;                                             // 0x0320 (size: 0x8)
    bool Failed?;                                                                     // 0x0328 (size: 0x1)
    class UNarrativeTask* Task;                                                       // 0x0330 (size: 0x8)
    bool TaskComplete;                                                                // 0x0338 (size: 0x1)
    FText Description;                                                                // 0x0340 (size: 0x18)

    void Construct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_BP_QuestTask(int32 EntryPoint);
}; // Size: 0x358

#endif

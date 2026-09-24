#ifndef UE4SS_SDK_BP_QuestTask_HPP
#define UE4SS_SDK_BP_QuestTask_HPP

class UBP_QuestTask_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class URichTextBlock* TaskDesc;                                                   // 0x0348 (size: 0x8)
    class UImage* Check;                                                              // 0x0350 (size: 0x8)
    FSlateColor TaskCompleteColor;                                                    // 0x0358 (size: 0x14)
    FSlateColor TaskFailedColor;                                                      // 0x036C (size: 0x14)
    FSlateColor TaskIncompleteColor;                                                  // 0x0380 (size: 0x14)
    class UTexture2D* TaskCompleteIcon;                                               // 0x0398 (size: 0x8)
    class UTexture2D* TaskIncompleteIcon;                                             // 0x03A0 (size: 0x8)
    bool Failed?;                                                                     // 0x03A8 (size: 0x1)
    class UNarrativeTask* Task;                                                       // 0x03B0 (size: 0x8)
    bool TaskComplete;                                                                // 0x03B8 (size: 0x1)
    FText Description;                                                                // 0x03C0 (size: 0x10)

    void Construct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_BP_QuestTask(int32 EntryPoint);
}; // Size: 0x3D0

#endif

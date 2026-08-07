#ifndef UE4SS_SDK_QuestBranchUI_HPP
#define UE4SS_SDK_QuestBranchUI_HPP

class UQuestBranchUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class URichTextBlock* StepDescription;                                            // 0x02C8 (size: 0x8)
    class UVerticalBox* TasksBox;                                                     // 0x02D0 (size: 0x8)
    class UHorizontalBox* TitleBox;                                                   // 0x02D8 (size: 0x8)
    class UQuestBranch* QuestBranch;                                                  // 0x02E0 (size: 0x8)
    bool Completed?;                                                                  // 0x02E8 (size: 0x1)
    bool Failed?;                                                                     // 0x02E9 (size: 0x1)
    FText MessageOverride;                                                            // 0x02F0 (size: 0x18)

    void Construct();
    void ExecuteUbergraph_QuestBranchUI(int32 EntryPoint);
}; // Size: 0x308

#endif

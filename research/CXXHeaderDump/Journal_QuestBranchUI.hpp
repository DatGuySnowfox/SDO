#ifndef UE4SS_SDK_Journal_QuestBranchUI_HPP
#define UE4SS_SDK_Journal_QuestBranchUI_HPP

class UJournal_QuestBranchUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UHorizontalBox* TitleBox;                                                   // 0x0348 (size: 0x8)
    class UVerticalBox* TasksBox;                                                     // 0x0350 (size: 0x8)
    class UTextBlock* StepDescription;                                                // 0x0358 (size: 0x8)
    class UQuestBranch* QuestBranch;                                                  // 0x0360 (size: 0x8)
    bool Completed?;                                                                  // 0x0368 (size: 0x1)
    bool Failed?;                                                                     // 0x0369 (size: 0x1)
    FText MessageOverride;                                                            // 0x0370 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_Journal_QuestBranchUI(int32 EntryPoint);
}; // Size: 0x380

#endif

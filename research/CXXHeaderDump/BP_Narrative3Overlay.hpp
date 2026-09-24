#ifndef UE4SS_SDK_BP_Narrative3Overlay_HPP
#define UE4SS_SDK_BP_Narrative3Overlay_HPP

class UBP_Narrative3Overlay_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UVerticalBox* TaskBox;                                                      // 0x0348 (size: 0x8)
    class UCommonTextBlock* QuestUpdatedText;                                         // 0x0350 (size: 0x8)
    class UCommonTextBlock* QuestUpdatedSubtext;                                      // 0x0358 (size: 0x8)
    class URichTextBlock* QuestMessage;                                               // 0x0360 (size: 0x8)
    class URichTextBlock* CurrentDialogueLine;                                        // 0x0368 (size: 0x8)
    class UVerticalBox* BranchesBox;                                                  // 0x0370 (size: 0x8)
    class UCanvasPanel* BaseCanvas;                                                   // 0x0378 (size: 0x8)
    class UWidgetAnimation* FadeInTasks;                                              // 0x0380 (size: 0x8)
    class UDialogueNode_Player* CurrentPlayerReply;                                   // 0x0388 (size: 0x8)
    TArray<UDialogueNode_NPC*> NPCReplies;                                            // 0x0390 (size: 0x10)
    TArray<UDialogueNode_Player*> PlayerReplies;                                      // 0x03A0 (size: 0x10)
    double SpacerMargin;                                                              // 0x03B0 (size: 0x8)
    bool LockDialogueUI?;                                                             // 0x03B8 (size: 0x1)
    class UDialogueNode_NPC* CurrentNPCReply;                                         // 0x03C0 (size: 0x8)
    FTimerHandle ClearNotificationHandle;                                             // 0x03C8 (size: 0x8)
    double LargeNotificationDuration;                                                 // 0x03D0 (size: 0x8)
    TArray<FText> Notifications;                                                      // 0x03D8 (size: 0x10)
    double SmallNotificationDuration;                                                 // 0x03E8 (size: 0x8)
    class USoundBase* SmallNotificationSound;                                         // 0x03F0 (size: 0x8)
    class UW_NarrativeMenu_QuestJournal_C* Journal;                                   // 0x03F8 (size: 0x8)
    TArray<FKey> SelectInputs;                                                        // 0x0400 (size: 0x10)
    TArray<FKey> DownInputs;                                                          // 0x0410 (size: 0x10)
    TArray<FKey> UpInputs;                                                            // 0x0420 (size: 0x10)
    TArray<FKey> EndDialogueInputs;                                                   // 0x0430 (size: 0x10)
    bool ShowPlayerName?;                                                             // 0x0440 (size: 0x1)
    TMap<UNarrativeTask*, UObject*> Wayoints;                                         // 0x0448 (size: 0x50)
    class UNarrativeComponent* NarrativeComponent;                                    // 0x0498 (size: 0x8)
    FText QuestFailedText;                                                            // 0x04A0 (size: 0x10)

    void Show Quest Failure(class UQuest* Quest);
    void On Dialogue Finished(class UDialogue* Dialogue, bool Is Starting New Dialogue?, EExitDialogueReason Reason);
    void On Dialogue Began(class UDialogue* Dialogue);
    void On Player Dialogue Line Started(class UDialogue* Dialogue, class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    void On Player Dialogue Line Finished(class UDialogue* Dialogue, class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    void On NPC Dialogue Line Finished(class UDialogue* Dialogue, class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    void On NPC Dialogue Line Started(class UDialogue* Dialogue, class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    bool All Tasks Complete(class UQuestBranch* Branch);
    void Unbind from Component(class UNarrativeComponent* Component);
    void Bind To Component(class UNarrativeComponent* Component);
    void OnLeaveParty(class UNarrativePartyComponent* LeftParty);
    void OnJoinedParty(class UNarrativePartyComponent* NewParty, class UNarrativePartyComponent* LeftParty);
    void On Quest New State(class UQuest* Quest, const class UQuestState* NewState);
    void On Quest Started(const class UQuest* Quest);
    void On Quest Failed(const class UQuest* Quest, const FText& QuestFailedMessage);
    void On Quest Succeeded(const class UQuest* Quest, const FText& QuestSucceededMessage);
    void On Quest Task Progress Made(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Step, int32 CurrentProgress, int32 RequiredProgress);
    void On Quest Task Completed(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Step);
    void On Quest Branch Completed(const class UQuest* Quest, const class UQuestBranch* Branch);
    void Show Quest Update(class UQuestBranch* UpdatedBranch, class UQuest* Quest);
    FString GetPlayerName();
    void Construct();
    void ExecuteUbergraph_BP_Narrative3Overlay(int32 EntryPoint);
}; // Size: 0x4B0

#endif

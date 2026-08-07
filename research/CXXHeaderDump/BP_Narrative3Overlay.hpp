#ifndef UE4SS_SDK_BP_Narrative3Overlay_HPP
#define UE4SS_SDK_BP_Narrative3Overlay_HPP

class UBP_Narrative3Overlay_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* FadeInTasks;                                              // 0x02C8 (size: 0x8)
    class UCanvasPanel* BaseCanvas;                                                   // 0x02D0 (size: 0x8)
    class UVerticalBox* BranchesBox;                                                  // 0x02D8 (size: 0x8)
    class URichTextBlock* CurrentDialogueLine;                                        // 0x02E0 (size: 0x8)
    class URichTextBlock* QuestMessage;                                               // 0x02E8 (size: 0x8)
    class UTextBlock* QuestUpdatedSubtext;                                            // 0x02F0 (size: 0x8)
    class UTextBlock* QuestUpdatedText;                                               // 0x02F8 (size: 0x8)
    class UVerticalBox* TaskBox;                                                      // 0x0300 (size: 0x8)
    class UDialogueNode_Player* CurrentPlayerReply;                                   // 0x0308 (size: 0x8)
    TArray<class UDialogueNode_NPC*> NPCReplies;                                      // 0x0310 (size: 0x10)
    TArray<class UDialogueNode_Player*> PlayerReplies;                                // 0x0320 (size: 0x10)
    double SpacerMargin;                                                              // 0x0330 (size: 0x8)
    bool LockDialogueUI?;                                                             // 0x0338 (size: 0x1)
    class UDialogueNode_NPC* CurrentNPCReply;                                         // 0x0340 (size: 0x8)
    FTimerHandle ClearNotificationHandle;                                             // 0x0348 (size: 0x8)
    double LargeNotificationDuration;                                                 // 0x0350 (size: 0x8)
    TArray<FText> Notifications;                                                      // 0x0358 (size: 0x10)
    double SmallNotificationDuration;                                                 // 0x0368 (size: 0x8)
    class USoundBase* SmallNotificationSound;                                         // 0x0370 (size: 0x8)
    class UW_NarrativeMenu_QuestJournal_C* Journal;                                   // 0x0378 (size: 0x8)
    TArray<FKey> SelectInputs;                                                        // 0x0380 (size: 0x10)
    TArray<FKey> DownInputs;                                                          // 0x0390 (size: 0x10)
    TArray<FKey> UpInputs;                                                            // 0x03A0 (size: 0x10)
    TArray<FKey> EndDialogueInputs;                                                   // 0x03B0 (size: 0x10)
    bool ShowPlayerName?;                                                             // 0x03C0 (size: 0x1)
    TMap<class UNarrativeTask*, class UObject*> Wayoints;                             // 0x03C8 (size: 0x50)
    class UNarrativeComponent* NarrativeComponent;                                    // 0x0418 (size: 0x8)

    void On Dialogue Finished(class UDialogue* Dialogue, bool Is Starting New Dialogue?);
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
}; // Size: 0x420

#endif

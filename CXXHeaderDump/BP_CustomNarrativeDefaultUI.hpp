#ifndef UE4SS_SDK_BP_CustomNarrativeDefaultUI_HPP
#define UE4SS_SDK_BP_CustomNarrativeDefaultUI_HPP

class UBP_CustomNarrativeDefaultUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* FadeInNotification;                                       // 0x02C8 (size: 0x8)
    class UWidgetAnimation* FadeInTasks;                                              // 0x02D0 (size: 0x8)
    class UCanvasPanel* BaseCanvas;                                                   // 0x02D8 (size: 0x8)
    class UVerticalBox* BranchesBox;                                                  // 0x02E0 (size: 0x8)
    class URichTextBlock* CurrentDialogueLine;                                        // 0x02E8 (size: 0x8)
    class URichTextBlock* CurrentSpeaker;                                             // 0x02F0 (size: 0x8)
    class UImage* DialogueImg;                                                        // 0x02F8 (size: 0x8)
    class UCanvasPanel* DialogueUI;                                                   // 0x0300 (size: 0x8)
    class UVerticalBox* LargeNotificationBox;                                         // 0x0308 (size: 0x8)
    class UTextBlock* LargeNotificationText;                                          // 0x0310 (size: 0x8)
    class UTextBlock* MinorNotificationText;                                          // 0x0318 (size: 0x8)
    class UTextBlock* NPCNameText;                                                    // 0x0320 (size: 0x8)
    class UVerticalBox* PlayerReplyBox;                                               // 0x0328 (size: 0x8)
    class URichTextBlock* QuestMessage;                                               // 0x0330 (size: 0x8)
    class UTextBlock* QuestUpdatedSubtext;                                            // 0x0338 (size: 0x8)
    class UTextBlock* QuestUpdatedText;                                               // 0x0340 (size: 0x8)
    class UTextBlock* SmallNotificationText;                                          // 0x0348 (size: 0x8)
    class UVerticalBox* TaskBox;                                                      // 0x0350 (size: 0x8)
    class UDialogueNode_Player* CurrentPlayerReply;                                   // 0x0358 (size: 0x8)
    TArray<class UDialogueNode_NPC*> NPCReplies;                                      // 0x0360 (size: 0x10)
    TArray<class UDialogueNode_Player*> PlayerReplies;                                // 0x0370 (size: 0x10)
    double SpacerMargin;                                                              // 0x0380 (size: 0x8)
    bool LockDialogueUI?;                                                             // 0x0388 (size: 0x1)
    class UDialogueNode_NPC* CurrentNPCReply;                                         // 0x0390 (size: 0x8)
    FTimerHandle ClearNotificationHandle;                                             // 0x0398 (size: 0x8)
    double LargeNotificationDuration;                                                 // 0x03A0 (size: 0x8)
    TArray<FText> Notifications;                                                      // 0x03A8 (size: 0x10)
    double SmallNotificationDuration;                                                 // 0x03B8 (size: 0x8)
    class USoundBase* SmallNotificationSound;                                         // 0x03C0 (size: 0x8)
    class UObject* Journal;                                                           // 0x03C8 (size: 0x8)
    TArray<FKey> SelectInputs;                                                        // 0x03D0 (size: 0x10)
    TArray<FKey> DownInputs;                                                          // 0x03E0 (size: 0x10)
    TArray<FKey> UpInputs;                                                            // 0x03F0 (size: 0x10)
    TArray<FKey> EndDialogueInputs;                                                   // 0x0400 (size: 0x10)
    bool ShowPlayerName?;                                                             // 0x0410 (size: 0x1)
    TMap<class UNarrativeTask*, class UBP_CustomWaypoint_C*> Wayoints;                // 0x0418 (size: 0x50)
    class UNarrativeComponent* NarrativeComponent;                                    // 0x0468 (size: 0x8)
    TArray<class UBP_CustomDialogueOption_C*> PlayerDialogueOptions;                  // 0x0470 (size: 0x10)

    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void On Quest New State(class UQuest* Quest, const class UQuestState* NewState);
    void On Quest Started(const class UQuest* Quest);
    void On Quest Failed(const class UQuest* Quest, const FText& QuestFailedMessage);
    void On Quest Succeeded(const class UQuest* Quest, const FText& QuestSucceededMessage);
    void On Quest Task Progress Made(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Step, int32 CurrentProgress, int32 RequiredProgress);
    void On Quest Task Completed(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Step);
    void On Quest Branch Completed(const class UQuest* Quest, const class UQuestBranch* Branch);
    void Show Quest Update(class UQuestBranch* UpdatedBranch, class UQuest* Quest);
    void On NPC Dialogue Line Finished(class UDialogue* Dialogue, class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    void On NPC Dialogue Line Started(class UDialogue* Dialogue, class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    void On Player Dialogue Line Finished(class UDialogue* Dialogue, class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    void On Player Dialogue Line Started(class UDialogue* Dialogue, class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    void On Dialogue Finished(class UDialogue* Dialogue, bool IsStartingNewDialogue?);
    void On Dialogue Began(class UDialogue* Dialogue);
    void On Dialogue Option Selected(class UDialogue* Dialogue, class UDialogueNode_Player* Player Reply);
    void On Dialogue Replies Available(class UDialogue* Dialogue, TArray<class UDialogueNode_Player*>& PlayerReplies);
    void OnLeaveParty(class UNarrativePartyComponent* LeftParty);
    void OnJoinedParty(class UNarrativePartyComponent* NewParty, class UNarrativePartyComponent* LeftParty);
    void Unbind from Component(class UNarrativeComponent* Component);
    void Bind To Component(class UNarrativeComponent* Component);
    FString GetPlayerName();
    void GetReplyText(class UDialogueNode* Dialogue Node, FText& Text);
    FEventReply OnPreviewKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    FEventReply OnMouseWheel(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void Select Reply(class UBP_CustomDialogueOption_C* Option);
    void Scroll Forward();
    void Scroll Back();
    void Set Player Reply(class UDialogueNode_Player* Player);
    void Handle Enter Key();
    void Construct();
    void Remove Waypoint(class UNarrativeTask* Task);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void Add Waypoint(class UNarrativeTask* Task);
    void Clear Large Notification();
    void Add Notification(FText Notification, bool Force);
    void Show Notification(FText Notification);
    void Check for Next Notification();
    void Large Notification(FText MajorText, FText MinorText);
    void ExecuteUbergraph_BP_CustomNarrativeDefaultUI(int32 EntryPoint);
}; // Size: 0x480

#endif

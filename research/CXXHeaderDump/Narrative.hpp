#ifndef UE4SS_SDK_Narrative_HPP
#define UE4SS_SDK_Narrative_HPP

#include "Narrative_enums.hpp"

struct FDialogueInfo
{
    class UDialogue* Dialogue;                                                        // 0x0000 (size: 0x8)
    class AActor* NPC;                                                                // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FDialogueLine
{
    FText Text;                                                                       // 0x0000 (size: 0x18)
    ELineDuration Duration;                                                           // 0x0018 (size: 0x1)
    float DurationSecondsOverride;                                                    // 0x001C (size: 0x4)
    class USoundBase* DialogueSound;                                                  // 0x0020 (size: 0x8)
    class UAnimMontage* DialogueMontage;                                              // 0x0028 (size: 0x8)
    class UAnimMontage* FacialAnimation;                                              // 0x0030 (size: 0x8)
    class UNarrativeDialogueSequence* Shot;                                           // 0x0038 (size: 0x8)

}; // Size: 0x40

struct FDialoguePlayParams
{
    FName StartFromID;                                                                // 0x0000 (size: 0x8)
    int32 Priority;                                                                   // 0x0008 (size: 0x4)

}; // Size: 0xC

struct FNarrativeSavedQuest
{
    TSubclassOf<class UQuest> QuestClass;                                             // 0x0000 (size: 0x8)
    FName CurrentStateID;                                                             // 0x0008 (size: 0x8)
    TArray<FSavedQuestBranch> QuestBranches;                                          // 0x0010 (size: 0x10)
    TArray<FName> ReachedStateNames;                                                  // 0x0020 (size: 0x10)

}; // Size: 0x30

struct FNarrativeUpdate
{
    EUpdateType UpdateType;                                                           // 0x0008 (size: 0x1)
    TSubclassOf<class UQuest> QuestClass;                                             // 0x0010 (size: 0x8)
    FString Payload;                                                                  // 0x0018 (size: 0x10)
    TArray<uint8> IntPayload;                                                         // 0x0028 (size: 0x10)

}; // Size: 0x40

struct FPlayerSpeakerInfo : public FSpeakerInfo
{
    class UNarrativeDialogueSequence* SelectingReplyShot;                             // 0x00A8 (size: 0x8)

}; // Size: 0xB0

struct FQuestTask
{
    class UNarrativeDataTask* Task;                                                   // 0x0008 (size: 0x8)
    FString Argument;                                                                 // 0x0010 (size: 0x10)
    int32 Quantity;                                                                   // 0x0020 (size: 0x4)
    bool bHidden;                                                                     // 0x0024 (size: 0x1)
    bool bOptional;                                                                   // 0x0025 (size: 0x1)
    bool bRetroactive;                                                                // 0x0026 (size: 0x1)
    FText TaskDescription;                                                            // 0x0028 (size: 0x18)
    int32 CurrentProgress;                                                            // 0x0040 (size: 0x4)

}; // Size: 0x48

struct FSavedQuestBranch
{
    FName BranchID;                                                                   // 0x0000 (size: 0x8)
    TArray<int32> TasksProgress;                                                      // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FShotTrackingSettings
{
    EShotTrackingRule AvatarToTrack;                                                  // 0x0000 (size: 0x1)
    FName TrackedAvatarCustomID;                                                      // 0x0004 (size: 0x8)
    FVector TrackBoneNudge;                                                           // 0x0010 (size: 0x18)
    bool bUpdateTrackingEveryFrame;                                                   // 0x0028 (size: 0x1)
    float UpdateTrackingInterpSpeed;                                                  // 0x002C (size: 0x4)

}; // Size: 0x30

struct FSpeakerInfo
{
    FName SpeakerID;                                                                  // 0x0000 (size: 0x8)
    FText SpeakerName;                                                                // 0x0008 (size: 0x18)
    class UNarrativeDialogueSequence* DefaultSpeakerShot;                             // 0x0020 (size: 0x8)
    TSubclassOf<class AActor> SpeakerAvatarClass;                                     // 0x0028 (size: 0x8)
    FTransform SpeakerAvatarTransform;                                                // 0x0030 (size: 0x60)
    FLinearColor NodeColor;                                                           // 0x0090 (size: 0x10)
    bool bIsPlayer;                                                                   // 0x00A0 (size: 0x1)

}; // Size: 0xB0

struct FSpeakerSelector
{
    FName SpeakerID;                                                                  // 0x0000 (size: 0x8)

}; // Size: 0x8

class ANarrativeDefaultCinecam : public ACineCameraActor
{
}; // Size: 0xA20

class UDefault__DialogueBlueprintGeneratedClass
{
}; // Size: 0x0

class UDefault__QuestBlueprintGeneratedClass
{
}; // Size: 0x0

class UDialogue : public UObject
{
    TArray<FSpeakerInfo> Speakers;                                                    // 0x0028 (size: 0x10)
    FPlayerSpeakerInfo PlayerSpeakerInfo;                                             // 0x0040 (size: 0xB0)
    TArray<FPlayerSpeakerInfo> PartySpeakerInfo;                                      // 0x00F0 (size: 0x10)
    bool bFreeMovement;                                                               // 0x0100 (size: 0x1)
    bool bCanBeExited;                                                                // 0x0101 (size: 0x1)
    bool bAutoRotateSpeakers;                                                         // 0x0102 (size: 0x1)
    bool bAutoStopMovement;                                                           // 0x0103 (size: 0x1)
    int32 Priority;                                                                   // 0x0104 (size: 0x4)
    FName DefaultHeadBoneName;                                                        // 0x0108 (size: 0x8)
    float DialogueBlendOutTime;                                                       // 0x0110 (size: 0x4)
    bool bAdjustPlayerTransform;                                                      // 0x0114 (size: 0x1)
    FTransform PlayerAutoAdjustTransform;                                             // 0x0120 (size: 0x60)
    TSubclassOf<class UCameraShakeBase> DialogueCameraShake;                          // 0x0180 (size: 0x8)
    class USoundAttenuation* DialogueSoundAttenuation;                                // 0x0188 (size: 0x8)
    class UNarrativeDialogueSequence* DefaultDialogueShot;                            // 0x0190 (size: 0x8)
    class UNarrativeComponent* OwningComp;                                            // 0x0198 (size: 0x8)
    class APawn* OwningPawn;                                                          // 0x01A0 (size: 0x8)
    class APlayerController* OwningController;                                        // 0x01A8 (size: 0x8)
    class UDialogueNode_NPC* RootDialogue;                                            // 0x01B0 (size: 0x8)
    TArray<class UDialogueNode_NPC*> NPCReplies;                                      // 0x01B8 (size: 0x10)
    TArray<class UDialogueNode_Player*> PlayerReplies;                                // 0x01C8 (size: 0x10)
    TArray<class UDialogueNode_NPC*> NPCReplyChain;                                   // 0x01D8 (size: 0x10)
    TArray<class UDialogueNode_Player*> AvailableResponses;                           // 0x01E8 (size: 0x10)
    class UDialogueNode* CurrentNode;                                                 // 0x01F8 (size: 0x8)
    FSpeakerInfo CurrentSpeaker;                                                      // 0x0200 (size: 0xB0)
    class AActor* CurrentSpeakerAvatar;                                               // 0x02B0 (size: 0x8)
    class AActor* CurrentListenerAvatar;                                              // 0x02B8 (size: 0x8)
    class APlayerState* CurrentPartySpeakerAvatar;                                    // 0x02C0 (size: 0x8)
    FDialogueLine CurrentLine;                                                        // 0x02C8 (size: 0x40)
    class ALevelSequenceActor* DialogueSequencePlayer;                                // 0x0308 (size: 0x8)
    class UNarrativeDialogueSequence* CurrentDialogueSequence;                        // 0x0310 (size: 0x8)
    class UAnimMontage* DialogueSpeakerMontage;                                       // 0x0318 (size: 0x8)
    class UAudioComponent* DialogueAudio;                                             // 0x0320 (size: 0x8)
    FDialoguePlayParams PlayParams;                                                   // 0x0328 (size: 0xC)
    TMap<class FName, class AActor*> SpeakerAvatars;                                  // 0x0338 (size: 0x50)
    class AActor* OldViewTarget;                                                      // 0x0388 (size: 0x8)
    FTimerHandle TimerHandle_NPCReplyFinished;                                        // 0x0390 (size: 0x8)
    FTimerHandle TimerHandle_PlayerReplyFinished;                                     // 0x0398 (size: 0x8)

    void TickDialogue(const float DeltaTime);
    void StopDialogueSequence();
    void StopDialogueAnimation();
    bool SkipCurrentLine();
    void PlayPlayerDialogue(class UDialogueNode_Player* PlayerReply, const FDialogueLine& Line);
    void PlayNPCDialogue(class UDialogueNode_NPC* NPCReply, const FDialogueLine& Line, const FSpeakerInfo& Speaker);
    void PlayNextNPCReply();
    void PlayDialogueSound(const FDialogueLine& Line, class AActor* Speaker, class AActor* Listener);
    void PlayDialogueSequence(class UNarrativeDialogueSequence* Sequence, class AActor* Speaker, class AActor* Listener);
    void PlayDialogueNode(class UDialogueNode* Node, const FDialogueLine& Line, const FSpeakerInfo& Speaker, class AActor* SpeakerActor, class AActor* ListenerActor);
    void PlayDialogueAnimation(class UDialogueNode* Node, const FDialogueLine& Line, class AActor* Speaker, class AActor* Listener);
    void OnPlayerDialogueLineStarted(class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    void OnPlayerDialogueLineFinished(class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    void OnNPCDialogueLineStarted(class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    void OnNPCDialogueLineFinished(class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    class AActor* LinkSpeakerAvatar(const FSpeakerInfo& Info);
    void K2_OnEndDialogue();
    void K2_OnBeginDialogue();
    bool IsPartyDialogue();
    FString GetStringVariable(const class UDialogueNode* Node, const FDialogueLine& Line, FString VariableName);
    FVector GetSpeakerHeadLocation(class AActor* Actor);
    class AActor* GetPlayerAvatar();
    TArray<class UDialogueNode*> GetNodes();
    float GetLineDuration(class UDialogueNode* Node, const FDialogueLine& Line);
    FVector GetConversationCenterPoint();
    class AActor* GetAvatar(const FName& SpeakerID);
    void FinishPlayerDialogue();
    void FinishNPCDialogue();
    void FinishDialogueNode(class UDialogueNode* Node, const FDialogueLine& Line, const FSpeakerInfo& Speaker, class AActor* SpeakerActor, class AActor* ListenerActor);
    void EndCurrentLine();
    void DestroySpeakerAvatar(const FSpeakerInfo& Info, class AActor* SpeakerAvatar);
    bool CanSkipCurrentLine();
    void BlendingOutFinished();
    void AdjustPlayerTransform();
}; // Size: 0x3B0

class UDialogueAsset : public UDataAsset
{
    class UDialogue* Dialogue;                                                        // 0x0030 (size: 0x8)

}; // Size: 0x38

class UDialogueBlueprintGeneratedClass : public UBlueprintGeneratedClass
{
    class UDialogue* DialogueTemplate;                                                // 0x0368 (size: 0x8)

}; // Size: 0x370

class UDialogueNode : public UNarrativeNodeBase
{
    FDialogueLine Line;                                                               // 0x0060 (size: 0x40)
    TArray<FDialogueLine> AlternativeLines;                                           // 0x00A0 (size: 0x10)
    FDialogueNodeOnDialogueFinished OnDialogueFinished;                               // 0x00B0 (size: 0x10)
    void OnDialogueNodeFinishedPlaying();
    FDialogueLine PlayedLine;                                                         // 0x00C0 (size: 0x40)
    TArray<class UDialogueNode_NPC*> NPCReplies;                                      // 0x0100 (size: 0x10)
    TArray<class UDialogueNode_Player*> PlayerReplies;                                // 0x0110 (size: 0x10)
    class UDialogue* OwningDialogue;                                                  // 0x0120 (size: 0x8)
    class UNarrativeComponent* OwningComponent;                                       // 0x0128 (size: 0x8)
    FName OnPlayNodeFuncName;                                                         // 0x0130 (size: 0x8)
    FName DirectedAtSpeakerID;                                                        // 0x0138 (size: 0x8)
    bool bIsSkippable;                                                                // 0x0140 (size: 0x1)

}; // Size: 0x148

class UDialogueNode_NPC : public UDialogueNode
{
    class UNarrativeDialogueSequence* SelectingReplyShot;                             // 0x0148 (size: 0x8)
    FName SpeakerID;                                                                  // 0x0150 (size: 0x8)

    FName GetSpeakerID();
}; // Size: 0x158

class UDialogueNode_Player : public UDialogueNode
{
    FText OptionText;                                                                 // 0x0148 (size: 0x18)
    FText HintText;                                                                   // 0x0160 (size: 0x18)
    bool bAutoSelect;                                                                 // 0x0178 (size: 0x1)

    FText GetOptionText(class UDialogue* InDialogue);
    FText GetHintText(class UDialogue* InDialogue);
}; // Size: 0x180

class UNarrativeComponent : public UActorComponent
{
    FNarrativeComponentOnNarrativeDataTaskCompleted OnNarrativeDataTaskCompleted;     // 0x00A0 (size: 0x10)
    void OnNarrativeTaskCompleted(const class UNarrativeDataTask* NarrativeTask, FString Name);
    FNarrativeComponentOnQuestBranchCompleted OnQuestBranchCompleted;                 // 0x00B0 (size: 0x10)
    void OnQuestBranchCompleted(const class UQuest* Quest, const class UQuestBranch* Branch);
    FNarrativeComponentOnQuestNewState OnQuestNewState;                               // 0x00C0 (size: 0x10)
    void OnQuestNewState(class UQuest* Quest, const class UQuestState* NewState);
    FNarrativeComponentOnQuestTaskProgressChanged OnQuestTaskProgressChanged;         // 0x00D0 (size: 0x10)
    void OnQuestTaskProgressChanged(const class UQuest* Quest, const class UNarrativeTask* ProgressedTask, const class UQuestBranch* Branch, int32 OldProgress, int32 NewProgress);
    FNarrativeComponentOnQuestTaskCompleted OnQuestTaskCompleted;                     // 0x00E0 (size: 0x10)
    void OnQuestTaskCompleted(const class UQuest* Quest, const class UNarrativeTask* CompletedTask, const class UQuestBranch* Branch);
    FNarrativeComponentOnQuestSucceeded OnQuestSucceeded;                             // 0x00F0 (size: 0x10)
    void OnQuestSucceeded(const class UQuest* Quest, const FText& QuestSucceededMessage);
    FNarrativeComponentOnQuestFailed OnQuestFailed;                                   // 0x0100 (size: 0x10)
    void OnQuestFailed(const class UQuest* Quest, const FText& QuestFailedMessage);
    FNarrativeComponentOnQuestStarted OnQuestStarted;                                 // 0x0110 (size: 0x10)
    void OnQuestStarted(const class UQuest* Quest);
    FNarrativeComponentOnQuestForgotten OnQuestForgotten;                             // 0x0120 (size: 0x10)
    void OnQuestForgotten(const class UQuest* Quest);
    FNarrativeComponentOnQuestRestarted OnQuestRestarted;                             // 0x0130 (size: 0x10)
    void OnQuestRestarted(const class UQuest* Quest);
    FNarrativeComponentOnBeginSave OnBeginSave;                                       // 0x0140 (size: 0x10)
    void OnBeginSave(FString SaveGameName);
    FNarrativeComponentOnSaveComplete OnSaveComplete;                                 // 0x0150 (size: 0x10)
    void OnSaveComplete(FString SaveGameName);
    FNarrativeComponentOnBeginLoad OnBeginLoad;                                       // 0x0160 (size: 0x10)
    void OnBeginLoad(FString SaveGameName);
    FNarrativeComponentOnLoadComplete OnLoadComplete;                                 // 0x0170 (size: 0x10)
    void OnLoadComplete(FString SaveGameName);
    FNarrativeComponentOnJoinedParty OnJoinedParty;                                   // 0x0180 (size: 0x10)
    void OnJoinedParty(class UNarrativePartyComponent* NewParty, class UNarrativePartyComponent* LeftParty);
    FNarrativeComponentOnLeaveParty OnLeaveParty;                                     // 0x0190 (size: 0x10)
    void OnLeaveParty(class UNarrativePartyComponent* LeftParty);
    FNarrativeComponentOnDialogueBegan OnDialogueBegan;                               // 0x01A0 (size: 0x10)
    void OnDialogueBegan(class UDialogue* Dialogue);
    FNarrativeComponentOnDialogueFinished OnDialogueFinished;                         // 0x01B0 (size: 0x10)
    void OnDialogueFinished(class UDialogue* Dialogue, const bool bStartingNewDialogue);
    FNarrativeComponentOnDialogueOptionSelected OnDialogueOptionSelected;             // 0x01C0 (size: 0x10)
    void DialogueOptionSelected(class UDialogue* Dialogue, class UDialogueNode_Player* PlayerReply);
    FNarrativeComponentOnDialogueRepliesAvailable OnDialogueRepliesAvailable;         // 0x01D0 (size: 0x10)
    void DialogueRepliesAvailable(class UDialogue* Dialogue, const TArray<class UDialogueNode_Player*>& PlayerReplies);
    FNarrativeComponentOnNPCDialogueLineStarted OnNPCDialogueLineStarted;             // 0x01E0 (size: 0x10)
    void NPCDialogueLineStarted(class UDialogue* Dialogue, class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    FNarrativeComponentOnNPCDialogueLineFinished OnNPCDialogueLineFinished;           // 0x01F0 (size: 0x10)
    void NPCDialogueLineFinished(class UDialogue* Dialogue, class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    FNarrativeComponentOnPlayerDialogueLineStarted OnPlayerDialogueLineStarted;       // 0x0200 (size: 0x10)
    void PlayerDialogueLineStarted(class UDialogue* Dialogue, class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    FNarrativeComponentOnPlayerDialogueLineFinished OnPlayerDialogueLineFinished;     // 0x0210 (size: 0x10)
    void PlayerDialogueLineFinished(class UDialogue* Dialogue, class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    TArray<FNarrativeUpdate> PendingUpdateList;                                       // 0x0220 (size: 0x10)
    TArray<class UQuest*> QuestList;                                                  // 0x0230 (size: 0x10)
    class UDialogue* CurrentDialogue;                                                 // 0x0240 (size: 0x8)
    TMap<FString, int32> MasterTaskList;                                              // 0x0248 (size: 0x50)
    class UNarrativePartyComponent* PartyComponent;                                   // 0x02A0 (size: 0x8)
    class APlayerController* OwnerPC;                                                 // 0x02A8 (size: 0x8)

    bool TrySkipCurrentDialogueLine();
    void TrySelectDialogueOption(class UDialogueNode_Player* Option);
    void TryExitDialogue();
    void ServerTrySkipCurrentDialogueLine();
    void ServerTryExitDialogue();
    void ServerSelectDialogueOption(const FName& OptionID);
    void SaveComplete(FString SaveName);
    bool Save(FString SaveName, const int32 Slot);
    bool RestartQuest(TSubclassOf<class UQuest> QuestClass, FName StartFromID);
    void QuestTaskProgressMade(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Branch, int32 OldProgress, int32 NewProgress);
    void QuestTaskCompleted(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Branch);
    void QuestSucceeded(const class UQuest* Quest, const FText& QuestSucceededMessage);
    void QuestStarted(const class UQuest* Quest);
    void QuestNewState(class UQuest* Quest, const class UQuestState* NewState);
    void QuestForgotten(const class UQuest* Quest);
    void QuestFailed(const class UQuest* Quest, const FText& QuestFailedMessage);
    void QuestBranchCompleted(const class UQuest* Quest, const class UQuestBranch* Branch);
    void OnRep_PendingUpdateList();
    void OnRep_PartyComponent(class UNarrativePartyComponent* OldPartyComponent);
    void NarrativeDataTaskCompleted(const class UNarrativeDataTask* NarrativeTask, FString Name);
    void LoadComplete(FString SaveName);
    bool Load(FString SaveName, const int32 Slot);
    bool IsQuestSucceeded(TSubclassOf<class UQuest> QuestClass);
    bool IsQuestStartedOrFinished(TSubclassOf<class UQuest> QuestClass);
    bool IsQuestInProgress(TSubclassOf<class UQuest> QuestClass);
    bool IsQuestFinished(TSubclassOf<class UQuest> QuestClass);
    bool IsQuestFailed(TSubclassOf<class UQuest> QuestClass);
    bool IsInDialogue();
    bool HasDialogueAvailable(TSubclassOf<class UDialogue> Dialogue, const FDialoguePlayParams PlayParams);
    bool HasCompletedTask(const class UNarrativeDataTask* Task, FString Name, const int32 Quantity);
    TArray<class UQuest*> GetSucceededQuests();
    class UQuest* GetQuestInstance(TSubclassOf<class UQuest> QuestClass);
    class UNarrativePartyComponent* GetParty();
    class APawn* GetOwningPawn();
    class APlayerController* GetOwningController();
    int32 GetNumberOfTimesTaskWasCompleted(const class UNarrativeDataTask* Task, FString Name);
    TArray<class UQuest*> GetInProgressQuests();
    TArray<class UQuest*> GetFailedQuests();
    class UDialogue* GetCurrentDialogue();
    TArray<class UQuest*> GetAllQuests();
    bool ForgetQuest(TSubclassOf<class UQuest> QuestClass);
    void DialogueRepliesAvailable(class UDialogue* Dialogue, const TArray<class UDialogueNode_Player*>& PlayerReplies);
    void DialogueLineStarted(class UDialogue* Dialogue, class UDialogueNode* Node, const FDialogueLine& DialogueLine);
    void DialogueLineFinished(class UDialogue* Dialogue, class UDialogueNode* Node, const FDialogueLine& DialogueLine);
    void DialogueFinished(class UDialogue* Dialogue, const bool bStartingNewDialogue);
    void DialogueBegan(class UDialogue* Dialogue);
    bool DeleteSave(FString SaveName, const int32 Slot);
    void ClientSelectDialogueOption(const FName& OptionID, class APlayerState* Selector);
    void ClientRecieveDialogueChunk(const TArray<FName>& NPCReplyChainIDs, const TArray<FName>& AvailableResponseIDs);
    void ClientReceiveSave(const TArray<FNarrativeSavedQuest>& SavedQuests, const TArray<FString>& Tasks, const TArray<int32>& Quantities);
    void ClientExitPartyDialogue();
    void ClientExitDialogue();
    void ClientBeginPartyDialogue(TSubclassOf<class UDialogue> Dialogue, const TArray<FName>& NPCReplyChainIDs, const TArray<FName>& AvailableResponseIDs);
    void ClientBeginDialogue(TSubclassOf<class UDialogue> Dialogue, const TArray<FName>& NPCReplyChainIDs, const TArray<FName>& AvailableResponseIDs);
    void BeginSave(FString SaveName);
    class UQuest* BeginQuest(TSubclassOf<class UQuest> QuestClass, FName StartFromID);
    void BeginLoad(FString SaveName);
    bool BeginDialogue(TSubclassOf<class UDialogue> Dialogue, const FDialoguePlayParams PlayParams);
}; // Size: 0x2B0

class UNarrativeCondition : public UObject
{
    bool bNot;                                                                        // 0x0028 (size: 0x1)
    EPartyConditionPolicy PartyConditionPolicy;                                       // 0x002C (size: 0x4)

    FString GetGraphDisplayText();
    bool CheckCondition(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x30

class UNarrativeDataTask : public UDataAsset
{
    FString TaskName;                                                                 // 0x0030 (size: 0x10)
    FText TaskDescription;                                                            // 0x0040 (size: 0x18)
    FString ArgumentName;                                                             // 0x0058 (size: 0x10)
    FString TaskCategory;                                                             // 0x0068 (size: 0x10)
    FString DefaultArgument;                                                          // 0x0078 (size: 0x10)

}; // Size: 0x88

class UNarrativeDialogueSequence : public UObject
{
    FText FriendlyShotName;                                                           // 0x0028 (size: 0x18)
    TArray<class ULevelSequence*> SequenceAssets;                                     // 0x0040 (size: 0x10)
    FMovieSceneSequencePlaybackSettings PlaybackSettings;                             // 0x0050 (size: 0x20)
    FPlateCropSettings CropSettings;                                                  // 0x0070 (size: 0x4)
    uint8 bShouldRestart;                                                             // 0x0074 (size: 0x1)
    EAnchorOriginRule AnchorOriginRule;                                               // 0x0078 (size: 0x1)
    FVector AnchorOriginNudge;                                                        // 0x0080 (size: 0x18)
    EAnchorRotationRule AnchorRotationRule;                                           // 0x0098 (size: 0x1)
    FName AnchorAvatarCustomID;                                                       // 0x009C (size: 0x8)
    bool bUse180DegreeRule;                                                           // 0x00A4 (size: 0x1)
    float UnitsY180DegreeRule;                                                        // 0x00A8 (size: 0x4)
    float DegreesYaw180DegreeRule;                                                    // 0x00AC (size: 0x4)
    FShotTrackingSettings LookatTrackingSettings;                                     // 0x00B0 (size: 0x30)
    FShotTrackingSettings FocusTrackingSettings;                                      // 0x00E0 (size: 0x30)
    bool bDrawDebugFocusPoint;                                                        // 0x0110 (size: 0x1)
    TWeakObjectPtr<class AActor> Speaker;                                             // 0x0114 (size: 0x8)
    TWeakObjectPtr<class AActor> Listener;                                            // 0x011C (size: 0x8)
    TWeakObjectPtr<class AActor> AnchorActor;                                         // 0x0124 (size: 0x8)
    TWeakObjectPtr<class AActor> LookAtActor;                                         // 0x012C (size: 0x8)
    TWeakObjectPtr<class AActor> FocusActor;                                          // 0x0134 (size: 0x8)
    TWeakObjectPtr<class ALevelSequenceActor> SequenceActor;                          // 0x013C (size: 0x8)
    TWeakObjectPtr<class ACineCameraActor> Cinecam;                                   // 0x0144 (size: 0x8)
    TWeakObjectPtr<class UDialogue> Dialogue;                                         // 0x014C (size: 0x8)

    void PlaySequence();
    void OnStop();
    FTransform GetShotAnchorTransform();
    FText GetGraphDisplayText();
    void EndSequence();
}; // Size: 0x158

class UNarrativeDialogueSettings : public UObject
{
    float DialogueLineAudioSilence;                                                   // 0x0028 (size: 0x4)
    float MinDialogueTextDisplayTime;                                                 // 0x002C (size: 0x4)
    float LettersPerSecondLineDuration;                                               // 0x0030 (size: 0x4)
    bool bAutoSelectSingleResponse;                                                   // 0x0034 (size: 0x1)
    bool bEnableVerticalWiring;                                                       // 0x0035 (size: 0x1)
    TArray<FLinearColor> SpeakerColors;                                               // 0x0038 (size: 0x10)

}; // Size: 0x48

class UNarrativeEvent : public UObject
{
    bool bRefireOnLoad;                                                               // 0x0028 (size: 0x1)
    EEventRuntime EventRuntime;                                                       // 0x0029 (size: 0x1)
    EPartyEventPolicy PartyEventPolicy;                                               // 0x002C (size: 0x4)

    FText GetHintText();
    FString GetGraphDisplayText();
    void ExecuteEvent(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x30

class UNarrativeFunctionLibrary : public UBlueprintFunctionLibrary
{

    FString MakeDisplayString(FString String);
    class UNarrativeDataTask* GetTaskByName(const class UObject* WorldContextObject, FString EventName);
    class UNarrativeComponent* GetNarrativeComponentFromTarget(class AActor* Target);
    class UNarrativeComponent* GetNarrativeComponent(const class UObject* WorldContextObject);
    bool CompleteNarrativeDataTask(class UNarrativeComponent* Target, const class UNarrativeDataTask* Task, FString Argument, const int32 Quantity);
    bool CompleteLooseNarrativeDataTask(class UNarrativeComponent* Target, FString Argument, const int32 Quantity);
}; // Size: 0x28

class UNarrativeNodeBase : public UObject
{
    FVector2D NodePos;                                                                // 0x0028 (size: 0x10)
    TArray<class UNarrativeCondition*> Conditions;                                    // 0x0038 (size: 0x10)
    TArray<class UNarrativeEvent*> Events;                                            // 0x0048 (size: 0x10)
    FName ID;                                                                         // 0x0058 (size: 0x8)

    void ProcessEvents(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent, const EEventRuntime Runtime);
    bool AreConditionsMet(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x60

class UNarrativePartyComponent : public UNarrativeComponent
{
    EPartyDialogueControlPolicy PartyDialogueControlPolicy;                           // 0x02B0 (size: 0x1)
    TArray<class UNarrativeComponent*> PartyMembers;                                  // 0x02B8 (size: 0x10)
    TArray<class APlayerState*> PartyMemberStates;                                    // 0x02C8 (size: 0x10)

    bool RemovePartyMember(class UNarrativeComponent* Member);
    bool IsPartyLeader(class APlayerState* Member);
    TArray<class APlayerState*> GetPartyMemberStates();
    TArray<class UNarrativeComponent*> GetPartyMembers();
    class UNarrativeComponent* GetPartyLeader();
    bool AddPartyMember(class UNarrativeComponent* Member);
}; // Size: 0x2D8

class UNarrativeQuestSettings : public UObject
{
    bool bResetTasksWhenCompleted;                                                    // 0x0028 (size: 0x1)

}; // Size: 0x30

class UNarrativeSaveGame : public USaveGame
{
    TArray<FNarrativeSavedQuest> SavedQuests;                                         // 0x0028 (size: 0x10)
    TMap<FString, int32> MasterTaskList;                                              // 0x0038 (size: 0x50)

}; // Size: 0x88

class UNarrativeTask : public UObject
{
    int32 RequiredQuantity;                                                           // 0x0028 (size: 0x4)
    FText DescriptionOverride;                                                        // 0x0030 (size: 0x18)
    bool bOptional;                                                                   // 0x0048 (size: 0x1)
    bool bHidden;                                                                     // 0x0049 (size: 0x1)
    float TickInterval;                                                               // 0x004C (size: 0x4)
    int32 CurrentProgress;                                                            // 0x0050 (size: 0x4)
    class UNarrativeComponent* OwningComp;                                            // 0x0058 (size: 0x8)
    class APawn* OwningPawn;                                                          // 0x0060 (size: 0x8)
    class APlayerController* OwningController;                                        // 0x0068 (size: 0x8)
    class UQuest* OwningQuest;                                                        // 0x0070 (size: 0x8)

    void TickTask();
    void SetProgress(const int32 NewProgress);
    void K2_OnTaskCompleted();
    void K2_EndTask();
    void K2_BeginTask();
    bool IsComplete();
    FText GetTaskProgressText();
    FText GetTaskNodeDescription();
    FText GetTaskDescription();
    class UQuestBranch* GetOwningBranch();
    void CompleteTask();
    void AddProgress(const int32 ProgressToAdd);
}; // Size: 0x88

class UNarrativeTaskManager : public UGameInstanceSubsystem
{
    TMap<class FString, class UNarrativeDataTask*> NarrativeTaskMap;                  // 0x0030 (size: 0x50)

}; // Size: 0x80

class UQuest : public UObject
{
    class UQuestState* CurrentState;                                                  // 0x0028 (size: 0x8)
    FText QuestName;                                                                  // 0x0030 (size: 0x18)
    FText QuestDescription;                                                           // 0x0048 (size: 0x18)
    bool bSaveQuest;                                                                  // 0x0060 (size: 0x1)
    bool bTracked;                                                                    // 0x0061 (size: 0x1)
    TArray<class UQuestState*> InheritableStates;                                     // 0x0068 (size: 0x10)
    EQuestCompletion QuestCompletion;                                                 // 0x0078 (size: 0x1)
    class UQuestState* QuestStartState;                                               // 0x0080 (size: 0x8)
    TArray<class UQuestState*> States;                                                // 0x0088 (size: 0x10)
    TArray<class UQuestBranch*> Branches;                                             // 0x0098 (size: 0x10)
    TArray<class AActor*> QuestActors;                                                // 0x00A8 (size: 0x10)
    TArray<class UQuestState*> ReachedStates;                                         // 0x00B8 (size: 0x10)
    class UNarrativeComponent* OwningComp;                                            // 0x00C8 (size: 0x8)
    class APawn* OwningPawn;                                                          // 0x00D0 (size: 0x8)
    class APlayerController* OwningController;                                        // 0x00D8 (size: 0x8)
    FQuestQuestBranchCompleted QuestBranchCompleted;                                  // 0x00E0 (size: 0x10)
    void OnQuestBranchCompleted(const class UQuest* Quest, const class UQuestBranch* Branch);
    FQuestQuestNewState QuestNewState;                                                // 0x00F0 (size: 0x10)
    void OnQuestNewState(class UQuest* Quest, const class UQuestState* NewState);
    FQuestQuestTaskProgressChanged QuestTaskProgressChanged;                          // 0x0100 (size: 0x10)
    void OnQuestTaskProgressChanged(const class UQuest* Quest, const class UNarrativeTask* ProgressedTask, const class UQuestBranch* Branch, int32 OldProgress, int32 NewProgress);
    FQuestQuestTaskCompleted QuestTaskCompleted;                                      // 0x0110 (size: 0x10)
    void OnQuestTaskCompleted(const class UQuest* Quest, const class UNarrativeTask* CompletedTask, const class UQuestBranch* Branch);
    FQuestQuestSucceeded QuestSucceeded;                                              // 0x0120 (size: 0x10)
    void OnQuestSucceeded(const class UQuest* Quest, const FText& QuestSucceededMessage);
    FQuestQuestFailed QuestFailed;                                                    // 0x0130 (size: 0x10)
    void OnQuestFailed(const class UQuest* Quest, const FText& QuestFailedMessage);
    FQuestQuestStarted QuestStarted;                                                  // 0x0140 (size: 0x10)
    void OnQuestStarted(const class UQuest* Quest);
    FQuestQuestForgotten QuestForgotten;                                              // 0x0150 (size: 0x10)
    void OnQuestForgotten(const class UQuest* Quest);
    FQuestQuestRestarted QuestRestarted;                                              // 0x0160 (size: 0x10)
    void OnQuestRestarted(const class UQuest* Quest);

    void SucceedQuest(FText QuestSucceededMessage);
    class AActor* SpawnQuestActor(TSubclassOf<class AActor> ActorClass, const FTransform& ActorTransform);
    void SetTracked(const bool bNewTracked);
    void SetQuestName(const FText& NewName);
    void SetQuestDescription(const FText& NewDescription);
    void OnQuestTaskProgressChanged(const class UNarrativeTask* Task, const class UQuestBranch* Step, int32 CurrentProgress, int32 RequiredProgress);
    void OnQuestTaskCompleted(const class UNarrativeTask* Task, const class UQuestBranch* Branch);
    void OnQuestBranchCompleted(const class UQuestBranch* Branch);
    bool IsTracked();
    TArray<class UQuestState*> GetStates();
    class UQuestState* GetState(FName ID);
    class UQuestState* GetQuestStartState();
    FText GetQuestName();
    FText GetQuestDescription();
    EQuestCompletion GetQuestCompletion();
    class APawn* GetOwningPawn();
    class APlayerController* GetOwningController();
    class UNarrativeComponent* GetOwningComp();
    TArray<class UQuestNode*> GetNodes();
    TArray<class APlayerController*> GetGroupMembers();
    TArray<class UQuestBranch*> GetBranches();
    class UQuestBranch* GetBranch(FName ID);
    void FailQuest(FText QuestFailedMessage);
    void EnterState(class UQuestState* NewState);
    void BPPreQuestStarted(const class UQuest* Quest);
    void BPOnTrackedChanged(const class UQuest* Quest, const bool bNewTracked);
    void BPOnQuestTaskProgressChanged(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Step, int32 CurrentProgress, int32 RequiredProgress);
    void BPOnQuestTaskCompleted(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Step);
    void BPOnQuestSucceeded(const class UQuest* Quest, const FText& QuestSucceededMessage);
    void BPOnQuestStarted(const class UQuest* Quest);
    void BPOnQuestNewState(class UQuest* Quest, const class UQuestState* NewState);
    void BPOnQuestFailed(const class UQuest* Quest, const FText& QuestFailedMessage);
    void BPOnQuestBranchCompleted(const class UQuest* Quest, const class UQuestBranch* Branch);
}; // Size: 0x170

class UQuestBlueprintGeneratedClass : public UBlueprintGeneratedClass
{
    class UQuest* QuestTemplate;                                                      // 0x0368 (size: 0x8)

}; // Size: 0x370

class UQuestBranch : public UQuestNode
{
    TArray<class UNarrativeTask*> QuestTasks;                                         // 0x0088 (size: 0x10)
    bool bHidden;                                                                     // 0x0098 (size: 0x1)
    class UQuestState* DestinationState;                                              // 0x00A0 (size: 0x8)

}; // Size: 0xA8

class UQuestNode : public UNarrativeNodeBase
{
    FText Description;                                                                // 0x0060 (size: 0x18)
    FName OnEnteredFuncName;                                                          // 0x0078 (size: 0x8)
    class UQuest* OwningQuest;                                                        // 0x0080 (size: 0x8)

}; // Size: 0x88

class UQuestState : public UQuestNode
{
    TArray<class UQuestBranch*> Branches;                                             // 0x0088 (size: 0x10)
    EStateNodeType StateNodeType;                                                     // 0x0098 (size: 0x1)

}; // Size: 0xA0

#endif

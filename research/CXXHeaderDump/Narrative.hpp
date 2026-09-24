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
    FText Text;                                                                       // 0x0000 (size: 0x10)
    ELineDuration Duration;                                                           // 0x0010 (size: 0x1)
    float DurationSecondsOverride;                                                    // 0x0014 (size: 0x4)
    class USoundBase* DialogueSound;                                                  // 0x0018 (size: 0x8)
    class UAnimMontage* DialogueMontage;                                              // 0x0020 (size: 0x8)
    class UAnimMontage* FacialAnimation;                                              // 0x0028 (size: 0x8)
    class UNarrativeDialogueSequence* Shot;                                           // 0x0030 (size: 0x8)
    TArray<UNarrativeCondition*> Conditions;                                          // 0x0038 (size: 0x10)

}; // Size: 0x48

struct FDialogueNodeSelector : public FNodeIDSelector
{
    TSoftClassPtr<UDialogue> Asset;                                                   // 0x0008 (size: 0x28)

}; // Size: 0x30

struct FDialoguePlayParams
{
    FName StartFromID;                                                                // 0x0000 (size: 0x8)
    int32 Priority;                                                                   // 0x0008 (size: 0x4)
    uint8 bOverride_bFreeMovement;                                                    // 0x000C (size: 0x1)
    bool bFreeMovement;                                                               // 0x0010 (size: 0x1)
    uint8 bOverride_bStopMovement;                                                    // 0x0014 (size: 0x1)
    bool bStopMovement;                                                               // 0x0018 (size: 0x1)
    uint8 bOverride_bUnskippable;                                                     // 0x001C (size: 0x1)
    bool bUnskippable;                                                                // 0x0020 (size: 0x1)

}; // Size: 0x24

struct FInstancedQuestRequirement
{
    class UQuestRequirement* Requirement;                                             // 0x0000 (size: 0x8)

}; // Size: 0x8

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

struct FNodeIDSelector
{
    FName NodeID;                                                                     // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FPlayerSpeakerInfo : public FSpeakerInfo
{
    class UNarrativeDialogueSequence* SelectingReplyShot;                             // 0x00C8 (size: 0x8)

}; // Size: 0xD0

struct FQuestBranchSelector : public FQuestNodeSelector
{
}; // Size: 0x30

struct FQuestNodeSelector : public FNodeIDSelector
{
    TSoftClassPtr<UQuest> Asset;                                                      // 0x0008 (size: 0x28)

}; // Size: 0x30

struct FQuestStateSelector : public FQuestNodeSelector
{
}; // Size: 0x30

struct FQuestTask
{
    class UNarrativeDataTask* Task;                                                   // 0x0008 (size: 0x8)
    FString Argument;                                                                 // 0x0010 (size: 0x10)
    int32 Quantity;                                                                   // 0x0020 (size: 0x4)
    bool bHidden;                                                                     // 0x0024 (size: 0x1)
    bool bOptional;                                                                   // 0x0025 (size: 0x1)
    bool bRetroactive;                                                                // 0x0026 (size: 0x1)
    FText TaskDescription;                                                            // 0x0028 (size: 0x10)
    int32 CurrentProgress;                                                            // 0x0038 (size: 0x4)

}; // Size: 0x40

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
    FText SpeakerName;                                                                // 0x0008 (size: 0x10)
    FGameplayTagContainer OwnedTags;                                                  // 0x0018 (size: 0x20)
    class UNarrativeDialogueSequence* DefaultSpeakerShot;                             // 0x0038 (size: 0x8)
    TSubclassOf<class AActor> SpeakerAvatarClass;                                     // 0x0040 (size: 0x8)
    FTransform SpeakerAvatarTransform;                                                // 0x0050 (size: 0x60)
    FLinearColor NodeColor;                                                           // 0x00B0 (size: 0x10)
    bool bIsPlayer;                                                                   // 0x00C0 (size: 0x1)

}; // Size: 0xD0

struct FSpeakerSelector
{
    FName SpeakerID;                                                                  // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FTaggedDialogue
{
    FGameplayTag Tag;                                                                 // 0x0000 (size: 0x8)
    TSoftClassPtr<UDialogue> Dialogue;                                                // 0x0008 (size: 0x28)
    FDialoguePlayParams PlayParams;                                                   // 0x0030 (size: 0x24)
    float Cooldown;                                                                   // 0x0054 (size: 0x4)
    float MaxDistance;                                                                // 0x0058 (size: 0x4)
    FGameplayTagContainer RequiredTags;                                               // 0x0060 (size: 0x20)
    FGameplayTagContainer BlockedTags;                                                // 0x0080 (size: 0x20)

}; // Size: 0xA0

class ANarrativeDefaultCinecam : public ACineCameraActor
{
}; // Size: 0xAB0

class UDefault__DialogueBlueprintGeneratedClass
{
}; // Size: 0x0

class UDefault__QuestBlueprintGeneratedClass
{
}; // Size: 0x0

class UDialogue : public UObject
{
    TArray<FSpeakerInfo> Speakers;                                                    // 0x0028 (size: 0x10)
    FPlayerSpeakerInfo PlayerSpeakerInfo;                                             // 0x0040 (size: 0xD0)
    TArray<FPlayerSpeakerInfo> PartySpeakerInfo;                                      // 0x0110 (size: 0x10)
    float EndDialogueDist;                                                            // 0x0120 (size: 0x4)
    bool bUnskippable;                                                                // 0x0124 (size: 0x1)
    bool bFreeMovement;                                                               // 0x0125 (size: 0x1)
    bool bCanBeExited;                                                                // 0x0126 (size: 0x1)
    bool bAutoRotateSpeakers;                                                         // 0x0127 (size: 0x1)
    bool bAutoStopMovement;                                                           // 0x0128 (size: 0x1)
    int32 Priority;                                                                   // 0x012C (size: 0x4)
    FName DefaultHeadBoneName;                                                        // 0x0130 (size: 0x8)
    float DialogueBlendOutTime;                                                       // 0x0138 (size: 0x4)
    bool bAdjustPlayerTransform;                                                      // 0x013C (size: 0x1)
    FTransform PlayerAutoAdjustTransform;                                             // 0x0140 (size: 0x60)
    TSubclassOf<class UCameraShakeBase> DialogueCameraShake;                          // 0x01A0 (size: 0x8)
    class USoundAttenuation* DialogueSoundAttenuation;                                // 0x01A8 (size: 0x8)
    class UNarrativeDialogueSequence* DefaultDialogueShot;                            // 0x01B0 (size: 0x8)
    class UNarrativeComponent* OwningComp;                                            // 0x01B8 (size: 0x8)
    class APawn* OwningPawn;                                                          // 0x01C0 (size: 0x8)
    class APlayerController* OwningController;                                        // 0x01C8 (size: 0x8)
    class UDialogueNode_NPC* RootDialogue;                                            // 0x01D0 (size: 0x8)
    TArray<UDialogueNode_NPC*> NPCReplies;                                            // 0x01D8 (size: 0x10)
    TArray<UDialogueNode_Player*> PlayerReplies;                                      // 0x01E8 (size: 0x10)
    TArray<UDialogueNode_NPC*> NPCReplyChain;                                         // 0x01F8 (size: 0x10)
    TArray<UDialogueNode_Player*> AvailableResponses;                                 // 0x0208 (size: 0x10)
    class UDialogueNode* CurrentNode;                                                 // 0x0218 (size: 0x8)
    FSpeakerInfo CurrentSpeaker;                                                      // 0x0220 (size: 0xD0)
    class AActor* CurrentSpeakerAvatar;                                               // 0x02F0 (size: 0x8)
    class AActor* CurrentListenerAvatar;                                              // 0x02F8 (size: 0x8)
    class APlayerState* CurrentPartySpeakerAvatar;                                    // 0x0300 (size: 0x8)
    FDialogueLine CurrentLine;                                                        // 0x0308 (size: 0x48)
    class ALevelSequenceActor* DialogueSequencePlayer;                                // 0x0350 (size: 0x8)
    class UNarrativeDialogueSequence* CurrentDialogueSequence;                        // 0x0358 (size: 0x8)
    class UAnimMontage* DialogueSpeakerMontage;                                       // 0x0360 (size: 0x8)
    class UAudioComponent* DialogueAudio;                                             // 0x0368 (size: 0x8)
    FDialoguePlayParams PlayParams;                                                   // 0x0370 (size: 0x24)
    TMap<FName, AActor*> SpeakerAvatars;                                              // 0x0398 (size: 0x50)
    class AActor* OldViewTarget;                                                      // 0x03E8 (size: 0x8)
    FTimerHandle TimerHandle_NPCReplyFinished;                                        // 0x03F0 (size: 0x8)
    FTimerHandle TimerHandle_PlayerReplyFinished;                                     // 0x03F8 (size: 0x8)

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
    class AActor* LinkSpeakerAvatar(const FSpeakerInfo& Info, const int32 Idx);
    void K2_OnEndDialogue();
    void K2_OnBeginDialogue();
    bool IsPartyDialogue();
    FString GetStringVariable(const class UDialogueNode* Node, const FDialogueLine& Line, FString VariableName);
    FVector GetSpeakerHeadLocation(class AActor* Actor);
    class AActor* GetPlayerAvatar();
    TArray<UDialogueNode*> GetNodes();
    float GetLineDuration(class UDialogueNode* Node, const FDialogueLine& Line);
    FVector GetConversationCenterPoint();
    class AActor* GetAvatar(const FName& SpeakerID);
    void FinishPlayerDialogue();
    void FinishNPCDialogue();
    void FinishDialogueNode(class UDialogueNode* Node, const FDialogueLine& Line, const FSpeakerInfo& Speaker, class AActor* SpeakerActor, class AActor* ListenerActor);
    void EndCurrentLine();
    void DestroySpeakerAvatar(const FSpeakerInfo& Info, class AActor* SpeakerAvatar);
    bool CanSkipDialogue();
    bool CanSkipCurrentLine();
    void BlendingOutFinished();
    void AdjustPlayerTransform();
}; // Size: 0x410

class UDialogueAsset : public UDataAsset
{
    class UDialogue* Dialogue;                                                        // 0x0030 (size: 0x8)

}; // Size: 0x38

class UDialogueBlueprintGeneratedClass : public UBlueprintGeneratedClass
{
    class UDialogue* DialogueTemplate;                                                // 0x0360 (size: 0x8)

}; // Size: 0x368

class UDialogueNode : public UNarrativeNodeBase
{
    FDialogueLine Line;                                                               // 0x0060 (size: 0x48)
    TArray<FDialogueLine> AlternativeLines;                                           // 0x00A8 (size: 0x10)
    FDialogueNodeOnDialogueFinished OnDialogueFinished;                               // 0x00B8 (size: 0x10)
    void OnDialogueNodeFinishedPlaying();
    FDialogueLine PlayedLine;                                                         // 0x00C8 (size: 0x48)
    TArray<UDialogueNode_NPC*> NPCReplies;                                            // 0x0110 (size: 0x10)
    TArray<UDialogueNode_Player*> PlayerReplies;                                      // 0x0120 (size: 0x10)
    class UDialogue* OwningDialogue;                                                  // 0x0130 (size: 0x8)
    class UNarrativeComponent* OwningComponent;                                       // 0x0138 (size: 0x8)
    FName OnPlayNodeFuncName;                                                         // 0x0140 (size: 0x8)
    FName DirectedAtSpeakerID;                                                        // 0x0148 (size: 0x8)
    bool bIsSkippable;                                                                // 0x0150 (size: 0x1)

    FText GetDialogueText();
}; // Size: 0x158

class UDialogueNode_NPC : public UDialogueNode
{
    class UNarrativeDialogueSequence* SelectingReplyShot;                             // 0x0158 (size: 0x8)
    FName SpeakerID;                                                                  // 0x0160 (size: 0x8)

    FName GetSpeakerID();
}; // Size: 0x168

class UDialogueNode_Player : public UDialogueNode
{
    FText OptionText;                                                                 // 0x0158 (size: 0x10)
    FText HintText;                                                                   // 0x0168 (size: 0x10)
    bool bAutoSelect;                                                                 // 0x0178 (size: 0x1)
    bool bAutoSelectIfOnlyReply;                                                      // 0x0179 (size: 0x1)

    FText GetOptionText(class UDialogue* InDialogue);
    FText GetHintText(class UDialogue* InDialogue);
}; // Size: 0x180

class UNarrativeComponent : public UActorComponent
{
    TMap<FString, int32> MasterTaskList;                                              // 0x00B8 (size: 0x50)
    FNarrativeComponentOnNarrativeDataTaskCompleted OnNarrativeDataTaskCompleted;     // 0x0108 (size: 0x10)
    void OnNarrativeTaskCompleted(const class UNarrativeDataTask* NarrativeTask, FString Name);
    FNarrativeComponentOnQuestBranchCompleted OnQuestBranchCompleted;                 // 0x0118 (size: 0x10)
    void OnQuestBranchCompleted(const class UQuest* Quest, const class UQuestBranch* Branch);
    FNarrativeComponentOnQuestNewState OnQuestNewState;                               // 0x0128 (size: 0x10)
    void OnQuestNewState(class UQuest* Quest, const class UQuestState* NewState);
    FNarrativeComponentOnQuestTaskProgressChanged OnQuestTaskProgressChanged;         // 0x0138 (size: 0x10)
    void OnQuestTaskProgressChanged(const class UQuest* Quest, const class UNarrativeTask* ProgressedTask, const class UQuestBranch* Branch, int32 OldProgress, int32 NewProgress);
    FNarrativeComponentOnQuestTaskCompleted OnQuestTaskCompleted;                     // 0x0148 (size: 0x10)
    void OnQuestTaskCompleted(const class UQuest* Quest, const class UNarrativeTask* CompletedTask, const class UQuestBranch* Branch);
    FNarrativeComponentOnQuestSucceeded OnQuestSucceeded;                             // 0x0158 (size: 0x10)
    void OnQuestSucceeded(const class UQuest* Quest, const FText& QuestSucceededMessage);
    FNarrativeComponentOnQuestFailed OnQuestFailed;                                   // 0x0168 (size: 0x10)
    void OnQuestFailed(const class UQuest* Quest, const FText& QuestFailedMessage);
    FNarrativeComponentOnQuestStarted OnQuestStarted;                                 // 0x0178 (size: 0x10)
    void OnQuestStarted(const class UQuest* Quest);
    FNarrativeComponentOnQuestForgotten OnQuestForgotten;                             // 0x0188 (size: 0x10)
    void OnQuestForgotten(const class UQuest* Quest);
    FNarrativeComponentOnQuestRestarted OnQuestRestarted;                             // 0x0198 (size: 0x10)
    void OnQuestRestarted(const class UQuest* Quest);
    FNarrativeComponentOnBeginSave OnBeginSave;                                       // 0x01A8 (size: 0x10)
    void OnBeginSave(FString SaveGameName);
    FNarrativeComponentOnSaveComplete OnSaveComplete;                                 // 0x01B8 (size: 0x10)
    void OnSaveComplete(FString SaveGameName);
    FNarrativeComponentOnBeginLoad OnBeginLoad;                                       // 0x01C8 (size: 0x10)
    void OnBeginLoad(FString SaveGameName);
    FNarrativeComponentOnLoadComplete OnLoadComplete;                                 // 0x01D8 (size: 0x10)
    void OnLoadComplete(FString SaveGameName);
    FNarrativeComponentOnJoinedParty OnJoinedParty;                                   // 0x01E8 (size: 0x10)
    void OnJoinedParty(class UNarrativePartyComponent* NewParty, class UNarrativePartyComponent* LeftParty);
    FNarrativeComponentOnLeaveParty OnLeaveParty;                                     // 0x01F8 (size: 0x10)
    void OnLeaveParty(class UNarrativePartyComponent* LeftParty);
    FNarrativeComponentOnDialogueBegan OnDialogueBegan;                               // 0x0208 (size: 0x10)
    void OnDialogueBegan(class UDialogue* Dialogue);
    FNarrativeComponentOnDialogueFinished OnDialogueFinished;                         // 0x0218 (size: 0x10)
    void OnDialogueFinished(class UDialogue* Dialogue, const bool bStartingNewDialogue, const EExitDialogueReason Reason);
    FNarrativeComponentOnDialogueOptionSelected OnDialogueOptionSelected;             // 0x0228 (size: 0x10)
    void DialogueOptionSelected(class UDialogue* Dialogue, class UDialogueNode_Player* PlayerReply);
    FNarrativeComponentOnDialogueRepliesAvailable OnDialogueRepliesAvailable;         // 0x0238 (size: 0x10)
    void DialogueRepliesAvailable(class UDialogue* Dialogue, const TArray<UDialogueNode_Player*>& PlayerReplies);
    FNarrativeComponentOnNPCDialogueLineStarted OnNPCDialogueLineStarted;             // 0x0248 (size: 0x10)
    void NPCDialogueLineStarted(class UDialogue* Dialogue, class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    FNarrativeComponentOnNPCDialogueLineFinished OnNPCDialogueLineFinished;           // 0x0258 (size: 0x10)
    void NPCDialogueLineFinished(class UDialogue* Dialogue, class UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
    FNarrativeComponentOnPlayerDialogueLineStarted OnPlayerDialogueLineStarted;       // 0x0268 (size: 0x10)
    void PlayerDialogueLineStarted(class UDialogue* Dialogue, class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    FNarrativeComponentOnPlayerDialogueLineFinished OnPlayerDialogueLineFinished;     // 0x0278 (size: 0x10)
    void PlayerDialogueLineFinished(class UDialogue* Dialogue, class UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
    TArray<FNarrativeUpdate> PendingUpdateList;                                       // 0x0288 (size: 0x10)
    TArray<UQuest*> QuestList;                                                        // 0x0298 (size: 0x10)
    class UDialogue* CurrentDialogue;                                                 // 0x02A8 (size: 0x8)
    class UNarrativePartyComponent* PartyComponent;                                   // 0x02B8 (size: 0x8)
    class APlayerController* OwnerPC;                                                 // 0x02C0 (size: 0x8)

    bool TrySkipCurrentDialogueLine();
    void TrySelectDialogueOption(class UDialogueNode_Player* Option);
    bool TryExitDialogue(const EExitDialogueReason Reason);
    void ServerTrySkipCurrentDialogueLine();
    void ServerTryExitDialogue(const EExitDialogueReason Reason);
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
    TArray<UQuest*> GetSucceededQuests();
    class UQuest* GetQuestInstance(TSubclassOf<class UQuest> QuestClass);
    class UNarrativePartyComponent* GetParty();
    class APawn* GetOwningPawn();
    class APlayerController* GetOwningController();
    int32 GetNumberOfTimesTaskWasCompleted(const class UNarrativeDataTask* Task, FString Name);
    TArray<UQuest*> GetInProgressQuests();
    TArray<UQuest*> GetFailedQuests();
    class UDialogue* GetCurrentDialogue();
    TArray<UQuest*> GetAllQuests();
    bool ForgetQuest(TSubclassOf<class UQuest> QuestClass);
    void DialogueRepliesAvailable(class UDialogue* Dialogue, const TArray<UDialogueNode_Player*>& PlayerReplies);
    void DialogueLineStarted(class UDialogue* Dialogue, class UDialogueNode* Node, const FDialogueLine& DialogueLine);
    void DialogueLineFinished(class UDialogue* Dialogue, class UDialogueNode* Node, const FDialogueLine& DialogueLine);
    void DialogueFinished(class UDialogue* Dialogue, const bool bStartingNewDialogue, const EExitDialogueReason Reason);
    void DialogueBegan(class UDialogue* Dialogue);
    bool DeleteSave(FString SaveName, const int32 Slot);
    void ClientSelectDialogueOption(const FName& OptionID, class APlayerState* Selector);
    void ClientRecieveDialogueChunk(const TArray<FName>& NPCReplyChainIDs, const TArray<FName>& AvailableResponseIDs);
    void ClientReceiveSave(const TArray<FNarrativeSavedQuest>& SavedQuests, const TArray<FString>& Tasks, const TArray<int32>& Quantities);
    void ClientExitPartyDialogue(const EExitDialogueReason Reason);
    void ClientExitDialogue(const EExitDialogueReason Reason);
    void ClientBeginPartyDialogue(TSubclassOf<class UDialogue> Dialogue, const TArray<FName>& NPCReplyChainIDs, const TArray<FName>& AvailableResponseIDs);
    void ClientBeginDialogue(TSubclassOf<class UDialogue> Dialogue, const TArray<FName>& NPCReplyChainIDs, const TArray<FName>& AvailableResponseIDs);
    void BeginSave(FString SaveName);
    class UQuest* BeginQuest(TSubclassOf<class UQuest> QuestClass, FName StartFromID);
    void BeginLoad(FString SaveName);
    bool BeginDialogue(TSubclassOf<class UDialogue> Dialogue, const FDialoguePlayParams PlayParams);
}; // Size: 0x2C8

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
    FText TaskDescription;                                                            // 0x0040 (size: 0x10)
    FString ArgumentName;                                                             // 0x0050 (size: 0x10)
    FString TaskCategory;                                                             // 0x0060 (size: 0x10)
    FString DefaultArgument;                                                          // 0x0070 (size: 0x10)

}; // Size: 0x80

class UNarrativeDialogueSequence : public UObject
{
    FText FriendlyShotName;                                                           // 0x0028 (size: 0x10)
    TArray<ULevelSequence*> SequenceAssets;                                           // 0x0038 (size: 0x10)
    FMovieSceneSequencePlaybackSettings PlaybackSettings;                             // 0x0048 (size: 0x28)
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
    TArray<UNarrativeCondition*> Conditions;                                          // 0x0030 (size: 0x10)

    void OnDeactivate(class APawn* Target, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
    void OnActivate(class APawn* Target, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
    FText GetHintText();
    FString GetGraphDisplayText();
    void ExecuteEvent(class APawn* Target, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
    bool AreConditionsMet(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x40

class UNarrativeFunctionLibrary : public UBlueprintFunctionLibrary
{

    FQuestStateSelector MakeQuestStateSelectorFromID(FName NodeID);
    FQuestStateSelector MakeQuestStateSelector(FQuestStateSelector Selector);
    FQuestBranchSelector MakeQuestBranchSelectorFromID(FName NodeID);
    FQuestBranchSelector MakeQuestBranchSelector(FQuestBranchSelector Selector);
    FString MakeDisplayString(FString String);
    FDialogueNodeSelector MakeDialogueNodeSelectorFromID(FName NodeID);
    FDialogueNodeSelector MakeDialogueNodeSelector(FDialogueNodeSelector Selector);
    class UNarrativeDataTask* GetTaskByName(const class UObject* WorldContextObject, FString EventName);
    class UNarrativeComponent* GetNarrativeComponentFromTarget(class AActor* Target);
    class UNarrativeComponent* GetNarrativeComponent(const class UObject* WorldContextObject);
    FName Conv_QuestStateSelectorToName(const FQuestStateSelector& Selector);
    FName Conv_QuestBranchSelectorToName(const FQuestBranchSelector& Selector);
    FQuestStateSelector Conv_NameToQuestStateSelector(const FName& NodeID);
    FQuestBranchSelector Conv_NameToQuestBranchSelector(const FName& NodeID);
    FDialogueNodeSelector Conv_NameToDialogueNodeSelector(const FName& NodeID);
    FName Conv_DialogueNodeSelectorToName(const FDialogueNodeSelector& Selector);
    bool CompleteNarrativeDataTask(class UNarrativeComponent* Target, const class UNarrativeDataTask* Task, FString Argument, const int32 Quantity);
    bool CompleteLooseNarrativeDataTask(class UNarrativeComponent* Target, FString Argument, const int32 Quantity);
    void BreakQuestStateSelector(const FQuestStateSelector& Selector, FName& NodeID);
    void BreakQuestBranchSelector(const FQuestBranchSelector& Selector, FName& NodeID);
    void BreakDialogueNodeSelector(const FDialogueNodeSelector& Selector, FName& NodeID);
}; // Size: 0x28

class UNarrativeNodeBase : public UObject
{
    FVector2D NodePos;                                                                // 0x0028 (size: 0x10)
    TArray<UNarrativeCondition*> Conditions;                                          // 0x0038 (size: 0x10)
    TArray<UNarrativeEvent*> Events;                                                  // 0x0048 (size: 0x10)
    FName ID;                                                                         // 0x0058 (size: 0x8)

    void ProcessEvents(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent, const EEventRuntime Runtime);
    bool AreConditionsMet(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x60

class UNarrativePartyComponent : public UNarrativeComponent
{
    EPartyDialogueControlPolicy PartyDialogueControlPolicy;                           // 0x02C8 (size: 0x1)
    TArray<UNarrativeComponent*> PartyMembers;                                        // 0x02D0 (size: 0x10)
    TArray<APlayerState*> PartyMemberStates;                                          // 0x02E0 (size: 0x10)

    bool RemovePartyMember(class UNarrativeComponent* Member);
    bool IsPartyLeader(class APlayerState* Member);
    TArray<APlayerState*> GetPartyMemberStates();
    TArray<UNarrativeComponent*> GetPartyMembers();
    class UNarrativeComponent* GetPartyLeader();
    bool AddPartyMember(class UNarrativeComponent* Member);
}; // Size: 0x2F0

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
    FText DescriptionOverride;                                                        // 0x0030 (size: 0x10)
    bool bOptional;                                                                   // 0x0040 (size: 0x1)
    bool bHidden;                                                                     // 0x0041 (size: 0x1)
    float TickInterval;                                                               // 0x0044 (size: 0x4)
    int32 CurrentProgress;                                                            // 0x0048 (size: 0x4)
    class UNarrativeComponent* OwningComp;                                            // 0x0050 (size: 0x8)
    class APawn* OwningPawn;                                                          // 0x0058 (size: 0x8)
    class APlayerController* OwningController;                                        // 0x0060 (size: 0x8)
    class UQuest* OwningQuest;                                                        // 0x0068 (size: 0x8)

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
}; // Size: 0x80

class UNarrativeTaskManager : public UGameInstanceSubsystem
{
    TMap<FString, UNarrativeDataTask*> NarrativeTaskMap;                              // 0x0030 (size: 0x50)

}; // Size: 0x80

class UQuest : public UObject
{
    class UQuestState* CurrentState;                                                  // 0x0028 (size: 0x8)
    FText QuestName;                                                                  // 0x0030 (size: 0x10)
    FText QuestDescription;                                                           // 0x0040 (size: 0x10)
    bool bTracked;                                                                    // 0x0050 (size: 0x1)
    TSubclassOf<class UDialogue> QuestDialogue;                                       // 0x0058 (size: 0x8)
    bool bResumeDialogueAfterLoad;                                                    // 0x0060 (size: 0x1)
    TArray<UQuestState*> InheritableStates;                                           // 0x0068 (size: 0x10)
    EQuestCompletion QuestCompletion;                                                 // 0x0078 (size: 0x1)
    class UQuestState* QuestStartState;                                               // 0x0080 (size: 0x8)
    TArray<UQuestState*> States;                                                      // 0x0088 (size: 0x10)
    TArray<UQuestBranch*> Branches;                                                   // 0x0098 (size: 0x10)
    TArray<AActor*> QuestActors;                                                      // 0x00A8 (size: 0x10)
    TArray<UQuestRequirement*> QuestRequirements;                                     // 0x00B8 (size: 0x10)
    TArray<UQuestState*> ReachedStates;                                               // 0x00C8 (size: 0x10)
    class UNarrativeComponent* OwningComp;                                            // 0x00D8 (size: 0x8)
    class APawn* OwningPawn;                                                          // 0x00E0 (size: 0x8)
    class APlayerController* OwningController;                                        // 0x00E8 (size: 0x8)
    FQuestQuestBranchCompleted QuestBranchCompleted;                                  // 0x00F0 (size: 0x10)
    void OnQuestBranchCompleted(const class UQuest* Quest, const class UQuestBranch* Branch);
    FQuestQuestNewState QuestNewState;                                                // 0x0100 (size: 0x10)
    void OnQuestNewState(class UQuest* Quest, const class UQuestState* NewState);
    FQuestQuestTaskProgressChanged QuestTaskProgressChanged;                          // 0x0110 (size: 0x10)
    void OnQuestTaskProgressChanged(const class UQuest* Quest, const class UNarrativeTask* ProgressedTask, const class UQuestBranch* Branch, int32 OldProgress, int32 NewProgress);
    FQuestQuestTaskCompleted QuestTaskCompleted;                                      // 0x0120 (size: 0x10)
    void OnQuestTaskCompleted(const class UQuest* Quest, const class UNarrativeTask* CompletedTask, const class UQuestBranch* Branch);
    FQuestQuestSucceeded QuestSucceeded;                                              // 0x0130 (size: 0x10)
    void OnQuestSucceeded(const class UQuest* Quest, const FText& QuestSucceededMessage);
    FQuestQuestFailed QuestFailed;                                                    // 0x0140 (size: 0x10)
    void OnQuestFailed(const class UQuest* Quest, const FText& QuestFailedMessage);
    FQuestQuestStarted QuestStarted;                                                  // 0x0150 (size: 0x10)
    void OnQuestStarted(const class UQuest* Quest);
    FQuestOnQuestPostLoad OnQuestPostLoad;                                            // 0x0160 (size: 0x10)
    void OnQuestPostLoad(const class UQuest* Quest);
    FQuestQuestForgotten QuestForgotten;                                              // 0x0170 (size: 0x10)
    void OnQuestForgotten(const class UQuest* Quest);
    FQuestQuestRestarted QuestRestarted;                                              // 0x0180 (size: 0x10)
    void OnQuestRestarted(const class UQuest* Quest);

    void SucceedQuest(FText QuestSucceededMessage);
    class AActor* SpawnQuestActor(TSubclassOf<class AActor> ActorClass, const FTransform& ActorTransform);
    void SetTracked(const bool bNewTracked);
    void SetQuestName(const FText& NewName);
    void SetQuestDescription(const FText& NewDescription);
    void RemoveQuestRequirement(class UQuestRequirement* Requirement);
    void OnQuestTaskProgressChanged(const class UNarrativeTask* Task, const class UQuestBranch* Step, int32 CurrentProgress, int32 RequiredProgress);
    void OnQuestTaskCompleted(const class UNarrativeTask* Task, const class UQuestBranch* Branch);
    void OnQuestBranchCompleted(const class UQuestBranch* Branch);
    bool IsTracked();
    TArray<UQuestState*> GetStates();
    class UQuestState* GetState(FName ID);
    class UQuestState* GetQuestStartState();
    FText GetQuestName();
    FText GetQuestDescription();
    EQuestCompletion GetQuestCompletion();
    class APawn* GetOwningPawn();
    class APlayerController* GetOwningController();
    class UNarrativeComponent* GetOwningComp();
    TArray<UQuestNode*> GetNodes();
    TArray<APlayerController*> GetGroupMembers();
    TArray<UQuestBranch*> GetBranches();
    class UQuestBranch* GetBranch(FName ID);
    void FailQuest(FText QuestFailedMessage);
    void EnterState(class UQuestState* NewState);
    void BPQuestPostLoad();
    void BPPreQuestStarted(const class UQuest* Quest);
    void BPOnTrackedChanged(const class UQuest* Quest, const bool bNewTracked);
    void BPOnQuestTaskProgressChanged(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Step, int32 CurrentProgress, int32 RequiredProgress);
    void BPOnQuestTaskCompleted(const class UQuest* Quest, const class UNarrativeTask* Task, const class UQuestBranch* Step);
    void BPOnQuestSucceeded(const class UQuest* Quest, const FText& QuestSucceededMessage);
    void BPOnQuestStarted(const class UQuest* Quest);
    void BPOnQuestNewState(class UQuest* Quest, const class UQuestState* NewState);
    void BPOnQuestFailed(const class UQuest* Quest, const FText& QuestFailedMessage);
    void BPOnQuestBranchCompleted(const class UQuest* Quest, const class UQuestBranch* Branch);
    void AddQuestRequirement(class UQuestRequirement* Requirement);
}; // Size: 0x190

class UQuestBlueprintGeneratedClass : public UBlueprintGeneratedClass
{
    class UQuest* QuestTemplate;                                                      // 0x0360 (size: 0x8)

}; // Size: 0x368

class UQuestBranch : public UQuestNode
{
    TArray<UNarrativeTask*> QuestTasks;                                               // 0x0080 (size: 0x10)
    bool bHidden;                                                                     // 0x0090 (size: 0x1)
    class UQuestState* DestinationState;                                              // 0x0098 (size: 0x8)

}; // Size: 0xA0

class UQuestNode : public UNarrativeNodeBase
{
    FText Description;                                                                // 0x0060 (size: 0x10)
    FName OnEnteredFuncName;                                                          // 0x0070 (size: 0x8)
    class UQuest* OwningQuest;                                                        // 0x0078 (size: 0x8)

}; // Size: 0x80

class UQuestRequirement : public UObject
{

    void OnRemoved(class UQuest* Quest);
    void OnAdded(class UQuest* Quest);
    class UQuest* GetOwningQuest();
}; // Size: 0x28

class UQuestState : public UQuestNode
{
    TArray<UQuestBranch*> Branches;                                                   // 0x0080 (size: 0x10)
    EStateNodeType StateNodeType;                                                     // 0x0090 (size: 0x1)

}; // Size: 0x98

class UTaggedDialogueComponent : public UActorComponent
{
    TSoftObjectPtr<class UTaggedDialogueSet> TaggedDialogueSet;                       // 0x00B8 (size: 0x28)
    TMap<FGameplayTag, float> LastPlayTimes;                                          // 0x00E0 (size: 0x50)

    bool PlayTaggedDialogue(FGameplayTag Tag, class AActor* DialogueInstigator);
    bool ExecutePlayTaggedDialogue(FTaggedDialogue Dialogue, class AActor* DialogueInstigator);
}; // Size: 0x130

class UTaggedDialogueSet : public UDataAsset
{
    TArray<FTaggedDialogue> TaggedDialogues;                                          // 0x0030 (size: 0x10)

}; // Size: 0x40

#endif

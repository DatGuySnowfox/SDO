enum class EAnchorOriginRule {
    AOR_Disabled = 0,
    AOR_ConversationCenter = 1,
    AOR_Speaker = 2,
    AOR_Listener = 3,
    AOR_Custom = 4,
    AOR_MAX = 5,
};

enum class EAnchorRotationRule {
    ARR_AnchorActorForwardVector = 0,
    ARR_Conversation = 1,
    ARR_MAX = 2,
};

enum class EEventRuntime {
    Start = 0,
    End = 1,
    Both = 2,
    EEventRuntime_MAX = 3,
};

enum class ELineDuration {
    LD_Default = 0,
    LD_WhenAudioEnds = 1,
    LD_WhenSequenceEnds = 2,
    LD_AfterReadingTime = 3,
    LD_AfterDuration = 4,
    LD_Never = 5,
    LD_MAX = 6,
};

enum class EPartyConditionPolicy {
    AnyPlayerPasses = 0,
    PartyPasses = 1,
    AllPlayersPass = 2,
    PartyLeaderPasses = 3,
    EPartyConditionPolicy_MAX = 4,
};

enum class EPartyDialogueControlPolicy {
    PartyLeaderControlled = 0,
    AllPlayers = 1,
    EPartyDialogueControlPolicy_MAX = 2,
};

enum class EPartyEventPolicy {
    Party = 0,
    AllPartyMembers = 1,
    PartyLeader = 2,
    EPartyEventPolicy_MAX = 3,
};

enum class EQuestCompletion {
    QC_NotStarted = 0,
    QC_Started = 1,
    QC_Succeded = 2,
    QC_Failed = 3,
    QC_MAX = 4,
};

enum class EShotTrackingRule {
    STR_Disabled = 0,
    STR_Speaker = 1,
    STR_Listener = 2,
    STR_Custom = 3,
    STR_MAX = 4,
};

enum class EStateNodeType {
    Regular = 0,
    Success = 1,
    Failure = 2,
    EStateNodeType_MAX = 3,
};

enum class EUpdateType {
    UT_None = 0,
    UT_CompleteTask = 1,
    UT_BeginQuest = 2,
    UT_ForgetQuest = 3,
    UT_RestartQuest = 4,
    UT_QuestNewState = 5,
    UT_TaskProgressMade = 6,
    UT_MAX = 7,
};


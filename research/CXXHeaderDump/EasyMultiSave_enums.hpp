enum class EActorType {
    AT_Runtime = 0,
    AT_Placed = 1,
    AT_LevelScript = 2,
    AT_PlayerActor = 3,
    AT_PlayerPawn = 4,
    AT_GameObject = 5,
    AT_Persistent = 6,
    AT_Destroyed = 7,
    AT_MAX = 8,
};

enum class EAsyncCheckType {
    CT_Both = 0,
    CT_Save = 1,
    CT_Load = 2,
    CT_MAX = 3,
};

enum class EDataLoadType {
    DATA_Level = 0,
    DATA_Player = 1,
    DATA_Object = 2,
    DATA_Collection = 3,
    DATA_MAX = 4,
};

enum class EDeleteSlotType {
    Full = 0,
    Player = 1,
    Level = 2,
    EDeleteSlotType_MAX = 3,
};

enum class EFileSaveMethod {
    FM_Desktop = 0,
    FM_Console = 1,
    FM_MAX = 2,
};

enum class EFileValidity {
    FILE_VALID = 0,
    FILE_MISSING = 1,
    FILE_INVALID = 2,
    FILE_INCOMPATIBLE = 3,
    FILE_MAX = 4,
};

enum class EIntegrityCheckResult {
    Success = 0,
    VersionMismatch = 1,
    Invalid = 2,
    Unknown = 3,
    EIntegrityCheckResult_MAX = 4,
};

enum class ELoadMethod {
    LM_Default = 0,
    LM_Deferred = 1,
    LM_Thread = 2,
    LM_MAX = 3,
};

enum class ELoadedStateMod {
    NoModify = 0,
    Unloaded = 1,
    Loaded = 2,
    ELoadedStateMod_MAX = 3,
};

enum class EMultiLevelSaveMethod {
    ML_Disabled = 0,
    ML_Normal = 1,
    ML_Stream = 2,
    ML_Full = 3,
    ML_MAX = 4,
};

enum class ENextStepType {
    SaveLevel = 0,
    FinishSave = 1,
    ENextStepType_MAX = 2,
};

enum class EOldPackageEngine {
    EN_UE40 = 0,
    EN_UE50 = 1,
    EN_UE54 = 2,
    EN_MAX = 3,
};

enum class EPrepareType {
    PT_Default = 0,
    PT_FullReload = 1,
    PT_MAX = 2,
};

enum class EResetCustomSaveType {
    Full = 0,
    ClearMemory = 1,
    ResetDefault = 2,
    EResetCustomSaveType_MAX = 3,
};

enum class ESaveErrorType {
    ER_Player = 0,
    ER_Level = 1,
    ER_Object = 2,
    ER_Collection = 3,
    ER_MAX = 4,
};

enum class ESaveEventType {
    PreSave = 0,
    PostSave = 1,
    ESaveEventType_MAX = 2,
};

enum class ESaveFileCheckType {
    CheckForGame = 0,
    CheckForCustom = 1,
    CheckForCustomSlot = 2,
    CheckForSlotOnly = 3,
    ESaveFileCheckType_MAX = 4,
};

enum class ESaveGameMode {
    MODE_Player = 0,
    MODE_Level = 1,
    MODE_All = 2,
    MODE_MAX = 3,
};

enum class ESaveObjectType {
    SaveGameOnly = 0,
    CompleteObject = 1,
    ESaveObjectType_MAX = 2,
};

enum class ESaveTypeFlags {
    None = 0,
    SF_Player = 1,
    SF_Level = 2,
    ESaveTypeFlags_MAX = 3,
};

enum class EThumbnailImageFormat {
    Png = 0,
    Jpeg = 1,
    EThumbnailImageFormat_MAX = 2,
};

enum class EWorldPartitionInit {
    Default = 0,
    Skip = 1,
    EWorldPartitionInit_MAX = 2,
};

enum class EWorldPartitionMethod {
    Enabled = 0,
    MemoryOnly = 1,
    LoadOnly = 2,
    Disabled = 3,
    EWorldPartitionMethod_MAX = 4,
};


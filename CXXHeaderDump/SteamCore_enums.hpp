enum class EOnlineComparison {
    Equals = 0,
    NotEquals = 1,
    GreaterThan = 2,
    GreaterThanEquals = 3,
    LessThan = 4,
    LessThanEquals = 5,
    EOnlineComparison_MAX = 6,
};

enum class ESteamAccountType {
    Invalid = 0,
    Individual = 1,
    Multiseat = 2,
    GameServer = 3,
    AnonGameServer = 4,
    Pending = 5,
    ContentServer = 6,
    Clan = 7,
    Chat = 8,
    ConsoleUser = 9,
    AnonUser = 10,
    Max = 11,
};

enum class ESteamActivateGameOverlayToWebPageMode {
    Default = 0,
    Modal = 1,
    ESteamActivateGameOverlayToWebPageMode_MAX = 2,
};

enum class ESteamAttributeType {
    NOT_SET = 0,
    INTEGER = 1,
    STRING = 2,
    ESteamAttributeType_MAX = 3,
};

enum class ESteamAudioPlaybackStatus {
    Undefined = 0,
    Playing = 1,
    Paused = 2,
    Idle = 3,
    ESteamAudioPlaybackStatus_MAX = 4,
};

enum class ESteamAuthSessionResponse {
    OK = 0,
    UserNotConnectedToSteam = 1,
    NoLicenseOrExpired = 2,
    VACBanned = 3,
    LoggedInElseWhere = 4,
    VACCheckTimedOut = 5,
    AuthTicketCanceled = 6,
    AuthTicketInvalidAlreadyUsed = 7,
    AuthTicketInvalid = 8,
    PublisherIssuedBan = 9,
    ESteamAuthSessionResponse_MAX = 10,
};

enum class ESteamBeginAuthSessionResult {
    OK = 0,
    InvalidTicket = 1,
    DuplicateRequest = 2,
    InvalidVersion = 3,
    GameMismatch = 4,
    ExpiredTicket = 5,
    ESteamBeginAuthSessionResult_MAX = 6,
};

enum class ESteamBroadcastUploadResult {
    None = 0,
    OK = 1,
    InitFailed = 2,
    FrameFailed = 3,
    Timeout = 4,
    BandwidthExceeded = 5,
    LowFPS = 6,
    MissingKeyFrames = 7,
    NoConnection = 8,
    RelayFailed = 9,
    SettingsChanged = 10,
    MissingAudio = 11,
    TooFarBehind = 12,
    TranscodeBehind = 13,
    ESteamBroadcastUploadResult_MAX = 14,
};

enum class ESteamChatEntryType {
    Invalid = 0,
    ChatMsg = 1,
    Typing = 2,
    InviteGame = 3,
    Emote = 4,
    LeftConversation = 6,
    Entered = 7,
    WasKicked = 8,
    WasBanned = 9,
    Disconnected = 10,
    HistoricalChat = 11,
    LinkBlocked = 14,
    ESteamChatEntryType_MAX = 15,
};

enum class ESteamChatMemberStateChange {
    None = 0,
    Entered = 1,
    Left = 2,
    Disconnected = 4,
    Kicked = 8,
    Banned = 16,
    ESteamChatMemberStateChange_MAX = 17,
};

enum class ESteamChatRoomEnterResponse {
    None = 0,
    Success = 1,
    DoesntExist = 2,
    NotAllowed = 3,
    Full = 4,
    Error = 5,
    Banned = 6,
    Limited = 7,
    ClanDisabled = 8,
    CommunityBan = 9,
    MemberBlockedYou = 10,
    YouBlockedMember = 11,
    RatelimitExceeded = 15,
    ESteamChatRoomEnterResponse_MAX = 16,
};

enum class ESteamCheckFileSignature {
    InvalidSignature = 0,
    ValidSignature = 1,
    FileNotFound = 2,
    NoSignaturesFoundForThisApp = 3,
    NoSignaturesFoundForThisFile = 4,
    ESteamCheckFileSignature_MAX = 5,
};

enum class ESteamComparisonOp {
    Equals = 0,
    NotEquals = 1,
    GreaterThan = 2,
    GreaterThanEquals = 3,
    LessThan = 4,
    LessThanEquals = 5,
    Near = 6,
    In = 7,
    NotIn = 8,
    ESteamComparisonOp_MAX = 9,
};

enum class ESteamCoreControllerPad {
    Left = 0,
    Right = 1,
    ESteamCoreControllerPad_MAX = 2,
};

enum class ESteamCoreDeviceFormFactor {
    Unknown = 0,
    Phone = 1,
    Tablet = 2,
    Computer = 3,
    TV = 4,
    ESteamCoreDeviceFormFactor_MAX = 5,
};

enum class ESteamCoreIdentical {
    Identical = 0,
    NotIdentical = 1,
    ESteamCoreIdentical_MAX = 2,
};

enum class ESteamCoreInputLEDFlag {
    SetColor = 0,
    RestoreUserDefault = 1,
    ESteamCoreInputLEDFlag_MAX = 2,
};

enum class ESteamCoreInputSource {
    None = 0,
    LeftTrackpad = 1,
    RightTrackpad = 2,
    Joystick = 3,
    ABXY = 4,
    Switch = 5,
    LeftTrigger = 6,
    RightTrigger = 7,
    LeftBumper = 8,
    RightBumper = 9,
    Gyro = 10,
    CenterTrackpad = 11,
    RightJoystick = 12,
    DPad = 13,
    Key = 14,
    Mouse = 15,
    LeftGyro = 16,
    Count = 17,
    ESteamCoreInputSource_MAX = 18,
};

enum class ESteamCoreInputSourceMode {
    None = 0,
    Dpad = 1,
    Buttons = 2,
    FourButtons = 3,
    AbsoluteMouse = 4,
    RelativeMouse = 5,
    JoystickMove = 6,
    JoystickMouse = 7,
    JoystickCamera = 8,
    ScrollWheel = 9,
    Trigger = 10,
    TouchMenu = 11,
    MouseJoystick = 12,
    MouseRegion = 13,
    RadialMenu = 14,
    SingleButton = 15,
    Switches = 16,
    ESteamCoreInputSourceMode_MAX = 17,
};

enum class ESteamCoreInputType {
    k_ESteamInputType_Unknown = 0,
    k_ESteamInputType_SteamController = 1,
    k_ESteamInputType_XBox360Controller = 2,
    k_ESteamInputType_XBoxOneController = 3,
    k_ESteamInputType_GenericGamepad = 4,
    k_ESteamInputType_PS4Controller = 5,
    k_ESteamInputType_AppleMFiController = 6,
    k_ESteamInputType_AndroidController = 7,
    k_ESteamInputType_SwitchJoyConPair = 8,
    k_ESteamInputType_SwitchJoyConSingle = 9,
    k_ESteamInputType_SwitchProController = 10,
    k_ESteamInputType_MobileTouch = 11,
    k_ESteamInputType_PS3Controller = 12,
    k_ESteamInputType_PS5Controller = 13,
    k_ESteamInputType_SteamDeckController = 14,
    k_ESteamInputType_Count = 15,
    k_ESteamInputType_MaximumPossibleValue = 255,
    k_ESteamInputType_MAX = 256,
};

enum ESteamCoreItemFlags {
    ENoTrade = 0,
    ERemoved = 8,
    EConsumed = 9,
    ESteamCoreItemFlags_MAX = 10,
};

enum class ESteamCoreValid {
    Valid = 0,
    NotValid = 1,
    ESteamCoreValid_MAX = 2,
};

enum class ESteamCoreXboxOrigin {
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    LeftBumper = 4,
    RightBumper = 5,
    Menu = 6,
    View = 7,
    LeftTrigger_Pull = 8,
    LeftTrigger_Click = 9,
    RightTrigger_Pull = 10,
    RightTrigger_Click = 11,
    LeftStick_Move = 12,
    LeftStick_Click = 13,
    LeftStick_DPadNorth = 14,
    LeftStick_DPadSouth = 15,
    LeftStick_DPadWest = 16,
    LeftStick_DPadEast = 17,
    RightStick_Move = 18,
    RightStick_Click = 19,
    RightStick_DPadNorth = 20,
    RightStick_DPadSouth = 21,
    RightStick_DPadWest = 22,
    RightStick_DPadEast = 23,
    DPad_North = 24,
    DPad_South = 25,
    DPad_West = 26,
    DPad_East = 27,
    Count = 28,
    ESteamCoreXboxOrigin_MAX = 29,
};

enum class ESteamDenyReason {
    Invalid = 0,
    InvalidVersion = 1,
    Generic = 2,
    NotLoggedOn = 3,
    NoLicense = 4,
    Cheater = 5,
    LoggedInElseWhere = 6,
    UnknownText = 7,
    IncompatibleAnticheat = 8,
    MemoryCorruption = 9,
    IncompatibleSoftware = 10,
    SteamConnectionLost = 11,
    SteamConnectionError = 12,
    SteamResponseTimedOut = 13,
    SteamValidationStalled = 14,
    SteamOwnerLeftGuestUser = 15,
    ESteamDenyReason_MAX = 16,
};

enum class ESteamFailureType {
    FlushedCallbackQueue = 0,
    PipeFail = 1,
    ESteamFailureType_MAX = 2,
};

enum class ESteamFavoriteFlags {
    None = 0,
    Favorite = 1,
    History = 2,
    ESteamFavoriteFlags_MAX = 3,
};

enum class ESteamFriendFlags {
    None = 0,
    Blocked = 1,
    FriendshipRequested = 2,
    Immediate = 3,
    ClanMember = 4,
    OnGameServer = 5,
    RequestingFriendship = 6,
    RequestingInfo = 7,
    Ignored = 8,
    IgnoredFriend = 9,
    ChatMember = 10,
    All = 11,
    ESteamFriendFlags_MAX = 12,
};

enum class ESteamFriendRelationship {
    None = 0,
    Blocked = 1,
    RequestRecipient = 2,
    Friend = 3,
    RequestInitiator = 4,
    Ignored = 5,
    IgnoredFriend = 6,
    Suggested_DEPRECATED = 7,
    Max = 8,
};

enum class ESteamGameSearchErrorCode {
    Invalid = 0,
    OK = 1,
    Failed_Search_Already_In_Progress = 2,
    Failed_No_Search_In_Progress = 3,
    Failed_Not_Lobby_Leader = 4,
    Failed_No_Host_Available = 5,
    Failed_Search_Params_Invalid = 6,
    Failed_Offline = 7,
    Failed_NotAuthorized = 8,
    Failed_Unknown_Error = 9,
    ESteamGameSearchErrorCode_MAX = 10,
};

enum class ESteamGamepadTextInputLineMode {
    SingleLine = 0,
    MultipleLines = 1,
    ESteamGamepadTextInputLineMode_MAX = 2,
};

enum class ESteamGamepadTextInputMode {
    Normal = 0,
    Password = 1,
    ESteamGamepadTextInputMode_MAX = 2,
};

enum class ESteamItemPreviewType {
    Image = 0,
    YouTubeVideo = 1,
    Sketchfab = 2,
    EnvironmentMap_HorizontalCross = 3,
    EnvironmentMap_LatLong = 4,
    ReservedMax = 254,
    ESteamItemPreviewType_MAX = 255,
};

enum class ESteamItemState {
    None = 0,
    Subscribed = 1,
    LegacyItem = 2,
    Installed = 4,
    NeedsUpdate = 8,
    Downloading = 16,
    DownloadPending = 32,
    ESteamItemState_MAX = 33,
};

enum class ESteamItemStatistic {
    NumSubscriptions = 0,
    NumFavorites = 1,
    NumFollowers = 2,
    NumUniqueSubscriptions = 3,
    NumUniqueFavorites = 4,
    NumUniqueFollowers = 5,
    NumUniqueWebsiteViews = 6,
    ReportScore = 7,
    NumSecondsPlayed = 8,
    NumPlaytimeSessions = 9,
    NumComments = 10,
    NumSecondsPlayedDuringTimePeriod = 11,
    NumPlaytimeSessionsDuringTimePeriod = 12,
    ESteamItemStatistic_MAX = 13,
};

enum class ESteamItemUpdateStatus {
    Invalid = 0,
    PreparingConfig = 1,
    PreparingContent = 2,
    UploadingContent = 3,
    UploadingPreviewFile = 4,
    CommittingChanges = 5,
    ESteamItemUpdateStatus_MAX = 6,
};

enum class ESteamLeaderboardDataRequest {
    Global = 0,
    GlobalAroundUser = 1,
    Friends = 2,
    Users = 3,
    ESteamLeaderboardDataRequest_MAX = 4,
};

enum class ESteamLeaderboardDisplayType {
    None = 0,
    Numeric = 1,
    TimeSeconds = 2,
    TimeMilliSeconds = 3,
    ESteamLeaderboardDisplayType_MAX = 4,
};

enum class ESteamLeaderboardSortMethod {
    None = 0,
    Ascending = 1,
    Descending = 2,
    ESteamLeaderboardSortMethod_MAX = 3,
};

enum class ESteamLeaderboardUploadScoreMethod {
    None = 0,
    KeepBest = 1,
    ForceUpdate = 2,
    ESteamLeaderboardUploadScoreMethod_MAX = 3,
};

enum class ESteamLobbyComparison {
    EqualToOrLessThan = 0,
    LessThan = 1,
    Equal = 2,
    GreaterThan = 3,
    EqualToOrGreaterThan = 4,
    NotEqual = 5,
    ESteamLobbyComparison_MAX = 6,
};

enum class ESteamLobbyDistanceFilter {
    Close = 0,
    Default = 1,
    Far = 2,
    Worldwide = 3,
    ESteamLobbyDistanceFilter_MAX = 4,
};

enum class ESteamLobbyType {
    Private = 0,
    FriendsOnly = 1,
    Public = 2,
    Invisible = 3,
    ESteamLobbyType_MAX = 4,
};

enum class ESteamMessageType {
    ENotification = 0,
    EMessage = 1,
    EWarning = 2,
    ESteamMessageType_MAX = 3,
};

enum class ESteamNotificationPosition {
    TopLeft = 0,
    TopRight = 1,
    BottomLeft = 2,
    BottomRight = 3,
    ESteamNotificationPosition_MAX = 4,
};

enum class ESteamOverlayToStoreFlag {
    None = 0,
    AddToCart = 1,
    AddToCartAndShow = 2,
    ESteamOverlayToStoreFlag_MAX = 3,
};

enum class ESteamP2PSend {
    Unreliable = 0,
    UnreliableNoDelay = 1,
    Reliable = 2,
    ReliableWithBuffering = 3,
    ESteamP2PSend_MAX = 4,
};

enum class ESteamP2PSessionError {
    None = 0,
    NotRunningApp = 1,
    NoRightsToApp = 2,
    DestinationNotLoggedIn = 3,
    Timeout = 4,
    Max = 5,
};

enum class ESteamParentalFeature {
    Invalid = 0,
    Store = 1,
    Community = 2,
    Profile = 3,
    Friends = 4,
    News = 5,
    Trading = 6,
    Settings = 7,
    Console = 8,
    Browser = 9,
    ParentalSetup = 10,
    Library = 11,
    Test = 12,
    Max = 13,
};

enum class ESteamPartiesBeaconLocationData {
    Invalid = 0,
    Name = 1,
    IconURLSmall = 2,
    IconURLMedium = 3,
    IconURLLarge = 4,
    ESteamPartiesBeaconLocationData_MAX = 5,
};

enum class ESteamPartiesBeaconLocationType {
    Invalid = 0,
    ChatGroup = 1,
    Max = 2,
};

enum class ESteamPersonaChange {
    Name = 0,
    Status = 1,
    ComeOnline = 2,
    GoneOffline = 3,
    GamePlayed = 4,
    GameServer = 5,
    Avatar = 6,
    JoinedSource = 7,
    LeftSource = 8,
    RelationshipChanged = 9,
    NameFirstSet = 10,
    FacebookInfo = 11,
    Nickname = 12,
    SteamLevel = 13,
    ESteamPersonaChange_MAX = 14,
};

enum class ESteamPersonaState {
    Offline = 0,
    Online = 1,
    Busy = 2,
    Away = 3,
    Snooze = 4,
    LookingToTrade = 5,
    LookingToPlay = 6,
    Max = 7,
};

enum class ESteamPlayerAcceptState {
    Unknown = 0,
    PlayerAccepted = 1,
    PlayerDeclined = 2,
    ESteamPlayerAcceptState_MAX = 3,
};

enum class ESteamPlayerResult {
    Invalid = 0,
    FailedToConnect = 1,
    Abandoned = 2,
    Kicked = 3,
    Incomplete = 4,
    Completed = 5,
    ESteamPlayerResult_MAX = 6,
};

enum class ESteamRemoteStoragePlatform {
    None = 0,
    Windows = 1,
    OSX = 2,
    PS3 = 4,
    Linux = 8,
    Reserved2 = 16,
    All = 254,
    ESteamRemoteStoragePlatform_MAX = 255,
};

enum class ESteamRemoteStoragePublishedFileVisibility {
    Public = 0,
    FriendsOnly = 1,
    Private = 2,
    ESteamRemoteStoragePublishedFileVisibility_MAX = 3,
};

enum class ESteamResult {
    None = 0,
    OK = 1,
    Fail = 2,
    NoConnection = 3,
    sultNoConnectionRetry = 4,
    InvalidPassword = 5,
    LoggedInElsewhere = 6,
    InvalidProtocolVer = 7,
    InvalidParam = 8,
    FileNotFound = 9,
    Busy = 10,
    InvalidState = 11,
    InvalidName = 12,
    InvalidEmail = 13,
    DuplicateName = 14,
    AccessDenied = 15,
    Timeout = 16,
    Banned = 17,
    AccountNotFound = 18,
    InvalidSteamID = 19,
    ServiceUnavailable = 20,
    NotLoggedOn = 21,
    Pending = 22,
    EncryptionFailure = 23,
    InsufficientPrivilege = 24,
    LimitExceeded = 25,
    Revoked = 26,
    Expired = 27,
    AlreadyRedeemed = 28,
    DuplicateRequest = 29,
    AlreadyOwned = 30,
    IPNotFound = 31,
    PersistFailed = 32,
    LockingFailed = 33,
    LogonSessionReplaced = 34,
    ConnectFailed = 35,
    HandshakeFailed = 36,
    IOFailure = 37,
    RemoteDisconnect = 38,
    ShoppingCartNotFound = 39,
    Blocked = 40,
    Ignored = 41,
    NoMatch = 42,
    AccountDisabled = 43,
    ServiceReadOnly = 44,
    AccountNotFeatured = 45,
    AdministratorOK = 46,
    ContentVersion = 47,
    TryAnotherCM = 48,
    PasswordRequiredToKickSession = 49,
    AlreadyLoggedInElsewhere = 50,
    Suspended = 51,
    Cancelled = 52,
    DataCorruption = 53,
    DiskFull = 54,
    RemoteCallFailed = 55,
    PasswordUnset = 56,
    ExternalAccountUnlinked = 57,
    PSNTicketInvalid = 58,
    ExternalAccountAlreadyLinked = 59,
    RemoteFileConflict = 60,
    IllegalPassword = 61,
    SameAsPreviousValue = 62,
    AccountLogonDenied = 63,
    CannotUseOldPassword = 64,
    InvalidLoginAuthCode = 65,
    AccountLogonDeniedNoMail = 66,
    HardwareNotCapableOfIPT = 67,
    IPTInitError = 68,
    ParentalControlRestricted = 69,
    FacebookQueryError = 70,
    ExpiredLoginAuthCode = 71,
    IPLoginRestrictionFailed = 72,
    AccountLockedDown = 73,
    AccountLogonDeniedVerifiedEmailRequired = 74,
    NoMatchingURL = 75,
    BadResponse = 76,
    RequirePasswordReEntry = 77,
    ValueOutOfRange = 78,
    UnexpectedError = 79,
    Disabled = 80,
    InvalidCEGSubmission = 81,
    RestrictedDevice = 82,
    RegionLocked = 83,
    RateLimitExceeded = 84,
    AccountLoginDeniedNeedTwoFactor = 85,
    ItemDeleted = 86,
    AccountLoginDeniedThrottle = 87,
    TwoFactorCodeMismatch = 88,
    TwoFactorActivationCodeMismatch = 89,
    AccountAssociatedToMultiplePartners = 90,
    NotModified = 91,
    NoMobileDevice = 92,
    TimeNotSynced = 93,
    SmsCodeFailed = 94,
    AccountLimitExceeded = 95,
    AccountActivityLimitExceeded = 96,
    PhoneActivityLimitExceeded = 97,
    RefundToWallet = 98,
    EmailSendFailure = 99,
    NotSettled = 100,
    NeedCaptcha = 101,
    GSLTDenied = 102,
    GSOwnerDenied = 103,
    InvalidItemType = 104,
    IPBanned = 105,
    GSLTExpired = 106,
    InsufficientFunds = 107,
    TooManyPending = 108,
    NoSiteLicensesFound = 109,
    WGNetworkSendExceeded = 110,
    AccountNotFriends = 111,
    LimitedUserAccount = 112,
    CantRemoveItem = 113,
    ESteamResult_MAX = 114,
};

enum class ESteamSessionFindType {
    Listen = 0,
    Dedicated = 1,
    ESteamSessionFindType_MAX = 2,
};

enum class ESteamSubsystem {
    SteamCore = 0,
    AppList = 1,
    Apps = 2,
    Friends = 3,
    GameServer = 4,
    GameServerStats = 5,
    Inventory = 6,
    Input = 7,
    Matchmaking = 8,
    MatchmakingServers = 9,
    Music = 10,
    Networking = 11,
    NetworkingUtils = 12,
    ParentalSettings = 13,
    RemoteStorage = 14,
    RemotePlay = 15,
    Screenshots = 16,
    UGC = 17,
    User = 18,
    UserStats = 19,
    Utils = 20,
    Video = 21,
    SteamParties = 22,
    GameSearch = 23,
    ESteamSubsystem_MAX = 24,
};

enum class ESteamTextFilteringContext {
    k_ETextFilteringContextUnknown = 0,
    k_ETextFilteringContextGameContent = 1,
    k_ETextFilteringContextChat = 2,
    k_ETextFilteringContextName = 3,
    k_MAX = 4,
};

enum class ESteamUGCMatchingUGCType {
    Items = 0,
    Items_Mtx = 1,
    Items_ReadyToUse = 2,
    Collections = 3,
    Artwork = 4,
    Videos = 5,
    Screenshots = 6,
    AllGuides = 7,
    WebGuides = 8,
    IntegratedGuides = 9,
    UsableInGame = 10,
    ControllerBindings = 11,
    GameManagedItems = 12,
    All = 13,
    ESteamUGCMatchingUGCType_MAX = 14,
};

enum class ESteamUGCQuery {
    RankedByVote = 0,
    RankedByPublicationDate = 1,
    AcceptedForGameRankedByAcceptanceDate = 2,
    RankedByTrend = 3,
    FavoritedByFriendsRankedByPublicationDate = 4,
    CreatedByFriendsRankedByPublicationDate = 5,
    RankedByNumTimesReported = 6,
    CreatedByFollowedUsersRankedByPublicationDate = 7,
    NotYetRated = 8,
    RankedByTotalVotesAsc = 9,
    RankedByVotesUp = 10,
    RankedByTextSearch = 11,
    RankedByTotalUniqueSubscriptions = 12,
    RankedByPlaytimeTrend = 13,
    RankedByTotalPlaytime = 14,
    RankedByAveragePlaytimeTrend = 15,
    RankedByLifetimeAveragePlaytime = 16,
    RankedByPlaytimeSessionsTrend = 17,
    RankedByLifetimePlaytimeSessions = 18,
    ESteamUGCQuery_MAX = 19,
};

enum class ESteamUGCReadAction {
    k_EUGCRead_ContinueReadingUntilFinished = 0,
    k_EUGCRead_ContinueReading = 1,
    k_EUGCRead_Close = 2,
    k_EUGCRead_MAX = 3,
};

enum class ESteamUniverse {
    Invalid = 0,
    Public = 1,
    Beta = 2,
    Internal = 3,
    Dev = 4,
    Max = 5,
};

enum class ESteamUserHasLicenseForAppResult {
    HasLicense = 0,
    DoesNotHaveLicense = 1,
    NoAuth = 2,
    ESteamUserHasLicenseForAppResult_MAX = 3,
};

enum class ESteamUserRestriction {
    None = 0,
    Unknown = 1,
    AnyChat = 2,
    VoiceChat = 4,
    GroupChat = 8,
    Rating = 16,
    GameInvites = 32,
    Trading = 64,
    ESteamUserRestriction_MAX = 65,
};

enum class ESteamUserUGCList {
    Published = 0,
    VotedOn = 1,
    VotedUp = 2,
    VotedDown = 3,
    WillVoteLater = 4,
    Favorited = 5,
    Subscribed = 6,
    UsedOrPlayed = 7,
    Followed = 8,
    ESteamUserUGCList_MAX = 9,
};

enum class ESteamUserUGCListSortOrder {
    CreationOrderDesc = 0,
    CreationOrderAsc = 1,
    TitleAsc = 2,
    LastUpdatedDesc = 3,
    SubscriptionDateDesc = 4,
    VoteScoreDesc = 5,
    ForModeration = 6,
    ESteamUserUGCListSortOrder_MAX = 7,
};

enum class ESteamVRScreenshotType {
    None = 0,
    Mono = 1,
    Stereo = 2,
    MonoCubemap = 3,
    MonoPanorama = 4,
    StereoPanorama = 5,
    ESteamVRScreenshotType_MAX = 6,
};

enum class ESteamVoiceResult {
    OK = 0,
    NotInitialized = 1,
    NotRecording = 2,
    NoData = 3,
    BufferTooSmall = 4,
    DataCorrupted = 5,
    Restricted = 6,
    UnsupportedCodec = 7,
    ReceiverOutOfDate = 8,
    ReceiverDidNotAnswer = 9,
    ESteamVoiceResult_MAX = 10,
};

enum class ESteamWorkshopFileType {
    First = 0,
    Community = 0,
    Microtransaction = 1,
    Collection = 2,
    Art = 3,
    Video = 4,
    Screenshot = 5,
    Game = 6,
    Software = 7,
    Concept = 8,
    WebGuide = 9,
    IntegratedGuide = 10,
    Merch = 11,
    ControllerBinding = 12,
    SteamworksAccessInvite = 13,
    SteamVideo = 14,
    GameManagedItem = 15,
    Max = 16,
};


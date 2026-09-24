enum class EAutoMapChainType {
    Exact = 0,
    Fuzzy = 1,
    Clear = 2,
    EAutoMapChainType_MAX = 3,
};

enum class EBasicAxis {
    X = 0,
    Y = 1,
    Z = 2,
    NegX = 3,
    NegY = 4,
    NegZ = 5,
    EBasicAxis_MAX = 6,
};

enum class ECopyOpSettingsContext {
    PreInitialize = 0,
    Runtime = 1,
    ECopyOpSettingsContext_MAX = 2,
};

enum class EFKChainRotationMode {
    Interpolated = 0,
    OneToOne = 1,
    OneToOneReversed = 2,
    MatchChain = 3,
    MatchScaledChain = 4,
    None = 5,
    EFKChainRotationMode_MAX = 6,
};

enum class EFKChainTranslationMode {
    None = 0,
    GloballyScaled = 1,
    Absolute = 2,
    StretchBoneLengthUniformly = 3,
    StretchBoneLengthNonUniformly = 4,
    EFKChainTranslationMode_MAX = 5,
};

enum class EIKRigGoalSpace {
    Component = 0,
    Additive = 1,
    World = 2,
    EIKRigGoalSpace_MAX = 3,
};

enum class EIKRigGoalTransformSource {
    Manual = 0,
    Bone = 1,
    ActorComponent = 2,
    EIKRigGoalTransformSource_MAX = 3,
};

enum class EPinBoneRotationMode {
    CopyGlobalRotation = 0,
    MaintainOffsetFromBoneToCopyFrom = 1,
    EPinBoneRotationMode_MAX = 2,
};

enum class EPinBoneTranslationMode {
    CopyGlobalPosition = 0,
    CopyLocalPosition = 1,
    CopyLocalPositionRelativeOffset = 2,
    CopyLocalPositionRelativeScaled = 3,
    CopyGlobalPositionAndMaintainOffset = 4,
    EPinBoneTranslationMode_MAX = 5,
};

enum class EPinBoneType {
    FullTransform = 0,
    TranslateOnly = 1,
    RotateOnly = 2,
    ScaleOnly = 3,
    EPinBoneType_MAX = 4,
};

enum class ERetargetRotationMode {
    Interpolated = 0,
    OneToOne = 1,
    OneToOneReversed = 2,
    MatchChain = 3,
    MatchScaledChain = 4,
    None = 5,
    ERetargetRotationMode_MAX = 6,
};

enum class ERetargetSourceMode {
    ParentSkeletalMeshComponent = 0,
    CustomSkeletalMeshComponent = 1,
    SourcePosePin = 2,
    ERetargetSourceMode_MAX = 3,
};

enum class ERetargetSourceOrTarget {
    Source = 0,
    Target = 1,
    ERetargetSourceOrTarget_MAX = 2,
};

enum class ERetargetTranslationMode {
    None = 0,
    GloballyScaled = 1,
    Absolute = 2,
    StretchBoneLengthUniformly = 3,
    StretchBoneLengthNonUniformly = 4,
    ERetargetTranslationMode_MAX = 5,
};

enum class ERootMotionHeightSource {
    CopyHeightFromSource = 0,
    SnapToGround = 1,
    ERootMotionHeightSource_MAX = 2,
};

enum class ERootMotionSource {
    CopyFromSourceRoot = 0,
    GenerateFromTargetPelvis = 1,
    ERootMotionSource_MAX = 2,
};

enum class EWarpingDirectionSource {
    Goals = 0,
    Chain = 1,
    RootBone = 2,
    EWarpingDirectionSource_MAX = 3,
};


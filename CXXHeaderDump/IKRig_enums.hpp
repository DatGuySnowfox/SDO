enum class EBasicAxis {
    X = 0,
    Y = 1,
    Z = 2,
    NegX = 3,
    NegY = 4,
    NegZ = 5,
    EBasicAxis_MAX = 6,
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

enum class ERetargetRotationMode {
    Interpolated = 0,
    OneToOne = 1,
    OneToOneReversed = 2,
    None = 3,
    ERetargetRotationMode_MAX = 4,
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
    ERetargetTranslationMode_MAX = 3,
};

enum class EWarpingDirectionSource {
    Goals = 0,
    Chain = 1,
    EWarpingDirectionSource_MAX = 2,
};


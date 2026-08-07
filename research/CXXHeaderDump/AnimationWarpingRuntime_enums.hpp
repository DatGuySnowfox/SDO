enum class EActorMovementCompensationMode {
    ComponentSpace = 0,
    WorldSpace = 1,
    SuddenMotionOnly = 2,
    EActorMovementCompensationMode_MAX = 3,
};

enum class EFootPlacementLockType {
    Unlocked = 0,
    PivotAroundBall = 1,
    PivotAroundAnkle = 2,
    LockRotation = 3,
    EFootPlacementLockType_MAX = 4,
};

enum class EOffsetRootBoneMode {
    Accumulate = 0,
    Interpolate = 1,
    Hold = 2,
    Release = 3,
    EOffsetRootBoneMode_MAX = 4,
};

enum class EPelvisHeightMode {
    AllLegs = 0,
    AllPlantedFeet = 1,
    FrontPlantedFeetUphill_FrontFeetDownhill = 2,
    EPelvisHeightMode_MAX = 3,
};


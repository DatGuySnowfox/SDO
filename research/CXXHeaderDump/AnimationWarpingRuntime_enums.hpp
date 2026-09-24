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
    LockOffsetAndConsumeAnimation = 2,
    LockOffsetIncreaseAndConsumeAnimation = 3,
    LockOffsetAndIgnoreAnimation = 4,
    Release = 5,
    EOffsetRootBoneMode_MAX = 6,
};

enum class EOffsetRootBone_CollisionTestingMode {
    Disabled = 0,
    ShrinkMaxTranslation = 1,
    PlanarCollision = 2,
    EOffsetRootBone_MAX = 3,
};

enum class EOrientationWarpingSpace {
    ComponentTransform = 0,
    RootBoneTransform = 1,
    CustomTransform = 2,
    EOrientationWarpingSpace_MAX = 3,
};

enum class EPelvisHeightMode {
    AllLegs = 0,
    AllPlantedFeet = 1,
    FrontPlantedFeetUphill_FrontFeetDownhill = 2,
    EPelvisHeightMode_MAX = 3,
};


enum class EClothMassMode {
    UniformMass = 0,
    TotalMass = 1,
    Density = 2,
    MaxClothMassMode = 3,
    EClothMassMode_MAX = 4,
};

enum class EClothingWindMethod_Legacy {
    Legacy = 0,
    Accurate = 1,
    EClothingWindMethod_MAX = 2,
};

enum class EWeightMapTargetCommon {
    None = 0,
    MaxDistance = 1,
    BackstopDistance = 2,
    BackstopRadius = 3,
    AnimDriveStiffness = 4,
    AnimDriveDamping_DEPRECATED = 5,
    FirstUserTarget = 6,
    LastUserTarget = 200,
    TetherEndsMask = 201,
    EWeightMapTargetCommon_MAX = 202,
};


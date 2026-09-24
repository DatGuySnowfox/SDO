enum class EChaosVDAABBTreeNodeFlags {
    None = 0,
    IsLeaf = 1,
    IsDirty = 2,
    EChaosVDAABBTreeNodeFlags_MAX = 3,
};

enum class EChaosVDAccelerationStructureType {
    BoundingVolume = 0,
    AABBTree = 1,
    AABBTreeBV = 2,
    Collection = 3,
    Unknown = 4,
    EChaosVDAccelerationStructureType_MAX = 5,
};

enum class EChaosVDCollisionQueryHitType {
    None = 0,
    Touch = 1,
    Block = 2,
    EChaosVDCollisionQueryHitType_MAX = 3,
};

enum class EChaosVDCollisionQueryParamsFlags {
    None = 0,
    TraceComplex = 1,
    FindInitialOverlaps = 2,
    ReturnFaceIndex = 4,
    ReturnPhysicalMaterial = 8,
    IgnoreBlocks = 16,
    IgnoreTouches = 32,
    SkipNarrowPhase = 64,
    TraceIntoSubComponents = 128,
    ReplaceHitWithSubComponents = 256,
    EChaosVDCollisionQueryParamsFlags_MAX = 257,
};

enum class EChaosVDCollisionShapeDataFlags {
    None = 0,
    SimCollision = 1,
    QueryCollision = 2,
    IsProbe = 4,
    EChaosVDCollisionShapeDataFlags_MAX = 5,
};

enum class EChaosVDCollisionTraceFlag {
    UseDefault = 0,
    UseSimpleAndComplex = 1,
    UseSimpleAsComplex = 2,
    UseComplexAsSimple = 3,
    MAX = 4,
};

enum class EChaosVDConstraintFlags {
    None = 0,
    IsCurrent = 1,
    Disabled = 2,
    UseManifold = 4,
    UseIncrementalManifold = 8,
    CanRestoreManifold = 16,
    WasManifoldRestored = 32,
    IsQuadratic0 = 64,
    IsQuadratic1 = 128,
    IsProbe = 256,
    CCDEnabled = 512,
    CCDSweepEnabled = 1024,
    ModifierApplied = 2048,
    MaterialSet = 4096,
    EChaosVDConstraintFlags_MAX = 4097,
};

enum class EChaosVDContactPointType {
    Unknown = 0,
    VertexPlane = 1,
    EdgeEdge = 2,
    PlaneVertex = 3,
    VertexVertex = 4,
    EChaosVDContactPointType_MAX = 5,
};

enum class EChaosVDContactShapesType {
    Unknown = 0,
    SphereSphere = 1,
    SphereCapsule = 2,
    SphereBox = 3,
    SphereConvex = 4,
    SphereTriMesh = 5,
    SphereHeightField = 6,
    SpherePlane = 7,
    CapsuleCapsule = 8,
    CapsuleBox = 9,
    CapsuleConvex = 10,
    CapsuleTriMesh = 11,
    CapsuleHeightField = 12,
    BoxBox = 13,
    BoxConvex = 14,
    BoxTriMesh = 15,
    BoxHeightField = 16,
    BoxPlane = 17,
    ConvexConvex = 18,
    ConvexTriMesh = 19,
    ConvexHeightField = 20,
    GenericConvexConvex = 21,
    LevelSetLevelSet = 22,
    NumShapesTypes = 23,
    EChaosVDContactShapesType_MAX = 24,
};

enum class EChaosVDGTJointStateFlags {
    None = 0,
    IsBreaking = 1,
    IsBroken = 2,
    DriveTargetChanged = 4,
    IsViolating = 8,
    EChaosVDGTJointStateFlags_MAX = 9,
};

enum class EChaosVDJointForceMode {
    Acceleration = 0,
    Force = 1,
    EChaosVDJointForceMode_MAX = 2,
};

enum class EChaosVDJointMotionType {
    Free = 0,
    Limited = 1,
    Locked = 2,
    EChaosVDJointMotionType_MAX = 3,
};

enum class EChaosVDJointReSimType {
    FullResim = 0,
    ResimAsFollower = 1,
    EChaosVDJointReSimType_MAX = 2,
};

enum class EChaosVDJointSettingsFlags {
    None = 0,
    CollisionEnabled = 1,
    MassConditioningEnabled = 2,
    AngularSLerpPositionDriveEnabled = 4,
    AngularSLerpVelocityDriveEnabled = 8,
    AngularTwistPositionDriveEnabled = 16,
    AngularTwistVelocityDriveEnabled = 32,
    AngularSwingPositionDriveEnabled = 64,
    AngularSwingVelocityDriveEnabled = 128,
    SoftLinearLimitsEnabled = 256,
    SoftTwistLimitsEnabled = 512,
    SoftSwingLimitsEnabled = 1024,
    LinearPositionDriveEnabled0 = 2048,
    LinearPositionDriveEnable1 = 4096,
    LinearPositionDriveEnable2 = 8192,
    LinearVelocityDriveEnabled0 = 16384,
    LinearVelocityDriveEnabled1 = 32768,
    LinearVelocityDriveEnabled2 = 65536,
    UseLinearSolver = 131072,
    EChaosVDJointSettingsFlags_MAX = 131073,
};

enum class EChaosVDJointSolverSettingsFlags {
    None = 0,
    UseLinearSolver = 1,
    SortEnabled = 2,
    SolvePositionLast = 4,
    UsePositionBasedDrives = 8,
    EnableTwistLimits = 16,
    EnableSwingLimits = 32,
    EnableDrives = 64,
    EChaosVDJointSolverSettingsFlags_MAX = 65,
};

enum class EChaosVDJointStateFlags {
    None = 0,
    Disabled = 1,
    Broken = 2,
    Breaking = 4,
    DriveTargetChanged = 8,
    EnabledDuringResim = 16,
    EChaosVDJointStateFlags_MAX = 17,
};

enum class EChaosVDJointSyncType {
    InSync = 0,
    HardDesync = 1,
    EChaosVDJointSyncType_MAX = 2,
};

enum class EChaosVDKinematicTargetMode {
    None = 0,
    Reset = 1,
    Position = 2,
    Velocity = 3,
    EChaosVDKinematicTargetMode_MAX = 4,
};

enum class EChaosVDManifoldPointFlags {
    None = 0,
    Disabled = 1,
    WasRestored = 2,
    WasReplaced = 4,
    HasStaticFrictionAnchor = 8,
    IsValid = 16,
    InsideStaticFrictionCone = 32,
    EChaosVDManifoldPointFlags_MAX = 33,
};

enum class EChaosVDMidPhaseFlags {
    None = 0,
    IsActive = 1,
    IsCCD = 2,
    IsCCDActive = 4,
    IsSleeping = 8,
    IsModified = 16,
    EChaosVDMidPhaseFlags_MAX = 17,
};

enum class EChaosVDMidPhaseType {
    Generic = 0,
    ShapePair = 1,
    SphereApproximation = 2,
    Unknown = 3,
    EChaosVDMidPhaseType_MAX = 4,
};

enum class EChaosVDObjectStateType {
    Uninitialized = 0,
    Sleeping = 1,
    Kinematic = 2,
    Static = 3,
    Dynamic = 4,
    Count = 5,
    EChaosVDObjectStateType_MAX = 6,
};

enum class EChaosVDParticleContext {
    Invalid = 0,
    GameThread = 1,
    PhysicsThread = 2,
    EChaosVDParticleContext_MAX = 3,
};

enum class EChaosVDParticleType {
    Static = 0,
    Kinematic = 1,
    Rigid = 2,
    Clustered = 3,
    StaticMesh = 4,
    SkeletalMesh = 5,
    GeometryCollection = 6,
    Unknown = 7,
    EChaosVDParticleType_MAX = 8,
};

enum class EChaosVDPlasticityType {
    Free = 0,
    Shrink = 1,
    Grow = 2,
    EChaosVDPlasticityType_MAX = 3,
};

enum class EChaosVDQueryFastDataParallelFlags {
    None = 0,
    Parallel0 = 1,
    Parallel1 = 2,
    Parallel2 = 4,
    EChaosVDQueryFastDataParallelFlags_MAX = 5,
};

enum class EChaosVDRecordingMode {
    Invalid = 0,
    Live = 1,
    File = 2,
    EChaosVDRecordingMode_MAX = 3,
};

enum class EChaosVDSQHitFlags {
    None = 0,
    Position = 1,
    Normal = 2,
    Distance = 4,
    UV = 8,
    MTD = 512,
    FaceIndex = 1024,
    EChaosVDSQHitFlags_MAX = 1025,
};

enum class EChaosVDSQVisitRejectReason {
    None = 0,
    NoHit = 1,
    PreFilter = 2,
    PostFilter = 3,
    ColocatedHitHasWorseNormal = 4,
    FailedFastBoundTest = 5,
    EChaosVDSQVisitRejectReason_MAX = 6,
};

enum class EChaosVDSceneQueryMode {
    Single = 0,
    Multi = 1,
    Test = 2,
    Invalid = 3,
    EChaosVDSceneQueryMode_MAX = 4,
};

enum class EChaosVDSceneQueryType {
    Invalid = 0,
    Sweep = 1,
    Overlap = 2,
    RayCast = 3,
    EChaosVDSceneQueryType_MAX = 4,
};

enum class EChaosVDSceneQueryVisitorType {
    Invalid = 0,
    BroadPhase = 1,
    NarrowPhase = 2,
    EChaosVDSceneQueryVisitorType_MAX = 3,
};

enum class EChaosVDSleepType {
    MaterialSleep = 0,
    NeverSleep = 1,
    EChaosVDSleepType_MAX = 2,
};


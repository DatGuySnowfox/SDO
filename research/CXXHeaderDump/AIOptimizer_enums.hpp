enum class EAIODebugGroup {
    Undefined = 0,
    Spawned = 1,
    Despawned = 2,
    PendingSpawn = 3,
    PendingDespawn = 4,
    SpawnedClose = 5,
    SpawnedMedium = 6,
    SpawnedFar = 7,
    NotUpdated = 8,
    EAIODebugGroup_MAX = 9,
};

enum class EAIOFeaturesFlags {
    AIBrain = 0,
    MovementComponent = 1,
    Visibility = 2,
    Collision = 3,
    Animations = 4,
    ActorTick = 5,
    Shadows = 6,
    EAIOFeaturesFlags_MAX = 7,
};

enum class EAIORespawnMethod {
    Undefined = 0,
    AllAtOnce = 1,
    EachIndividually = 2,
    EAIORespawnMethod_MAX = 3,
};

enum class EAIOSelectSpawnPointsMethod {
    UseRandomPoints = 0,
    UseSpecifiedSpawnPoints = 1,
    EAIOSelectSpawnPointsMethod_MAX = 2,
};

enum class EAIOSpawnPointsProjectionMethod {
    None = 0,
    Geometry = 1,
    Navigation = 2,
    EAIOSpawnPointsProjectionMethod_MAX = 3,
};

enum class EAIOStartSpawningMethod {
    None = 0,
    SpawnOnGameStart = 1,
    SpawnOnRadius = 2,
    SpawnOnRegion = 3,
    EAIOStartSpawningMethod_MAX = 4,
};

enum class EDespawnMethod {
    UseQueue = 0,
    Immediately = 1,
    EDespawnMethod_MAX = 2,
};


enum class EGeometryFlow_ComputeNormalsType {
    PerTriangle = 0,
    PerVertex = 1,
    RecomputeExistingTopology = 2,
    FromFaceAngleThreshold = 3,
    FromGroups = 4,
    EGeometryFlow_MAX = 5,
};

enum class EGeometryFlow_ComputeTangentsType {
    PerTriangle = 0,
    FastMikkT = 1,
    EGeometryFlow_MAX = 2,
};

enum class EGeometryFlow_EdgeRefineFlags {
    NoConstraint = 0,
    NoFlip = 1,
    NoSplit = 2,
    NoCollapse = 4,
    FullyConstrained = 7,
    SplitsOnly = 5,
    FlipOnly = 6,
    CollapseOnly = 3,
    EGeometryFlow_MAX = 8,
};

enum class EGeometryFlow_MeshSimplifyType {
    Standard = 0,
    VolumePreserving = 1,
    AttributeAware = 2,
    EGeometryFlow_MAX = 3,
};

enum class EGeometryFlow_ProjectedHullAxisMode {
    X = 0,
    Y = 1,
    Z = 2,
    SmallestBoxDimension = 3,
    SmallestVolume = 4,
    EGeometryFlow_MAX = 5,
};

enum class EGeometryFlow_RecalculateUVsUnwrapType {
    Auto = 0,
    ExpMap = 1,
    Conformal = 2,
    EGeometryFlow_MAX = 3,
};

enum class EGeometryFlow_SimpleCollisionGeometryType {
    AlignedBoxes = 0,
    OrientedBoxes = 1,
    MinimalSpheres = 2,
    Capsules = 3,
    ConvexHulls = 4,
    SweptHulls = 5,
    MinVolume = 6,
    None = 7,
    EGeometryFlow_MAX = 8,
};

enum class EGeometryFlow_SmoothTypes {
    Uniform = 0,
    Cotan = 1,
    MeanValue = 2,
    EGeometryFlow_MAX = 3,
};

enum class EGeomtryFlow_MeshSimplifyTargetType {
    TriangleCount = 0,
    VertexCount = 1,
    TrianglePercentage = 2,
    GeometricDeviation = 3,
    EGeomtryFlow_MAX = 4,
};


enum class EFunctionType {
    LinearFunction = 0,
    SquaredFunction = 1,
    CustomCurve = 2,
    EFunctionType_MAX = 3,
};

enum class EModuleInputValueType {
    MBoolean = 0,
    MAxis1D = 1,
    MAxis2D = 2,
    MAxis3D = 3,
    MInteger = 4,
    EModuleInputValueType_MAX = 5,
};

enum ESimTreeProcessingOrder {
    ManualOverride = 0,
    LeafFirst = 1,
    RootFirst = 2,
    LeafFirstBFS = 3,
    ESimTreeProcessingOrder_MAX = 4,
};


enum class EAxleType {
    Undefined = 0,
    Front = 1,
    Rear = 2,
    EAxleType_MAX = 3,
};

enum class EInputFunctionType {
    LinearFunction = 0,
    SquaredFunction = 1,
    CustomCurve = 2,
    EInputFunctionType_MAX = 3,
};

enum class ESteeringType {
    SingleAngle = 0,
    AngleRatio = 1,
    Ackermann = 2,
    ESteeringType_MAX = 3,
};

enum class ESweepShape {
    Raycast = 0,
    Spherecast = 1,
    Shapecast = 2,
    ESweepShape_MAX = 3,
};

enum class ESweepType {
    SimpleSweep = 0,
    ComplexSweep = 1,
    ESweepType_MAX = 2,
};

enum class ETorqueCombineMethod {
    None = 0,
    Override = 1,
    Additive = 2,
    ETorqueCombineMethod_MAX = 3,
};

enum class EVehicleAerofoilType {
    Fixed = 0,
    Wing = 1,
    Rudder = 2,
    Elevator = 3,
    EVehicleAerofoilType_MAX = 4,
};

enum class EVehicleDifferential {
    Undefined = 0,
    AllWheelDrive = 1,
    FrontWheelDrive = 2,
    RearWheelDrive = 3,
    EVehicleDifferential_MAX = 4,
};

enum class EVehicleThrustType {
    Fixed = 0,
    Wing = 1,
    Rudder = 2,
    Elevator = 3,
    EVehicleThrustType_MAX = 4,
};


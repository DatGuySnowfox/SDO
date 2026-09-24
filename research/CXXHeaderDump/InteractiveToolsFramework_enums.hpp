enum class EGizmoElementArrowHeadType {
    Cone = 0,
    Cube = 1,
    EGizmoElementArrowHeadType_MAX = 2,
};

enum class EGizmoElementDrawType {
    None = 0,
    Fill = 2,
    Line = 4,
    FillAndLine = 6,
    EGizmoElementDrawType_MAX = 7,
};

enum class EGizmoElementInteractionState {
    None = 0,
    Hovering = 1,
    Interacting = 2,
    EGizmoElementInteractionState_MAX = 3,
};

enum class EGizmoElementPartialType {
    None = 0,
    Partial = 1,
    PartialViewDependent = 2,
    EGizmoElementPartialType_MAX = 3,
};

enum class EGizmoElementState {
    None = 0,
    Visible = 2,
    Hittable = 4,
    VisibleAndHittable = 6,
    EGizmoElementState_MAX = 7,
};

enum class EGizmoElementViewAlignType {
    None = 0,
    PointOnly = 1,
    PointEye = 2,
    PointScreen = 3,
    Axial = 4,
    EGizmoElementViewAlignType_MAX = 5,
};

enum class EGizmoElementViewDependentType {
    None = 0,
    Axis = 1,
    Plane = 2,
    EGizmoElementViewDependentType_MAX = 3,
};

enum class EInputCaptureRequestType {
    Begin = 1,
    Ignore = 2,
    EInputCaptureRequestType_MAX = 3,
};

enum class EInputCaptureSide {
    None = 0,
    Left = 1,
    Right = 2,
    Both = 3,
    Any = 99,
    EInputCaptureSide_MAX = 100,
};

enum class EInputCaptureState {
    Begin = 1,
    Continue = 2,
    End = 3,
    Ignore = 4,
    EInputCaptureState_MAX = 5,
};

enum class EInputDevices {
    None = 0,
    Keyboard = 1,
    Mouse = 2,
    Gamepad = 4,
    OculusTouch = 8,
    HTCViveWands = 16,
    AnySpatialDevice = 24,
    TabletFingers = 1024,
    EInputDevices_MAX = 1025,
};

enum class ESceneSnapQueryTargetResult {
    NotSnapped = 0,
    Snapped = 1,
    Disabled = 2,
    Unsupported = 3,
    ESceneSnapQueryTargetResult_MAX = 4,
};

enum class ESceneSnapQueryTargetType {
    None = 0,
    MeshVertex = 1,
    MeshEdge = 2,
    Grid = 4,
    ObjectTransform = 8,
    Custom = 16,
    All = 31,
    ESceneSnapQueryTargetType_MAX = 32,
};

enum class ESceneSnapQueryType {
    Position = 1,
    Rotation = 2,
    RotationAngle = 3,
    Scale = 4,
    Transform = 5,
    ESceneSnapQueryType_MAX = 6,
};

enum class ESelectedObjectsModificationType {
    Replace = 0,
    Add = 1,
    Remove = 2,
    Clear = 3,
    ESelectedObjectsModificationType_MAX = 4,
};

enum class EStandardToolContextMaterials {
    VertexColorMaterial = 1,
    EStandardToolContextMaterials_MAX = 2,
};

enum class EToolChangeTrackingMode {
    NoChangeTracking = 1,
    UndoToExit = 2,
    FullUndoRedo = 3,
    EToolChangeTrackingMode_MAX = 4,
};

enum class EToolContextCoordinateSystem {
    World = 0,
    Local = 1,
    Screen = 2,
    EToolContextCoordinateSystem_MAX = 3,
};

enum class EToolContextTransformGizmoMode {
    NoGizmo = 0,
    Translation = 1,
    Rotation = 2,
    Scale = 3,
    Combined = 8,
    EToolContextTransformGizmoMode_MAX = 9,
};

enum class EToolManagerToolSwitchMode {
    AcceptIfAble = 0,
    CancelIfAble = 1,
    CustomizableAcceptIfAble = 2,
    CustomizableCancelIfAble = 3,
    EToolManagerToolSwitchMode_MAX = 4,
};

enum class EToolMessageLevel {
    Internal = 0,
    UserMessage = 1,
    UserNotification = 2,
    UserWarning = 3,
    UserError = 4,
    EToolMessageLevel_MAX = 5,
};

enum class EToolShutdownType {
    Completed = 0,
    Accept = 1,
    Cancel = 2,
    EToolShutdownType_MAX = 3,
};

enum class EToolSide {
    Left = 1,
    Mouse = 1,
    Right = 2,
    EToolSide_MAX = 3,
};

enum class ETransformGizmoSubElements {
    None = 0,
    TranslateAxisX = 2,
    TranslateAxisY = 4,
    TranslateAxisZ = 8,
    TranslateAllAxes = 14,
    TranslatePlaneXY = 16,
    TranslatePlaneXZ = 32,
    TranslatePlaneYZ = 64,
    TranslateAllPlanes = 112,
    RotateAxisX = 128,
    RotateAxisY = 256,
    RotateAxisZ = 512,
    RotateAllAxes = 896,
    ScaleAxisX = 1024,
    ScaleAxisY = 2048,
    ScaleAxisZ = 4096,
    ScaleAllAxes = 7168,
    ScalePlaneYZ = 8192,
    ScalePlaneXZ = 16384,
    ScalePlaneXY = 32768,
    ScaleAllPlanes = 57344,
    ScaleUniform = 65536,
    FreeTranslate = 131072,
    FreeRotate = 262144,
    StandardTranslateRotate = 394238,
    TranslateRotateUniformScale = 459774,
    FullTranslateRotateScale = 524286,
    ETransformGizmoSubElements_MAX = 524287,
};

enum class EViewInteractionState {
    None = 0,
    Hovered = 1,
    Focused = 2,
    EViewInteractionState_MAX = 3,
};


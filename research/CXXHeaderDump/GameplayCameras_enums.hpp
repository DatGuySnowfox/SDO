enum class EBuiltInDoubleCameraVariable {
    None = 0,
    Yaw = 1,
    Pitch = 2,
    Roll = 3,
    Zoom = 4,
    EBuiltInDoubleCameraVariable_MAX = 5,
};

enum class EBuiltInRotator3dCameraVariable {
    None = 0,
    ControlRotation = 1,
    EBuiltInRotator3dCameraVariable_MAX = 2,
};

enum class EBuiltInVector2dCameraVariable {
    None = 0,
    YawPitch = 1,
    EBuiltInVector2dCameraVariable_MAX = 2,
};

enum class ECameraAutoRotateDirection {
    Facing = 0,
    Movement = 1,
    MovementOrFacing = 2,
    ECameraAutoRotateDirection_MAX = 3,
};

enum class ECameraBlendStackType {
    IsolatedTransient = 0,
    AdditivePersistent = 1,
    ECameraBlendStackType_MAX = 2,
};

enum class ECameraBuildStatus {
    Clean = 0,
    CleanWithWarnings = 1,
    WithErrors = 2,
    Dirty = 3,
    ECameraBuildStatus_MAX = 4,
};

enum class ECameraContextDataContainerType {
    None = 0,
    Array = 1,
    ECameraContextDataContainerType_MAX = 2,
};

enum class ECameraContextDataType {
    Name = 0,
    String = 1,
    Enum = 2,
    Struct = 3,
    Object = 4,
    Class = 5,
    Count = 6,
    ECameraContextDataType_MAX = 7,
};

enum class ECameraEvaluationDataCondition {
    ActiveCameraRig = 0,
    ECameraEvaluationDataCondition_MAX = 1,
};

enum class ECameraNodeOriginPosition {
    CameraPose = 0,
    ActiveContext = 1,
    OwningContext = 2,
    Pivot = 3,
    Pawn = 4,
    ECameraNodeOriginPosition_MAX = 5,
};

enum class ECameraNodeSpace {
    CameraPose = 0,
    ActiveContext = 1,
    OwningContext = 2,
    Pivot = 3,
    Pawn = 4,
    World = 5,
    ECameraNodeSpace_MAX = 6,
};

enum class ECameraObjectInterfaceParameterType {
    Blendable = 0,
    Data = 1,
    ECameraObjectInterfaceParameterType_MAX = 2,
};

enum class ECameraRigInitialOrientation {
    None = 0,
    ContextYawPitch = 1,
    PreviousYawPitch = 2,
    PreviousAbsoluteTarget = 3,
    PreviousRelativeTarget = 4,
    ECameraRigInitialOrientation_MAX = 5,
};

enum class ECameraRigLayer {
    None = 0,
    Base = 1,
    Main = 2,
    Global = 3,
    Visual = 4,
    ECameraRigLayer_MAX = 5,
};

enum class ECameraShakeEvaluationMode {
    Inline = 0,
    VisualLayer = 1,
    ECameraShakeEvaluationMode_MAX = 2,
};

enum class ECameraTargetShape {
    Point = 0,
    AutomaticBounds = 1,
    ManualBounds = 2,
    ECameraTargetShape_MAX = 3,
};

enum class ECameraVariableSetterBlendType {
    None = 0,
    Linear = 1,
    SmoothStep = 2,
    SmootherStep = 3,
    ECameraVariableSetterBlendType_MAX = 4,
};

enum class ECameraVariableType {
    Boolean = 0,
    Integer32 = 1,
    Float = 2,
    Double = 3,
    Vector2f = 4,
    Vector2d = 5,
    Vector3f = 6,
    Vector3d = 7,
    Vector4f = 8,
    Vector4d = 9,
    Rotator3f = 10,
    Rotator3d = 11,
    Transform3f = 12,
    Transform3d = 13,
    BlendableStruct = 14,
    ECameraVariableType_MAX = 15,
};

enum class ECollisionSafePosition {
    ActiveContext = 0,
    OwningContext = 1,
    Pivot = 2,
    Pawn = 3,
    ECollisionSafePosition_MAX = 4,
};

enum class ECollisionSafePositionOffsetSpace {
    ActiveContext = 0,
    OwningContext = 1,
    Pivot = 2,
    CameraPose = 3,
    Pawn = 4,
    ECollisionSafePositionOffsetSpace_MAX = 5,
};

enum class EGameplayCameraComponentActivationMode {
    Push = 0,
    PushAndInsert = 1,
    InsertOrPush = 2,
    EGameplayCameraComponentActivationMode_MAX = 3,
};

enum class EGameplayCamerasViewRotationMode {
    None = 0,
    PreviewUpdate = 1,
    EGameplayCamerasViewRotationMode_MAX = 2,
};

enum class ESmoothCameraBlendType {
    SmoothStep = 0,
    SmootherStep = 1,
    ESmoothCameraBlendType_MAX = 2,
};


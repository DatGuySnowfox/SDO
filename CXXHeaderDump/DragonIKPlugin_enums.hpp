enum class EDragonIKTraceMType {
    LineTrace = 0,
    SphereTrace = 1,
    EDragonIKTraceMType_MAX = 2,
};

enum class EIKTrace_Type_Plugin {
    ENUM_LineTrace_Type = 0,
    ENUM_SphereTrace_Type = 1,
    ENUM_MAX = 2,
};

enum class EIK_Type_Plugin {
    ENUM_Two_Bone_Ik = 0,
    ENUM_Single_Bone_Ik = 1,
    ENUM_MAX = 2,
};

enum class EInputTransformSpace_DragonIK {
    ENUM_WorldSpaceSystem = 0,
    ENUM_ComponentSpaceSystem = 1,
    ENUM_MAX = 2,
};

enum class EInput_FatBoneFAxis {
    ENUM_XAxis = 0,
    ENUM_YAxis = 1,
    ENUM_ZAxis = 2,
    ENUM_XYZAxis = 3,
    ENUM_MAX = 4,
};

enum class EInterpoLocation_Type_Plugin {
    ENUM_DivisiveLoc_Interp = 0,
    ENUM_LegacyLoc_Interp = 1,
    ENUM_MAX = 2,
};

enum class EInterpoRotation_Type_Plugin {
    ENUM_DivisiveRot_Interp = 0,
    ENUM_LegacyRot_Interp = 1,
    ENUM_MAX = 2,
};

enum class EPole_System_DragonIK {
    ENUM_SinglePoleSystem = 0,
    ENUM_NSEWPoleSystem = 1,
    ENUM_PoseBendSystem = 2,
    ENUM_MAX = 3,
};

enum class ERefPosePluginEnum {
    VE_Animated = 0,
    VE_Rest = 1,
    VE_MAX = 2,
};

enum class ERotation_Type_DragonIK {
    ENUM_AdditiveRotation = 0,
    ENUM_ReplaceRotation = 1,
    ENUM_MAX = 2,
};

enum class ESolverComplexityPluginEnum {
    VE_Simple = 0,
    VE_Complex = 1,
    VE_MAX = 2,
};

enum class ETwist_Type_DragonIK {
    ENUM_PoseAxisTwist = 0,
    ENUM_UpAxisTwist = 1,
    ENUM_MAX = 2,
};


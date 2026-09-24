enum class EBodyTextureType {
    Body_Basecolor = 0,
    Body_Normal = 1,
    Body_Cavity = 2,
    Body_Underwear_Basecolor = 3,
    Body_Underwear_Normal = 4,
    Body_Underwear_Mask = 5,
    Chest_Basecolor = 6,
    Chest_Normal = 7,
    Chest_Cavity = 8,
    Chest_Underwear_Basecolor = 9,
    Chest_Underwear_Normal = 10,
    Count = 11,
    EBodyTextureType_MAX = 12,
};

enum class EFaceTextureType {
    Basecolor = 0,
    Basecolor_Animated_CM1 = 1,
    Basecolor_Animated_CM2 = 2,
    Basecolor_Animated_CM3 = 3,
    Normal = 4,
    Normal_Animated_WM1 = 5,
    Normal_Animated_WM2 = 6,
    Normal_Animated_WM3 = 7,
    Cavity = 8,
    Count = 9,
    EFaceTextureType_MAX = 10,
};

enum class EMetaHumanBodyBodyPartIndex {
    Body = 0,
    Face = 1,
    Torso = 2,
    Legs = 3,
    Feet = 4,
    Count = 5,
    EMetaHumanBodyBodyPartIndex_MAX = 6,
};

enum class EMetaHumanBodyType {
    f_med_nrw = 0,
    f_med_ovw = 1,
    f_med_unw = 2,
    f_srt_nrw = 3,
    f_srt_ovw = 4,
    f_srt_unw = 5,
    f_tal_nrw = 6,
    f_tal_ovw = 7,
    f_tal_unw = 8,
    m_med_nrw = 9,
    m_med_ovw = 10,
    m_med_unw = 11,
    m_srt_nrw = 12,
    m_srt_ovw = 13,
    m_srt_unw = 14,
    m_tal_nrw = 15,
    m_tal_ovw = 16,
    m_tal_unw = 17,
    BlendableBody = 18,
    Count = 19,
    EMetaHumanBodyType_MAX = 20,
};

enum class EMetaHumanQualityLevel {
    Low = 0,
    Medium = 1,
    High = 2,
    Cinematic = 3,
    Count = 4,
    EMetaHumanQualityLevel_MAX = 5,
};


enum class EInterchangeMaterialXBSDF {
    OrenNayarDiffuse = 0,
    BurleyDiffuse = 1,
    Translucent = 2,
    Dielectric = 3,
    Conductor = 4,
    GeneralizedSchlick = 5,
    Subsurface = 6,
    Sheen = 7,
    ThinFilm = 8,
    MaxBSDFCount = 9,
    EInterchangeMaterialXBSDF_MAX = 10,
};

enum class EInterchangeMaterialXEDF {
    Uniform = 0,
    Conical = 1,
    Measured = 2,
    MaxEDFCount = 3,
    EInterchangeMaterialXEDF_MAX = 4,
};

enum class EInterchangeMaterialXShaders {
    OpenPBRSurface = 0,
    OpenPBRSurfaceTransmission = 1,
    StandardSurface = 2,
    StandardSurfaceTransmission = 3,
    SurfaceUnlit = 4,
    UsdPreviewSurface = 5,
    Surface = 6,
    Displacement = 7,
    MaxShaderCount = 8,
    EInterchangeMaterialXShaders_MAX = 9,
};

enum class EInterchangeMaterialXVDF {
    Absorption = 0,
    Anisotropic = 1,
    MaxVDFCount = 2,
    EInterchangeMaterialXVDF_MAX = 3,
};

enum class EInterchangeSparseVolumeTextureFormat {
    Unorm8 = 0,
    Float16 = 1,
    Float32 = 2,
    EInterchangeSparseVolumeTextureFormat_MAX = 3,
};

enum class EInterchangeUsdPrimvar {
    Standard = 0,
    Bake = 1,
    All = 2,
    EInterchangeUsdPrimvar_MAX = 3,
};


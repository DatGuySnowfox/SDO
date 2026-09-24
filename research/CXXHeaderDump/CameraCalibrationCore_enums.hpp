enum class ECalibratedMapChannels {
    RG = 0,
    BA = 1,
    None = 2,
    MAX = 3,
};

enum class ECalibratedMapPixelOrigin {
    TopLeft = 0,
    BottomLeft = 1,
    MAX = 2,
};

enum ECalibrationPointVisualization {
    CalibrationPointVisualizationCube = 0,
    CalibrationPointVisualizationPyramid = 1,
    ECalibrationPointVisualization_MAX = 2,
};

enum class ELensDataCategory {
    Focus = 0,
    Iris = 1,
    Zoom = 2,
    Distortion = 3,
    ImageCenter = 4,
    STMap = 5,
    NodalOffset = 6,
    ELensDataCategory_MAX = 7,
};

enum class ELensDataMode {
    Parameters = 0,
    STMap = 1,
    ELensDataMode_MAX = 2,
};

enum class ELensDisplayUnit {
    Millimeters = 0,
    Pixels = 1,
    Normalized = 2,
    ELensDisplayUnit_MAX = 3,
};


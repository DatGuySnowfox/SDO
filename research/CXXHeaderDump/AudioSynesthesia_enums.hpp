enum class EConstantQFFTSizeEnum {
    Min = 0,
    XXSmall = 1,
    XSmall = 2,
    Small = 3,
    Medium = 4,
    Large = 5,
    XLarge = 6,
    XXLarge = 7,
    Max = 8,
};

enum class EConstantQNormalizationEnum {
    EqualEuclideanNorm = 0,
    EqualEnergy = 1,
    EqualAmplitude = 2,
    EConstantQNormalizationEnum_MAX = 3,
};

enum class ELoudnessCurveTypeEnum {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    K = 4,
    None = 5,
    ELoudnessCurveTypeEnum_MAX = 6,
};

enum class ELoudnessNRTCurveTypeEnum {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    K = 4,
    None = 5,
    ELoudnessNRTCurveTypeEnum_MAX = 6,
};

enum class EMeterPeakType {
    MeanSquared = 0,
    RootMeanSquared = 1,
    Peak = 2,
    Count = 3,
    EMeterPeakType_MAX = 4,
};


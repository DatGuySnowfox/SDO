enum class EBrushActionMode {
    Paint = 0,
    FloodFill = 1,
    EBrushActionMode_MAX = 2,
};

enum class EBrushToolSizeType {
    Adaptive = 0,
    World = 1,
    EBrushToolSizeType_MAX = 2,
};

enum class EConvertToPolygonsMode {
    FaceNormalDeviation = 0,
    FindPolygons = 1,
    FromMaterialIDs = 7,
    FromUVIslands = 2,
    FromNormalSeams = 3,
    FromConnectedTris = 4,
    FromFurthestPointSampling = 5,
    CopyFromLayer = 6,
    EConvertToPolygonsMode_MAX = 8,
};

enum class EDisplaceMeshToolChannelType {
    Red = 0,
    Green = 1,
    Blue = 2,
    Alpha = 3,
    EDisplaceMeshToolChannelType_MAX = 4,
};

enum class EDisplaceMeshToolDisplaceType {
    Constant = 0,
    DisplacementMap = 1,
    RandomNoise = 2,
    PerlinNoise = 3,
    SineWave = 4,
    EDisplaceMeshToolDisplaceType_MAX = 5,
};

enum class EDisplaceMeshToolSubdivisionType {
    Flat = 0,
    PNTriangles = 1,
    EDisplaceMeshToolSubdivisionType_MAX = 2,
};

enum class EDisplaceMeshToolTriangleSelectionType {
    None = 0,
    Material = 1,
    EDisplaceMeshToolTriangleSelectionType_MAX = 2,
};

enum class EDrawPolygonDrawMode {
    Freehand = 0,
    Circle = 1,
    Square = 2,
    Rectangle = 3,
    RoundedRectangle = 4,
    Ring = 5,
    EDrawPolygonDrawMode_MAX = 6,
};

enum class EDrawPolygonExtrudeMode {
    Flat = 0,
    Fixed = 1,
    Interactive = 2,
    EDrawPolygonExtrudeMode_MAX = 3,
};

enum class EDynamicMeshSculptBrushType {
    Move = 0,
    PullKelvin = 1,
    PullSharpKelvin = 2,
    Smooth = 3,
    Offset = 4,
    SculptView = 5,
    SculptMax = 6,
    Inflate = 7,
    ScaleKelvin = 8,
    Pinch = 9,
    TwistKelvin = 10,
    Flatten = 11,
    Plane = 12,
    PlaneViewAligned = 13,
    FixedPlane = 14,
    Resample = 15,
    LastValue = 16,
    EDynamicMeshSculptBrushType_MAX = 17,
};

enum class EEdgeLoopInsertionMode {
    Retriangulate = 0,
    PlaneCut = 1,
    EEdgeLoopInsertionMode_MAX = 2,
};

enum class EEdgeLoopPositioningMode {
    Even = 0,
    ProportionOffset = 1,
    DistanceOffset = 2,
    EEdgeLoopPositioningMode_MAX = 3,
};

enum class EEditMeshPolygonsToolActions {
    NoAction = 0,
    AcceptCurrent = 1,
    CancelCurrent = 2,
    Extrude = 3,
    PushPull = 4,
    Offset = 5,
    Inset = 6,
    Outset = 7,
    BevelFaces = 8,
    InsertEdge = 9,
    InsertEdgeLoop = 10,
    Complete = 11,
    PlaneCut = 12,
    Merge = 13,
    Delete = 14,
    CutFaces = 15,
    RecalculateNormals = 16,
    FlipNormals = 17,
    Retriangulate = 18,
    Decompose = 19,
    Disconnect = 20,
    Duplicate = 21,
    CollapseEdge = 22,
    WeldEdges = 23,
    WeldEdgesCentered = 24,
    StraightenEdge = 25,
    FillHole = 26,
    BridgeEdges = 27,
    ExtrudeEdges = 28,
    BevelEdges = 29,
    SimplifyAlongEdges = 30,
    PlanarProjectionUV = 31,
    SimplifyByGroups = 32,
    RegenerateExtraCorners = 33,
    PokeSingleFace = 34,
    SplitSingleEdge = 35,
    FlipSingleEdge = 36,
    CollapseSingleEdge = 37,
    BevelAuto = 38,
    EEditMeshPolygonsToolActions_MAX = 39,
};

enum class EEditMeshPolygonsToolSelectionMode {
    Faces = 0,
    Edges = 1,
    Vertices = 2,
    Loops = 3,
    Rings = 4,
    FacesEdgesVertices = 5,
    EEditMeshPolygonsToolSelectionMode_MAX = 6,
};

enum class EFlareProfileType {
    SinMode = 0,
    SinSquaredMode = 1,
    TriangleMode = 2,
    EFlareProfileType_MAX = 3,
};

enum class EGroupBoundaryConstraint {
    Fixed = 7,
    Refine = 5,
    Free = 1,
    Ignore = 0,
    EGroupBoundaryConstraint_MAX = 8,
};

enum class EGroupEdgeInsertionMode {
    Retriangulate = 0,
    PlaneCut = 1,
    EGroupEdgeInsertionMode_MAX = 2,
};

enum class EGroupTopologyDeformationStrategy {
    Linear = 0,
    Laplacian = 1,
    EGroupTopologyDeformationStrategy_MAX = 2,
};

enum class EHoleFillToolActions {
    NoAction = 0,
    SelectAll = 1,
    ClearSelection = 2,
    EHoleFillToolActions_MAX = 3,
};

enum class ELatticeDeformerToolAction {
    NoAction = 0,
    Constrain = 1,
    ClearConstraints = 2,
    ELatticeDeformerToolAction_MAX = 3,
};

enum class ELatticeInterpolationType {
    Linear = 0,
    Cubic = 1,
    ELatticeInterpolationType_MAX = 2,
};

enum class ELocalFrameMode {
    FromObject = 0,
    FromGeometry = 1,
    ELocalFrameMode_MAX = 2,
};

enum class EMakeMeshPivotLocation {
    Base = 0,
    Centered = 1,
    Top = 2,
    EMakeMeshPivotLocation_MAX = 3,
};

enum class EMakeMeshPlacementType {
    GroundPlane = 0,
    OnScene = 1,
    AtOrigin = 2,
    EMakeMeshPlacementType_MAX = 3,
};

enum class EMakeMeshPolygroupMode {
    PerShape = 0,
    PerFace = 1,
    PerQuad = 2,
    EMakeMeshPolygroupMode_MAX = 3,
};

enum class EMaterialBoundaryConstraint {
    Fixed = 7,
    Refine = 5,
    Free = 1,
    Ignore = 0,
    EMaterialBoundaryConstraint_MAX = 8,
};

enum class EMeshAttributePaintToolActions {
    NoAction = 0,
    EMeshAttributePaintToolActions_MAX = 1,
};

enum class EMeshBoundaryConstraint {
    Fixed = 7,
    Refine = 5,
    Free = 1,
    EMeshBoundaryConstraint_MAX = 8,
};

enum class EMeshEditingMaterialModes {
    ExistingMaterial = 0,
    Diffuse = 1,
    Grey = 2,
    Soft = 3,
    Transparent = 4,
    TangentNormal = 5,
    VertexColor = 6,
    CustomImage = 7,
    Custom = 8,
    EMeshEditingMaterialModes_MAX = 9,
};

enum class EMeshGroupPaintBrushAreaType {
    Connected = 0,
    Volumetric = 1,
    EMeshGroupPaintBrushAreaType_MAX = 2,
};

enum class EMeshGroupPaintBrushType {
    Paint = 0,
    Erase = 1,
    LastValue = 2,
    EMeshGroupPaintBrushType_MAX = 3,
};

enum class EMeshGroupPaintInteractionType {
    Brush = 0,
    Fill = 1,
    GroupFill = 2,
    PolyLasso = 3,
    LastValue = 4,
    EMeshGroupPaintInteractionType_MAX = 5,
};

enum class EMeshGroupPaintToolActions {
    NoAction = 0,
    ClearFrozen = 1,
    FreezeCurrent = 2,
    FreezeOthers = 3,
    GrowCurrent = 4,
    ShrinkCurrent = 5,
    ClearCurrent = 6,
    FloodFillCurrent = 7,
    ClearAll = 8,
    EMeshGroupPaintToolActions_MAX = 9,
};

enum class EMeshGroupPaintVisibilityType {
    None = 0,
    FrontFacing = 1,
    Unoccluded = 2,
    EMeshGroupPaintVisibilityType_MAX = 3,
};

enum class EMeshSculptFalloffType {
    Smooth = 0,
    Linear = 1,
    Inverse = 2,
    Round = 3,
    BoxSmooth = 4,
    BoxLinear = 5,
    BoxInverse = 6,
    BoxRound = 7,
    LastValue = 8,
    EMeshSculptFalloffType_MAX = 9,
};

enum class EMeshSpaceDeformerToolAction {
    NoAction = 0,
    ShiftToCenter = 1,
    EMeshSpaceDeformerToolAction_MAX = 2,
};

enum class EMeshVertexPaintBrushAreaType {
    Connected = 0,
    Volumetric = 1,
    EMeshVertexPaintBrushAreaType_MAX = 2,
};

enum class EMeshVertexPaintBrushType {
    Paint = 0,
    Erase = 1,
    Soften = 2,
    Smooth = 3,
    LastValue = 4,
    EMeshVertexPaintBrushType_MAX = 5,
};

enum class EMeshVertexPaintColorBlendMode {
    Lerp = 0,
    Mix = 1,
    Multiply = 2,
    EMeshVertexPaintColorBlendMode_MAX = 3,
};

enum class EMeshVertexPaintColorChannel {
    Red = 0,
    Green = 1,
    Blue = 2,
    Alpha = 3,
    EMeshVertexPaintColorChannel_MAX = 4,
};

enum class EMeshVertexPaintInteractionType {
    Brush = 0,
    TriFill = 1,
    Fill = 2,
    GroupFill = 3,
    PolyLasso = 4,
    LastValue = 5,
    EMeshVertexPaintInteractionType_MAX = 6,
};

enum class EMeshVertexPaintMaterialMode {
    LitVertexColor = 0,
    UnlitVertexColor = 1,
    OriginalMaterial = 2,
    EMeshVertexPaintMaterialMode_MAX = 3,
};

enum class EMeshVertexPaintSecondaryActionType {
    Erase = 0,
    Soften = 1,
    Smooth = 2,
    EMeshVertexPaintSecondaryActionType_MAX = 3,
};

enum class EMeshVertexPaintToolActions {
    NoAction = 0,
    PaintAll = 1,
    EraseAll = 2,
    FillBlack = 3,
    FillWhite = 4,
    ApplyCurrentUtility = 5,
    EMeshVertexPaintToolActions_MAX = 6,
};

enum class EMeshVertexPaintToolUtilityOperations {
    BlendAllSeams = 0,
    FillChannels = 1,
    InvertChannels = 2,
    CopyChannelToChannel = 3,
    SwapChannels = 4,
    CopyFromWeightMap = 5,
    CopyToOtherLODs = 6,
    CopyToSingleLOD = 7,
    EMeshVertexPaintToolUtilityOperations_MAX = 8,
};

enum class EMeshVertexPaintVisibilityType {
    None = 0,
    FrontFacing = 1,
    Unoccluded = 2,
    EMeshVertexPaintVisibilityType_MAX = 3,
};

enum class EMeshVertexSculptBrushFilterType {
    None = 0,
    Component = 1,
    PolyGroup = 2,
    EMeshVertexSculptBrushFilterType_MAX = 3,
};

enum class EMeshVertexSculptBrushType {
    Move = 0,
    PullKelvin = 1,
    PullSharpKelvin = 2,
    Smooth = 3,
    SmoothFill = 4,
    Offset = 5,
    SculptView = 6,
    SculptMax = 7,
    Inflate = 8,
    ScaleKelvin = 9,
    Pinch = 10,
    TwistKelvin = 11,
    Flatten = 12,
    Plane = 13,
    PlaneViewAligned = 14,
    FixedPlane = 15,
    EraseSculptLayer = 16,
    LastValue = 17,
    EMeshVertexSculptBrushType_MAX = 18,
};

enum class ENonlinearOperationType {
    Bend = 0,
    Flare = 1,
    Twist = 2,
    ENonlinearOperationType_MAX = 3,
};

enum class EOccludedAction {
    Remove = 0,
    SetNewGroup = 1,
    EOccludedAction_MAX = 2,
};

enum class EOcclusionCalculationUIMode {
    GeneralizedWindingNumber = 0,
    RaycastOcclusionSamples = 1,
    EOcclusionCalculationUIMode_MAX = 2,
};

enum class EOcclusionTriangleSamplingUIMode {
    Vertices = 0,
    VerticesAndCentroids = 1,
    EOcclusionTriangleSamplingUIMode_MAX = 2,
};

enum class EOffsetMeshToolOffsetType {
    Iterative = 0,
    Implicit = 1,
    EOffsetMeshToolOffsetType_MAX = 2,
};

enum class EPlaneBrushSideMode {
    BothSides = 0,
    PushDown = 1,
    PullTowards = 2,
    EPlaneBrushSideMode_MAX = 3,
};

enum class EPolyEditCutPlaneOrientation {
    FaceNormals = 0,
    ViewDirection = 1,
    EPolyEditCutPlaneOrientation_MAX = 2,
};

enum class EPolyEditExtrudeDirection {
    SelectionNormal = 0,
    WorldX = 1,
    WorldY = 2,
    WorldZ = 3,
    LocalX = 4,
    LocalY = 5,
    LocalZ = 6,
    EPolyEditExtrudeDirection_MAX = 7,
};

enum class EPolyEditExtrudeDistanceMode {
    ClickInViewport = 0,
    Fixed = 1,
    EPolyEditExtrudeDistanceMode_MAX = 2,
};

enum class EPolyEditExtrudeEdgeDirectionMode {
    LocalExtrudeFrames = 0,
    SingleDirection = 1,
    EPolyEditExtrudeEdgeDirectionMode_MAX = 2,
};

enum class EPolyEditExtrudeEdgeDistanceMode {
    Fixed = 0,
    Gizmo = 1,
    EPolyEditExtrudeEdgeDistanceMode_MAX = 2,
};

enum class EPolyEditExtrudeModeOptions {
    SingleDirection = 3,
    SelectedTriangleNormals = 0,
    SelectedTriangleNormalsEven = 1,
    EPolyEditExtrudeModeOptions_MAX = 4,
};

enum class EPolyEditOffsetModeOptions {
    VertexNormals = 2,
    SelectedTriangleNormals = 0,
    SelectedTriangleNormalsEven = 1,
    EPolyEditOffsetModeOptions_MAX = 3,
};

enum class EPolyEditPushPullModeOptions {
    SelectedTriangleNormals = 0,
    SelectedTriangleNormalsEven = 1,
    SingleDirection = 3,
    VertexNormals = 2,
    EPolyEditPushPullModeOptions_MAX = 4,
};

enum class EProceduralDiscType {
    Disc = 0,
    PuncturedDisc = 1,
    EProceduralDiscType_MAX = 2,
};

enum class EProceduralRectType {
    Rectangle = 0,
    RoundedRectangle = 1,
    EProceduralRectType_MAX = 2,
};

enum class EProceduralSphereType {
    LatLong = 0,
    Box = 1,
    EProceduralSphereType_MAX = 2,
};

enum class EProceduralStairsType {
    Linear = 0,
    Floating = 1,
    Curved = 2,
    Spiral = 3,
    EProceduralStairsType_MAX = 4,
};

enum class EQuickTransformerMode {
    AxisTranslation = 0,
    AxisRotation = 1,
    EQuickTransformerMode_MAX = 2,
};

enum class ERevolvePropertiesCapFillMode {
    None = 0,
    CenterFan = 1,
    Delaunay = 2,
    EarClipping = 3,
    ERevolvePropertiesCapFillMode_MAX = 4,
};

enum class ERevolvePropertiesPolygroupMode {
    PerShape = 0,
    PerFace = 1,
    PerRevolveStep = 2,
    PerPathSegment = 3,
    ERevolvePropertiesPolygroupMode_MAX = 4,
};

enum class ERevolvePropertiesQuadSplit {
    Uniform = 0,
    Compact = 1,
    ERevolvePropertiesQuadSplit_MAX = 2,
};

enum class ESetMeshMaterialMode {
    Original = 0,
    Checkerboard = 1,
    Override = 2,
    ESetMeshMaterialMode_MAX = 3,
};

enum class ESmoothMeshToolSmoothType {
    Iterative = 0,
    Implicit = 1,
    Diffusion = 2,
    ESmoothMeshToolSmoothType_MAX = 3,
};

enum class EUVProjectionToolActions {
    NoAction = 0,
    AutoFit = 1,
    AutoFitAlign = 2,
    Reset = 3,
    EUVProjectionToolActions_MAX = 4,
};

enum class EUVProjectionToolInitializationMode {
    Default = 0,
    UsePrevious = 1,
    AutoFit = 2,
    AutoFitAlign = 3,
    EUVProjectionToolInitializationMode_MAX = 4,
};

enum class EVertexColorPaintBrushOpBlendMode {
    Lerp = 0,
    Mix = 1,
    Multiply = 2,
    EVertexColorPaintBrushOpBlendMode_MAX = 3,
};

enum class EWeightScheme {
    Uniform = 0,
    Umbrella = 1,
    Valence = 2,
    MeanValue = 3,
    Cotangent = 4,
    ClampedCotangent = 5,
    IDTCotangent = 6,
    EWeightScheme_MAX = 7,
};

enum class EWeldMeshEdgesAttributeUIMode {
    None = 0,
    OnWeldedMeshEdgesOnly = 1,
    OnFullMesh = 2,
    EWeldMeshEdgesAttributeUIMode_MAX = 3,
};


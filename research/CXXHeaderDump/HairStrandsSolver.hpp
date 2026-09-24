#ifndef UE4SS_SDK_HairStrandsSolver_HPP
#define UE4SS_SDK_HairStrandsSolver_HPP

struct FAddSolverDeformerDataflowNode : public FDataflowSimulationNode
{
    TArray<FDataflowSimulationProperty> PhysicsSolvers;                               // 0x0280 (size: 0x10)
    FDataflowSimulationTime SimulationTime;                                           // 0x0290 (size: 0xC)
    class UOptimusDeformer* MeshDeformer;                                             // 0x02A0 (size: 0x8)
    TArray<FDataflowNumericTypes> DeformerNumericInputs;                              // 0x02A8 (size: 0x10)
    TArray<FDataflowVectorTypes> DeformerVectorInputs;                                // 0x02B8 (size: 0x10)
    TArray<FDataflowStringTypes> DeformerStringInputs;                                // 0x02C8 (size: 0x10)
    TArray<FDataflowBoolTypes> DeformerBoolInputs;                                    // 0x02D8 (size: 0x10)
    TArray<FDataflowTransformTypes> DeformerTransformInputs;                          // 0x02E8 (size: 0x10)
    TArray<FDataflowNumericArrayTypes> DeformerNumericArrays;                         // 0x02F8 (size: 0x10)
    TArray<FDataflowVectorArrayTypes> DeformerVectorArrays;                           // 0x0308 (size: 0x10)
    TArray<FDataflowStringArrayTypes> DeformerStringArrays;                           // 0x0318 (size: 0x10)
    TArray<FDataflowBoolArrayTypes> DeformerBoolArrays;                               // 0x0328 (size: 0x10)
    TArray<FDataflowTransformArrayTypes> DeformerTransformArrays;                     // 0x0338 (size: 0x10)

}; // Size: 0x348

struct FDataflowGroomSolverProxy : public FDataflowPhysicsSolverProxy
{
}; // Size: 0x138

struct FGroomSolverSettings
{
    int32 NumSubSteps;                                                                // 0x0000 (size: 0x4)
    int32 NumSolverIterations;                                                        // 0x0004 (size: 0x4)
    float SimulationFrameRate;                                                        // 0x0008 (size: 0x4)
    float MaxLODDistance;                                                             // 0x000C (size: 0x4)
    float MinLODDistance;                                                             // 0x0010 (size: 0x4)

}; // Size: 0x68

class UGroomSolverComponent : public UMeshComponent
{
    FGroomSolverSettings SolverSettings;                                              // 0x0570 (size: 0x68)
    FDataflowSimulationAsset SimulationAsset;                                         // 0x05D8 (size: 0x58)
    TSet<UGroomComponent*> GroomComponents;                                           // 0x0630 (size: 0x50)
    class UMeshDeformer* MeshDeformer;                                                // 0x0680 (size: 0x8)
    class UMeshDeformerInstance* DeformerInstance;                                    // 0x0688 (size: 0x8)
    class UMeshDeformerInstanceSettings* DeformerSettings;                            // 0x0690 (size: 0x8)

    void SetDeformerSolver(class UMeshDeformer* DeformerSolver);
    void ResetGroomComponents();
    void RemoveGroomComponent(class UGroomComponent* GroomComponent);
    void AddGroomComponent(class UGroomComponent* GroomComponent);
}; // Size: 0x7D0

#endif

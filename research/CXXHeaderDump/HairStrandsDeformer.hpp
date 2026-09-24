#ifndef UE4SS_SDK_HairStrandsDeformer_HPP
#define UE4SS_SDK_HairStrandsDeformer_HPP

#include "HairStrandsDeformer_enums.hpp"

class UOptimusGroomAssetComponentSource : public UOptimusComponentSource
{
}; // Size: 0x28

class UOptimusGroomAttributeReadDataInterface : public UOptimusComputeDataInterface
{
    FName GroomAttributeName;                                                         // 0x0028 (size: 0x8)
    EOptimusGroomExecDomain GroomAttributeGroup;                                      // 0x0030 (size: 0x1)
    EOptimusGroomAttributeTypes GroomAttributeType;                                   // 0x0031 (size: 0x1)

}; // Size: 0x48

class UOptimusGroomAttributeReadDataProvider : public UComputeDataProvider
{
    class UMeshComponent* MeshComponent;                                              // 0x0028 (size: 0x8)
    FName GroomAttributeName;                                                         // 0x0030 (size: 0x8)
    EOptimusGroomExecDomain GroomAttributeGroup;                                      // 0x0038 (size: 0x1)
    EOptimusGroomAttributeTypes GroomAttributeType;                                   // 0x0039 (size: 0x1)

}; // Size: 0x40

class UOptimusGroomExecDataInterface : public UOptimusComputeDataInterface
{
    EOptimusGroomExecDomain Domain;                                                   // 0x0030 (size: 0x1)

}; // Size: 0x38

class UOptimusGroomExecDataProvider : public UComputeDataProvider
{
    class UMeshComponent* MeshComponent;                                              // 0x0028 (size: 0x8)
    EOptimusGroomExecDomain Domain;                                                   // 0x0030 (size: 0x1)

}; // Size: 0x38

class UOptimusGroomGuidesReadDataInterface : public UOptimusComputeDataInterface
{
}; // Size: 0x28

class UOptimusGroomGuidesReadDataProvider : public UComputeDataProvider
{
    class UMeshComponent* MeshComponent;                                              // 0x0028 (size: 0x8)

}; // Size: 0x30

class UOptimusGroomGuidesWriteDataInterface : public UOptimusComputeDataInterface
{
}; // Size: 0x28

class UOptimusGroomGuidesWriteDataProvider : public UComputeDataProvider
{
    class UMeshComponent* MeshComponent;                                              // 0x0028 (size: 0x8)

}; // Size: 0x38

class UOptimusGroomMeshesReadDataInterface : public UOptimusComputeDataInterface
{
}; // Size: 0x28

class UOptimusGroomMeshesReadDataProvider : public UComputeDataProvider
{
    class UMeshComponent* MeshComponent;                                              // 0x0028 (size: 0x8)

}; // Size: 0x30

class UOptimusGroomSolverComponentSource : public UOptimusComponentSource
{
}; // Size: 0x28

class UOptimusGroomSolverReadDataInterface : public UOptimusComputeDataInterface
{
    FName ResetSimulationTrigger;                                                     // 0x0028 (size: 0x8)

}; // Size: 0x30

class UOptimusGroomSolverReadDataProvider : public UComputeDataProvider
{
    class UGroomSolverComponent* SolverComponent;                                     // 0x0030 (size: 0x8)
    class UOptimusDeformerInstance* DeformerInstance;                                 // 0x0038 (size: 0x8)

}; // Size: 0x50

class UOptimusGroomStrandsReadDataInterface : public UOptimusComputeDataInterface
{
}; // Size: 0x28

class UOptimusGroomStrandsReadDataProvider : public UComputeDataProvider
{
    class UMeshComponent* MeshComponent;                                              // 0x0028 (size: 0x8)

}; // Size: 0x30

class UOptimusGroomStrandsWriteDataInterface : public UOptimusComputeDataInterface
{
}; // Size: 0x28

class UOptimusGroomStrandsWriteDataProvider : public UComputeDataProvider
{
    class UMeshComponent* MeshComponent;                                              // 0x0028 (size: 0x8)

}; // Size: 0x38

#endif

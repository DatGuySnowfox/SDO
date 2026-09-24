#ifndef UE4SS_SDK_GeometryDataflowNodes_HPP
#define UE4SS_SDK_GeometryDataflowNodes_HPP

#include "GeometryDataflowNodes_enums.hpp"

struct FMeshBooleanDataflowNode : public FDataflowNode
{
    EMeshBooleanOperationEnum Operation;                                              // 0x0280 (size: 0x1)
    class UDynamicMesh* Mesh1;                                                        // 0x0288 (size: 0x8)
    class UDynamicMesh* Mesh2;                                                        // 0x0290 (size: 0x8)
    class UDynamicMesh* Mesh;                                                         // 0x0298 (size: 0x8)

}; // Size: 0x2A0

#endif

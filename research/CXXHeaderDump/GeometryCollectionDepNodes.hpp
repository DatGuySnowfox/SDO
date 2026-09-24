#ifndef UE4SS_SDK_GeometryCollectionDepNodes_HPP
#define UE4SS_SDK_GeometryCollectionDepNodes_HPP

#include "GeometryCollectionDepNodes_enums.hpp"

struct FGeometryCollectionTransferVertexScalarAttributeNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FManagedArrayCollection FromCollection;                                           // 0x0330 (size: 0xB0)
    FCollectionAttributeKey AttributeKey;                                             // 0x03E0 (size: 0x20)
    EDataflowTransferNodeBoundingVolume BoundingVolumeType;                           // 0x0400 (size: 0x1)
    EDataflowTransferNodeSampleScale SampleScale;                                     // 0x0401 (size: 0x1)
    EDataflowTransferNodeFalloff Falloff;                                             // 0x0402 (size: 0x1)
    float FalloffThreshold;                                                           // 0x0404 (size: 0x4)
    float EdgeMultiplier;                                                             // 0x0408 (size: 0x4)
    float BoundMultiplier;                                                            // 0x040C (size: 0x4)

}; // Size: 0x410

struct FSetVertexColorInCollectionFromFloatArrayDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<float> FloatArray;                                                         // 0x0330 (size: 0x10)
    float Scale;                                                                      // 0x0340 (size: 0x4)

}; // Size: 0x348

struct FSetVertexColorInCollectionFromVertexSelectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowVertexSelection VertexSelection;                                         // 0x0330 (size: 0x28)
    FLinearColor SelectedColor;                                                       // 0x0358 (size: 0x10)
    FLinearColor NonSelectedColor;                                                    // 0x0368 (size: 0x10)

}; // Size: 0x378

#endif

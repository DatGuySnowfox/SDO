#ifndef UE4SS_SDK_DataflowCore_HPP
#define UE4SS_SDK_DataflowCore_HPP

#include "DataflowCore_enums.hpp"

struct FConvertBoolArrayTypesDataflowNode : public FDataflowNode
{
    FDataflowBoolArrayTypes In;                                                       // 0x0280 (size: 0x10)
    FDataflowBoolArrayTypes Out;                                                      // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FConvertBoolTypesDataflowNode : public FDataflowNode
{
    FDataflowBoolTypes In;                                                            // 0x0280 (size: 0x1)
    FDataflowBoolTypes Out;                                                           // 0x0281 (size: 0x1)

}; // Size: 0x288

struct FConvertNumericArrayTypesDataflowNode : public FDataflowNode
{
    FDataflowNumericArrayTypes In;                                                    // 0x0280 (size: 0x10)
    FDataflowNumericArrayTypes Out;                                                   // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FConvertNumericTypesDataflowNode : public FDataflowNode
{
    FDataflowNumericTypes In;                                                         // 0x0280 (size: 0x8)
    FDataflowNumericTypes Out;                                                        // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FConvertRotationDataflowNode : public FDataflowNode
{
    FDataflowRotationTypes In;                                                        // 0x0280 (size: 0x18)
    FDataflowRotationTypes Out;                                                       // 0x0298 (size: 0x18)

}; // Size: 0x2B0

struct FConvertSelectionTypesDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowSelectionTypes In;                                                       // 0x0330 (size: 0x28)
    bool bAllElementsMustBeSelected;                                                  // 0x0358 (size: 0x1)
    FDataflowSelectionTypes Out;                                                      // 0x0360 (size: 0x28)

}; // Size: 0x388

struct FConvertStringArrayTypesDataflowNode : public FDataflowNode
{
    FDataflowStringArrayTypes In;                                                     // 0x0280 (size: 0x10)
    FDataflowStringArrayTypes Out;                                                    // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FConvertStringConvertibleTypesDataflowNode : public FDataflowNode
{
    FDataflowStringConvertibleTypes In;                                               // 0x0280 (size: 0x10)
    FDataflowStringConvertibleTypes Out;                                              // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FConvertStringTypesDataflowNode : public FDataflowNode
{
    FDataflowStringTypes In;                                                          // 0x0280 (size: 0x10)
    FDataflowStringTypes Out;                                                         // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FConvertTransformArrayTypesDataflowNode : public FDataflowNode
{
    FDataflowTransformArrayTypes In;                                                  // 0x0280 (size: 0x10)
    FDataflowTransformArrayTypes Out;                                                 // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FConvertTransformTypesDataflowNode : public FDataflowNode
{
    FDataflowTransformTypes In;                                                       // 0x0280 (size: 0x60)
    FDataflowTransformTypes Out;                                                      // 0x02E0 (size: 0x60)

}; // Size: 0x340

struct FConvertUObjectConvertibleTypesDataflowNode : public FDataflowNode
{
    FDataflowUObjectConvertibleTypes In;                                              // 0x0280 (size: 0x8)
    FDataflowUObjectConvertibleTypes Out;                                             // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FConvertVectorArrayTypesDataflowNode : public FDataflowNode
{
    FDataflowVectorArrayTypes In;                                                     // 0x0280 (size: 0x10)
    FDataflowVectorArrayTypes Out;                                                    // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FConvertVectorTypesDataflowNode : public FDataflowNode
{
    FDataflowVectorTypes In;                                                          // 0x0280 (size: 0x20)
    FDataflowVectorTypes Out;                                                         // 0x02A0 (size: 0x20)

}; // Size: 0x2C0

struct FDataflowAllTypes : public FDataflowAnyType
{
}; // Size: 0x1

struct FDataflowAnyType
{
}; // Size: 0x1

struct FDataflowArrayInput : public FDataflowInput
{
}; // Size: 0x80

struct FDataflowArrayOutput : public FDataflowOutput
{
}; // Size: 0xA0

struct FDataflowArrayTypes : public FDataflowAnyType
{
}; // Size: 0x1

struct FDataflowBaseElement
{
}; // Size: 0x60

struct FDataflowBoolArrayTypes : public FDataflowAnyType
{
    TArray<bool> Value;                                                               // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDataflowBoolTypes : public FDataflowAnyType
{
    bool Value;                                                                       // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FDataflowBranchNode : public FDataflowNode
{
    FDataflowAnyType TrueValue;                                                       // 0x0280 (size: 0x1)
    FDataflowAnyType FalseValue;                                                      // 0x0281 (size: 0x1)
    bool bCondition;                                                                  // 0x0282 (size: 0x1)
    FDataflowAnyType Result;                                                          // 0x0283 (size: 0x1)

}; // Size: 0x288

struct FDataflowConnection
{
}; // Size: 0x60

struct FDataflowFaceSelection : public FDataflowSelection
{
}; // Size: 0x28

struct FDataflowForceDependencyNode : public FDataflowNode
{
    FDataflowAnyType Value;                                                           // 0x0280 (size: 0x1)
    FDataflowAnyType DependentValue;                                                  // 0x0281 (size: 0x1)

}; // Size: 0x288

struct FDataflowFreezeActions
{
}; // Size: 0x1

struct FDataflowGeometrySelection : public FDataflowSelection
{
}; // Size: 0x28

struct FDataflowImage
{
}; // Size: 0x28

struct FDataflowImageCombineChannelsNode : public FDataflowNode
{
    FDataflowImage Red;                                                               // 0x0280 (size: 0x28)
    FDataflowImage Green;                                                             // 0x02A8 (size: 0x28)
    FDataflowImage Blue;                                                              // 0x02D0 (size: 0x28)
    FDataflowImage Alpha;                                                             // 0x02F8 (size: 0x28)
    FDataflowImage Image;                                                             // 0x0320 (size: 0x28)
    EDataflowImageCombineResolutionOption ResolutionOptions;                          // 0x0348 (size: 0x4)
    EDataflowImageResolution Resolution;                                              // 0x034C (size: 0x4)

}; // Size: 0x350

struct FDataflowImageFromColorNode : public FDataflowNode
{
    FLinearColor FillColor;                                                           // 0x0280 (size: 0x10)
    EDataflowImageResolution Resolution;                                              // 0x0290 (size: 0x4)
    FDataflowImage Image;                                                             // 0x0298 (size: 0x28)

}; // Size: 0x2C0

struct FDataflowImageSplitChannelsNode : public FDataflowNode
{
    FDataflowImage Image;                                                             // 0x0280 (size: 0x28)
    FDataflowImage Red;                                                               // 0x02A8 (size: 0x28)
    FDataflowImage Green;                                                             // 0x02D0 (size: 0x28)
    FDataflowImage Blue;                                                              // 0x02F8 (size: 0x28)
    FDataflowImage Alpha;                                                             // 0x0320 (size: 0x28)

}; // Size: 0x348

struct FDataflowInput : public FDataflowConnection
{
}; // Size: 0x70

struct FDataflowMaterialSelection : public FDataflowSelection
{
}; // Size: 0x28

struct FDataflowMathAbsNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathAddNode : public FDataflowMathTwoInputsOperatorNode
{
}; // Size: 0x298

struct FDataflowMathArcCosNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathArcSinNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathArcTan2Node : public FDataflowMathTwoInputsOperatorNode
{
}; // Size: 0x298

struct FDataflowMathArcTanNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathCeilNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathClampNode : public FDataflowMathOneInputOperatorNode
{
    FDataflowNumericTypes min;                                                        // 0x0290 (size: 0x8)
    FDataflowNumericTypes max;                                                        // 0x0298 (size: 0x8)

}; // Size: 0x2A0

struct FDataflowMathConstantNode : public FDataflowNode
{
    EDataflowMathConstantsEnum Constant;                                              // 0x0280 (size: 0x1)
    FDataflowNumericTypes Result;                                                     // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FDataflowMathCosNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathCubeNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathDegToRadNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathDivideNode : public FDataflowMathTwoInputsOperatorNode
{
    FDataflowNumericTypes Fallback;                                                   // 0x0298 (size: 0x8)

}; // Size: 0x2A0

struct FDataflowMathExpNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathFloorNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathFracNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathInverseSquareRootNode : public FDataflowMathOneInputOperatorNode
{
    FDataflowNumericTypes Fallback;                                                   // 0x0290 (size: 0x8)

}; // Size: 0x298

struct FDataflowMathLogNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathLogXNode : public FDataflowMathOneInputOperatorNode
{
    FDataflowNumericTypes base;                                                       // 0x0290 (size: 0x8)

}; // Size: 0x298

struct FDataflowMathMaximumNode : public FDataflowMathTwoInputsOperatorNode
{
}; // Size: 0x298

struct FDataflowMathMaximumNode_v2 : public FDataflowNode
{
    TArray<FDataflowNumericTypes> Inputs;                                             // 0x0280 (size: 0x10)
    FDataflowNumericTypes Result;                                                     // 0x0290 (size: 0x8)

}; // Size: 0x298

struct FDataflowMathMinimumNode : public FDataflowMathTwoInputsOperatorNode
{
}; // Size: 0x298

struct FDataflowMathMinimumNode_v2 : public FDataflowNode
{
    TArray<FDataflowNumericTypes> Inputs;                                             // 0x0280 (size: 0x10)
    FDataflowNumericTypes Result;                                                     // 0x0290 (size: 0x8)

}; // Size: 0x298

struct FDataflowMathMultiplyNode : public FDataflowMathTwoInputsOperatorNode
{
}; // Size: 0x298

struct FDataflowMathNegateNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathOneInputOperatorNode : public FDataflowNode
{
    FDataflowNumericTypes A;                                                          // 0x0280 (size: 0x8)
    FDataflowNumericTypes Result;                                                     // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FDataflowMathOneMinusNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathPowNode : public FDataflowMathTwoInputsOperatorNode
{
}; // Size: 0x298

struct FDataflowMathRadToDegNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathReciprocalNode : public FDataflowMathOneInputOperatorNode
{
    FDataflowNumericTypes Fallback;                                                   // 0x0290 (size: 0x8)

}; // Size: 0x298

struct FDataflowMathRoundNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathSignNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathSinNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathSquareNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathSquareRootNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathSubtractNode : public FDataflowMathTwoInputsOperatorNode
{
}; // Size: 0x298

struct FDataflowMathTanNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathTruncNode : public FDataflowMathOneInputOperatorNode
{
}; // Size: 0x290

struct FDataflowMathTwoInputsOperatorNode : public FDataflowNode
{
    FDataflowNumericTypes A;                                                          // 0x0280 (size: 0x8)
    FDataflowNumericTypes B;                                                          // 0x0288 (size: 0x8)
    FDataflowNumericTypes Result;                                                     // 0x0290 (size: 0x8)

}; // Size: 0x298

struct FDataflowNode
{
    bool bActive;                                                                     // 0x00D8 (size: 0x1)
    bool bOverrideColor;                                                              // 0x00D9 (size: 0x1)
    FLinearColor OverrideColor;                                                       // 0x00DC (size: 0x10)
    FInstancedPropertyBag FrozenProperties;                                           // 0x0230 (size: 0x10)
    bool bIsFrozen;                                                                   // 0x0240 (size: 0x1)

}; // Size: 0x280

struct FDataflowNumericArrayTypes : public FDataflowAnyType
{
    TArray<double> Value;                                                             // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDataflowNumericTypes : public FDataflowAnyType
{
    double Value;                                                                     // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FDataflowOutput : public FDataflowConnection
{
}; // Size: 0x90

struct FDataflowOverrideNode : public FDataflowNode
{
    FName Key;                                                                        // 0x0280 (size: 0x8)
    FString Default;                                                                  // 0x0288 (size: 0x10)
    bool IsOverriden;                                                                 // 0x0298 (size: 0x1)

}; // Size: 0x2A0

struct FDataflowPath
{
}; // Size: 0x40

struct FDataflowPrintNode : public FDataflowNode
{
    FDataflowStringConvertibleTypes Value;                                            // 0x0280 (size: 0x10)

}; // Size: 0x290

struct FDataflowReRouteNode : public FDataflowNode
{
    FDataflowAnyType Value;                                                           // 0x0280 (size: 0x1)

}; // Size: 0x288

struct FDataflowRotationTypes : public FDataflowAnyType
{
    FRotator Value;                                                                   // 0x0000 (size: 0x18)

}; // Size: 0x18

struct FDataflowSelectNode : public FDataflowNode
{
    TArray<FDataflowAnyType> Inputs;                                                  // 0x0280 (size: 0x10)
    int32 SelectedIndex;                                                              // 0x0290 (size: 0x4)
    FDataflowAnyType Result;                                                          // 0x0294 (size: 0x1)

}; // Size: 0x298

struct FDataflowSelection
{
}; // Size: 0x28

struct FDataflowSelectionTypes : public FDataflowAnyType
{
    FDataflowSelection Value;                                                         // 0x0000 (size: 0x28)

}; // Size: 0x28

struct FDataflowStringArrayTypes : public FDataflowAnyType
{
    TArray<FString> Value;                                                            // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDataflowStringConvertibleTypes : public FDataflowAnyType
{
    FString Value;                                                                    // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDataflowStringTypes : public FDataflowAnyType
{
    FString Value;                                                                    // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDataflowTerminalNode : public FDataflowNode
{
}; // Size: 0x280

struct FDataflowTransformArrayTypes : public FDataflowAnyType
{
    TArray<FTransform> Value;                                                         // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDataflowTransformSelection : public FDataflowSelection
{
}; // Size: 0x28

struct FDataflowTransformTypes : public FDataflowAnyType
{
    FTransform Value;                                                                 // 0x0000 (size: 0x60)

}; // Size: 0x60

struct FDataflowUObjectConvertibleTypes : public FDataflowAnyType
{
    class UObject* Value;                                                             // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FDataflowVectorAddNode : public FDataflowNode
{
    FDataflowVectorTypes A;                                                           // 0x0280 (size: 0x20)
    FDataflowVectorTypes B;                                                           // 0x02A0 (size: 0x20)
    FDataflowVectorTypes V;                                                           // 0x02C0 (size: 0x20)

}; // Size: 0x2E0

struct FDataflowVectorArrayTypes : public FDataflowAnyType
{
    TArray<FVector4> Value;                                                           // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDataflowVectorBreakNode : public FDataflowNode
{
    FDataflowVectorTypes V;                                                           // 0x0280 (size: 0x20)
    FDataflowNumericTypes X;                                                          // 0x02A0 (size: 0x8)
    FDataflowNumericTypes Y;                                                          // 0x02A8 (size: 0x8)
    FDataflowNumericTypes Z;                                                          // 0x02B0 (size: 0x8)
    FDataflowNumericTypes W;                                                          // 0x02B8 (size: 0x8)

}; // Size: 0x2C0

struct FDataflowVectorCrossProductNode : public FDataflowNode
{
    FDataflowVectorTypes A;                                                           // 0x0280 (size: 0x20)
    FDataflowVectorTypes B;                                                           // 0x02A0 (size: 0x20)
    FDataflowVectorTypes CrossProduct;                                                // 0x02C0 (size: 0x20)

}; // Size: 0x2E0

struct FDataflowVectorDistanceNode : public FDataflowNode
{
    FDataflowVectorTypes A;                                                           // 0x0280 (size: 0x20)
    FDataflowVectorTypes B;                                                           // 0x02A0 (size: 0x20)
    FDataflowNumericTypes Distance;                                                   // 0x02C0 (size: 0x8)

}; // Size: 0x2D0

struct FDataflowVectorDotProductNode : public FDataflowNode
{
    FDataflowVectorTypes A;                                                           // 0x0280 (size: 0x20)
    FDataflowVectorTypes B;                                                           // 0x02A0 (size: 0x20)
    FDataflowNumericTypes DotProduct;                                                 // 0x02C0 (size: 0x8)

}; // Size: 0x2D0

struct FDataflowVectorLengthNode : public FDataflowNode
{
    FDataflowVectorTypes V;                                                           // 0x0280 (size: 0x20)
    FDataflowNumericTypes Length;                                                     // 0x02A0 (size: 0x8)

}; // Size: 0x2B0

struct FDataflowVectorMakeVec2Node : public FDataflowNode
{
    FDataflowNumericTypes X;                                                          // 0x0280 (size: 0x8)
    FDataflowNumericTypes Y;                                                          // 0x0288 (size: 0x8)
    FDataflowVectorTypes Vector2D;                                                    // 0x0290 (size: 0x20)

}; // Size: 0x2B0

struct FDataflowVectorMakeVec3Node : public FDataflowNode
{
    FDataflowNumericTypes X;                                                          // 0x0280 (size: 0x8)
    FDataflowNumericTypes Y;                                                          // 0x0288 (size: 0x8)
    FDataflowNumericTypes Z;                                                          // 0x0290 (size: 0x8)
    FDataflowVectorTypes Vector3d;                                                    // 0x02A0 (size: 0x20)

}; // Size: 0x2C0

struct FDataflowVectorMakeVec4Node : public FDataflowNode
{
    FDataflowNumericTypes X;                                                          // 0x0280 (size: 0x8)
    FDataflowNumericTypes Y;                                                          // 0x0288 (size: 0x8)
    FDataflowNumericTypes Z;                                                          // 0x0290 (size: 0x8)
    FDataflowNumericTypes W;                                                          // 0x0298 (size: 0x8)
    FDataflowVectorTypes Vector4d;                                                    // 0x02A0 (size: 0x20)

}; // Size: 0x2C0

struct FDataflowVectorNormalize : public FDataflowNode
{
    FDataflowVectorTypes V;                                                           // 0x0280 (size: 0x20)
    FDataflowVectorTypes Normalized;                                                  // 0x02A0 (size: 0x20)

}; // Size: 0x2C0

struct FDataflowVectorScaleNode : public FDataflowNode
{
    FDataflowVectorTypes V;                                                           // 0x0280 (size: 0x20)
    FDataflowNumericTypes Scale;                                                      // 0x02A0 (size: 0x8)
    FDataflowVectorTypes Scaled;                                                      // 0x02B0 (size: 0x20)

}; // Size: 0x2D0

struct FDataflowVectorSquaredLengthNode : public FDataflowNode
{
    FDataflowVectorTypes V;                                                           // 0x0280 (size: 0x20)
    FDataflowNumericTypes SquaredLength;                                              // 0x02A0 (size: 0x8)

}; // Size: 0x2B0

struct FDataflowVectorSubtractNode : public FDataflowNode
{
    FDataflowVectorTypes A;                                                           // 0x0280 (size: 0x20)
    FDataflowVectorTypes B;                                                           // 0x02A0 (size: 0x20)
    FDataflowVectorTypes V;                                                           // 0x02C0 (size: 0x20)

}; // Size: 0x2E0

struct FDataflowVectorTypes : public FDataflowAnyType
{
    FVector4 Value;                                                                   // 0x0000 (size: 0x20)

}; // Size: 0x20

struct FDataflowVertexSelection : public FDataflowSelection
{
}; // Size: 0x28

struct FNodeColors
{
    FLinearColor NodeTitleColor;                                                      // 0x0000 (size: 0x10)
    FLinearColor NodeBodyTintColor;                                                   // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FPinSettings
{
    FLinearColor PinColor;                                                            // 0x0000 (size: 0x10)
    float WireThickness;                                                              // 0x0010 (size: 0x4)

}; // Size: 0x14

struct FTransformLevelColors
{
    TArray<FLinearColor> LevelColors;                                                 // 0x0000 (size: 0x10)
    FLinearColor BlankColor;                                                          // 0x0010 (size: 0x10)

}; // Size: 0x20

class IDataflowGraphInterface : public IInterface
{
}; // Size: 0x28

class UDataflowSettings : public UDeveloperSettings
{
    TMap<FName, FNodeColors> NodeColorsMap;                                           // 0x0038 (size: 0x50)
    TMap<FName, FPinSettings> PinSettingsMap;                                         // 0x0088 (size: 0x50)
    FTransformLevelColors TransformLevelColors;                                       // 0x00D8 (size: 0x20)

}; // Size: 0x128

#endif

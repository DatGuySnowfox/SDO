#ifndef UE4SS_SDK_ChaosVehiclesCore_HPP
#define UE4SS_SDK_ChaosVehiclesCore_HPP

#include "ChaosVehiclesCore_enums.hpp"

struct FModuleInputContainer
{
    TArray<FModuleInputValue> InputValues;                                            // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FModuleInputSetup
{
    FName Name;                                                                       // 0x0000 (size: 0x8)
    EModuleInputValueType Type;                                                       // 0x0008 (size: 0x1)
    TSubclassOf<class UDefaultModularVehicleInputModifier> InputModifierClass;        // 0x0010 (size: 0x8)
    bool bApplyInputDecay;                                                            // 0x0018 (size: 0x1)

}; // Size: 0x20

struct FModuleInputValue
{
    FVector Value;                                                                    // 0x0000 (size: 0x18)
    int32 ValueInt;                                                                   // 0x0018 (size: 0x4)
    EModuleInputValueType ValueType;                                                  // 0x001C (size: 0x1)
    bool bApplyInputDecay;                                                            // 0x001D (size: 0x1)

}; // Size: 0x20

class UDefaultModularVehicleInputModifier : public UObject
{
    float RiseRate;                                                                   // 0x0028 (size: 0x4)
    float FallRate;                                                                   // 0x002C (size: 0x4)
    EFunctionType InputCurveFunction;                                                 // 0x0030 (size: 0x1)

}; // Size: 0x38

class UVehicleInputProducerBase : public UObject
{
}; // Size: 0x28

#endif

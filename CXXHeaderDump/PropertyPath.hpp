#ifndef UE4SS_SDK_PropertyPath_HPP
#define UE4SS_SDK_PropertyPath_HPP

#include "PropertyPath_enums.hpp"

struct FCachedPropertyPath
{
    TArray<FPropertyPathSegment> Segments;                                            // 0x0000 (size: 0x10)
    class UFunction* CachedFunction;                                                  // 0x0018 (size: 0x8)

}; // Size: 0x38

struct FPropertyPathSegment
{
    FName Name;                                                                       // 0x0000 (size: 0x8)
    int32 ArrayIndex;                                                                 // 0x0008 (size: 0x4)
    class UStruct* Struct;                                                            // 0x0010 (size: 0x8)

}; // Size: 0x20

struct FPropertyPathTestBed
{
    class UPropertyPathTestObject* Object;                                            // 0x0000 (size: 0x8)
    class UPropertyPathTestObject* ModifiedObject;                                    // 0x0008 (size: 0x8)
    FPropertyPathTestStruct ModifiedStruct;                                           // 0x0010 (size: 0x50)
    FPropertyPathTestStruct DefaultStruct;                                            // 0x0060 (size: 0x50)

}; // Size: 0xB0

struct FPropertyPathTestInnerStruct
{
    float float;                                                                      // 0x0000 (size: 0x4)
    bool bool;                                                                        // 0x0004 (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumOne;                                       // 0x0005 (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumTwo;                                       // 0x0006 (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumThree;                                     // 0x0007 (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumFour;                                      // 0x0008 (size: 0x1)
    int32 Integer;                                                                    // 0x000C (size: 0x4)
    FString String;                                                                   // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FPropertyPathTestStruct
{
    bool bool;                                                                        // 0x0000 (size: 0x1)
    int32 Integer;                                                                    // 0x0004 (size: 0x4)
    TEnumAsByte<EPropertyPathTestEnum> EnumOne;                                       // 0x0008 (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumTwo;                                       // 0x0009 (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumThree;                                     // 0x000A (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumFour;                                      // 0x000B (size: 0x1)
    FString String;                                                                   // 0x0010 (size: 0x10)
    float float;                                                                      // 0x0020 (size: 0x4)
    FPropertyPathTestInnerStruct InnerStruct;                                         // 0x0028 (size: 0x20)
    class UPropertyPathTestObject* InnerObject;                                       // 0x0048 (size: 0x8)

}; // Size: 0x50

class UPropertyPathTestObject : public UObject
{
    bool bool;                                                                        // 0x0028 (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumOne;                                       // 0x0029 (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumTwo;                                       // 0x002A (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumThree;                                     // 0x002B (size: 0x1)
    TEnumAsByte<EPropertyPathTestEnum> EnumFour;                                      // 0x002C (size: 0x1)
    int32 Integer;                                                                    // 0x0030 (size: 0x4)
    FString String;                                                                   // 0x0038 (size: 0x10)
    float float;                                                                      // 0x0048 (size: 0x4)
    FPropertyPathTestStruct Struct;                                                   // 0x0050 (size: 0x50)
    FPropertyPathTestStruct StructRef;                                                // 0x00A0 (size: 0x50)
    FPropertyPathTestStruct StructConstRef;                                           // 0x00F0 (size: 0x50)
    class UPropertyPathTestObject* InnerObject;                                       // 0x0140 (size: 0x8)

    void SetStructRef(FPropertyPathTestStruct InStruct);
    void SetStructConstRef(FPropertyPathTestStruct InStruct);
    void SetStruct(FPropertyPathTestStruct InStruct);
    void SetFloat(float InFloat);
    FPropertyPathTestStruct GetStructRef();
    FPropertyPathTestStruct GetStructConstRef();
    FPropertyPathTestStruct GetStruct();
    float GetFloat();
}; // Size: 0x150

#endif

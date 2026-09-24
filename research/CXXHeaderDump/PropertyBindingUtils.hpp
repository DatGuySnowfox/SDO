#ifndef UE4SS_SDK_PropertyBindingUtils_HPP
#define UE4SS_SDK_PropertyBindingUtils_HPP

#include "PropertyBindingUtils_enums.hpp"

struct FPropertyBindingBindableStructDescriptor
{
    class UStruct* Struct;                                                            // 0x0008 (size: 0x8)
    FName Name;                                                                       // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FPropertyBindingBinding
{
    FPropertyBindingPath SourcePropertyPath;                                          // 0x0008 (size: 0x10)
    FPropertyBindingPath TargetPropertyPath;                                          // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FPropertyBindingBindingCollection
{
    TArray<FPropertyBindingCopyInfoBatch> CopyBatches;                                // 0x0010 (size: 0x10)
    TArray<FPropertyBindingCopyInfo> PropertyCopies;                                  // 0x0020 (size: 0x10)
    TArray<FPropertyBindingPropertyIndirection> PropertyIndirections;                 // 0x0030 (size: 0x10)
    TScriptInterface<class IPropertyBindingBindingCollectionOwner> BindingsOwner;     // 0x0048 (size: 0x10)

}; // Size: 0xC0

struct FPropertyBindingCopyInfo
{
    FPropertyBindingPropertyIndirection SourceIndirection;                            // 0x0000 (size: 0x18)
    FPropertyBindingPropertyIndirection TargetIndirection;                            // 0x0018 (size: 0x18)
    class UStruct* SourceStructType;                                                  // 0x0040 (size: 0x8)
    int32 CopySize;                                                                   // 0x0048 (size: 0x4)
    FInstancedStruct SourceDataHandle;                                                // 0x0050 (size: 0x10)
    EPropertyCopyType Type;                                                           // 0x0060 (size: 0x1)

}; // Size: 0x68

struct FPropertyBindingCopyInfoBatch
{
    FInstancedStruct TargetStruct;                                                    // 0x0000 (size: 0x10)
    FPropertyBindingIndex16 BindingsBegin;                                            // 0x0010 (size: 0x2)
    FPropertyBindingIndex16 BindingsEnd;                                              // 0x0012 (size: 0x2)
    FPropertyBindingIndex16 PropertyFunctionsBegin;                                   // 0x0014 (size: 0x2)
    FPropertyBindingIndex16 PropertyFunctionsEnd;                                     // 0x0016 (size: 0x2)

}; // Size: 0x18

struct FPropertyBindingIndex16
{
    uint16 Value;                                                                     // 0x0000 (size: 0x2)

}; // Size: 0x2

struct FPropertyBindingPath
{
    TArray<FPropertyBindingPathSegment> Segments;                                     // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FPropertyBindingPathSegment
{
    FName Name;                                                                       // 0x0000 (size: 0x8)
    int32 ArrayIndex;                                                                 // 0x0008 (size: 0x4)
    class UStruct* InstanceStruct;                                                    // 0x0010 (size: 0x8)
    EPropertyBindingPropertyAccessType InstancedStructAccessType;                     // 0x0018 (size: 0x1)

}; // Size: 0x20

struct FPropertyBindingPropertyIndirection
{
    FPropertyBindingIndex16 ArrayIndex;                                               // 0x0000 (size: 0x2)
    uint16 Offset;                                                                    // 0x0002 (size: 0x2)
    FPropertyBindingIndex16 NextIndex;                                                // 0x0004 (size: 0x2)
    EPropertyBindingPropertyAccessType Type;                                          // 0x0006 (size: 0x1)
    class UStruct* InstanceStruct;                                                    // 0x0008 (size: 0x8)

}; // Size: 0x18

class IPropertyBindingBindingCollectionOwner : public IInterface
{
}; // Size: 0x28

#endif

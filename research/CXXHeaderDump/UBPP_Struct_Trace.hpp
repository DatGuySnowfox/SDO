#ifndef UE4SS_SDK_UBPP_Struct_Trace_HPP
#define UE4SS_SDK_UBPP_Struct_Trace_HPP

struct FUBPP_Struct_Trace
{
    TArray<class AActor*> ActorstoIgnore_11_59C868654E8EA627D004178C0BDB17CC;         // 0x0000 (size: 0x10)
    TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType_18_2DF5AFE84B80E0506192068839BAE11A; // 0x0010 (size: 0x1)
    bool IgnoreSelf_17_39A7384449C24B2F0E0AC791E8C53C0D;                              // 0x0011 (size: 0x1)
    FLinearColor TraceColor_2_39F0CF91492D0BC525B3FDA56C286A0F;                       // 0x0014 (size: 0x10)
    FLinearColor TraceHitColor_4_7E31B0934E2BD6B1163589A9E9F51153;                    // 0x0024 (size: 0x10)
    float DrawTime_7_2314DB4F469D2CA55CBDC1BB60FF1274;                                // 0x0034 (size: 0x4)

}; // Size: 0x38

#endif

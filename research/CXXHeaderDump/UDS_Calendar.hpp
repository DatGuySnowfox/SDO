#ifndef UE4SS_SDK_UDS_Calendar_HPP
#define UE4SS_SDK_UDS_Calendar_HPP

class UUDS_Calendar_C : public UPrimaryDataAsset
{
    TMap<FString, int32> Months;                                                      // 0x0030 (size: 0x50)
    bool Uses Leap Year;                                                              // 0x0080 (size: 0x1)
    int32 Leap Year Month;                                                            // 0x0084 (size: 0x4)
    int32 Winter Solstice Offset;                                                     // 0x0088 (size: 0x4)
    TArray<FString> Days of the Week;                                                 // 0x0090 (size: 0x10)
    int32 Day of the Week Offset;                                                     // 0x00A0 (size: 0x4)
    TArray<int32> Month Lengths;                                                      // 0x00A8 (size: 0x10)
    TArray<int32> Day Count At Start of Each Month;                                   // 0x00B8 (size: 0x10)
    TArray<int32> Day Count At Start of Each Month (Leap Year);                       // 0x00C8 (size: 0x10)
    TArray<int32> Month Lengths (Leap Year);                                          // 0x00D8 (size: 0x10)
    int32 Number of Days in Year;                                                     // 0x00E8 (size: 0x4)
    bool Calendar Data Saved;                                                         // 0x00EC (size: 0x1)

}; // Size: 0xED

#endif

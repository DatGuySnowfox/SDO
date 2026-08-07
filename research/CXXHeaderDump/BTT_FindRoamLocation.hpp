#ifndef UE4SS_SDK_BTT_FindRoamLocation_HPP
#define UE4SS_SDK_BTT_FindRoamLocation_HPP

class UBTT_FindRoamLocation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)
    FVector Move To Location;                                                         // 0x00B8 (size: 0x18)
    int32 Find Location Checks;                                                       // 0x00D0 (size: 0x4)

    void Roam Location(FVector& Location);
    void Base Roam Location(FVector& Location);
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_FindRoamLocation(int32 EntryPoint);
}; // Size: 0xD4

#endif

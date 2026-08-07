#ifndef UE4SS_SDK_BTT_ClimbPath_HPP
#define UE4SS_SDK_BTT_ClimbPath_HPP

class UBTT_ClimbPath_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* Smart AI;                                           // 0x00B0 (size: 0x8)
    FVector EQS Locaton Result;                                                       // 0x00B8 (size: 0x18)

    void Location(class UEnvQueryInstanceBlueprintWrapper* QueryInstance, TEnumAsByte<EEnvQueryStatus::Type> QueryStatus);
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_ClimbPath(int32 EntryPoint);
}; // Size: 0xD0

#endif

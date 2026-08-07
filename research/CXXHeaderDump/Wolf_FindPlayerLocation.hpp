#ifndef UE4SS_SDK_Wolf_FindPlayerLocation_HPP
#define UE4SS_SDK_Wolf_FindPlayerLocation_HPP

class UWolf_FindPlayerLocation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Target;                                                    // 0x00B0 (size: 0x28)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_Wolf_FindPlayerLocation(int32 EntryPoint);
}; // Size: 0xD8

#endif

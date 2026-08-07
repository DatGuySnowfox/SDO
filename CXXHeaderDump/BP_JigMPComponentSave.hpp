#ifndef UE4SS_SDK_BP_JigMPComponentSave_HPP
#define UE4SS_SDK_BP_JigMPComponentSave_HPP

class UBP_JigMPComponentSave_C : public USaveGame
{
    TArray<FS_ReplicatedContainerInfo> JigContainers;                                 // 0x0028 (size: 0x10)
    TArray<FGuid> MainUIDs;                                                           // 0x0038 (size: 0x10)
    TArray<FS_InvWeight> Weights;                                                     // 0x0048 (size: 0x10)
    TArray<FGuid> ActionbarUIDs;                                                      // 0x0058 (size: 0x10)
    FVector PlayerLocation;                                                           // 0x0068 (size: 0x18)
    FString MapName;                                                                  // 0x0080 (size: 0x10)

}; // Size: 0x90

#endif

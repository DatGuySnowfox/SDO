#ifndef UE4SS_SDK_BP_InfestationManager_HPP
#define UE4SS_SDK_BP_InfestationManager_HPP

class ABP_InfestationManager_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    TArray<class ABP_PrimaryInfestation_C*> PrimaryInfestations;                      // 0x02A8 (size: 0x10)

    void GetAllPrimaries();
    void AreAllPrimariesDestroyed(bool& AllDestroyed?);
    void OnCompleted_B3F7F6984C752DC85F8BB59455E585B9();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_InfestationManager(int32 EntryPoint);
}; // Size: 0x2B8

#endif

#ifndef UE4SS_SDK_BP_Zombie_Infected_Horde_HPP
#define UE4SS_SDK_BP_Zombie_Infected_Horde_HPP

class ABP_Zombie_Infected_Horde_C : public ABP_MasterZombie_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x08B0 (size: 0x8)

    void OnNotifyEnd_B68543284987CD1D13B41E99F9665966(FName NotifyName);
    void OnNotifyBegin_B68543284987CD1D13B41E99F9665966(FName NotifyName);
    void OnInterrupted_B68543284987CD1D13B41E99F9665966(FName NotifyName);
    void OnBlendOut_B68543284987CD1D13B41E99F9665966(FName NotifyName);
    void OnCompleted_B68543284987CD1D13B41E99F9665966(FName NotifyName);
    void ReceiveBeginPlay();
    void Event_PlayerDead();
    void ExecuteUbergraph_BP_Zombie_Infected_Horde(int32 EntryPoint);
}; // Size: 0x8B8

#endif

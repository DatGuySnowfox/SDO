#ifndef UE4SS_SDK_BP_Zombie_Radiated_HPP
#define UE4SS_SDK_BP_Zombie_Radiated_HPP

class ABP_Zombie_Radiated_C : public ABP_MasterZombie_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0848 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_Zombie_Radiated(int32 EntryPoint);
}; // Size: 0x850

#endif

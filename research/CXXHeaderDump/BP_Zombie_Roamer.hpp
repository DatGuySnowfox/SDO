#ifndef UE4SS_SDK_BP_Zombie_Roamer_HPP
#define UE4SS_SDK_BP_Zombie_Roamer_HPP

class ABP_Zombie_Roamer_C : public ABP_MasterZombie_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x08B0 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_Zombie_Roamer(int32 EntryPoint);
}; // Size: 0x8B8

#endif

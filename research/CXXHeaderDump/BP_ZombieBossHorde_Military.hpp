#ifndef UE4SS_SDK_BP_ZombieBossHorde_Military_HPP
#define UE4SS_SDK_BP_ZombieBossHorde_Military_HPP

class ABP_ZombieBossHorde_Military_C : public ABP_ZombieBoss_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0780 (size: 0x8)

    void ReceiveBeginPlay();
    void Event_PlayerDead();
    void ExecuteUbergraph_BP_ZombieBossHorde_Military(int32 EntryPoint);
}; // Size: 0x788

#endif

#ifndef UE4SS_SDK_LockPickingComponent_HPP
#define UE4SS_SDK_LockPickingComponent_HPP

class ULockPickingComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    bool PlayerLockPicking?;                                                          // 0x00C0 (size: 0x1)
    class ULockpickUI_C* UI;                                                          // 0x00C8 (size: 0x8)
    bool IsTryingLock;                                                                // 0x00D0 (size: 0x1)
    double LPDurability;                                                              // 0x00D8 (size: 0x8)

    void Svr_Initialise();
    void ExecuteUbergraph_LockPickingComponent(int32 EntryPoint);
}; // Size: 0xE0

#endif

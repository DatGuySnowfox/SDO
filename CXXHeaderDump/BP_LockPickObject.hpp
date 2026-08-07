#ifndef UE4SS_SDK_BP_LockPickObject_HPP
#define UE4SS_SDK_BP_LockPickObject_HPP

class ABP_LockPickObject_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    TEnumAsByte<Enum_LockpickDifficulty::Type> Difficulty;                            // 0x02A8 (size: 0x1)
    bool IsLocked?;                                                                   // 0x02A9 (size: 0x1)
    TArray<double> LockpickRange;                                                     // 0x02B0 (size: 0x10)

    void LockpickSuccessful();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_LockPickObject(int32 EntryPoint);
}; // Size: 0x2C0

#endif

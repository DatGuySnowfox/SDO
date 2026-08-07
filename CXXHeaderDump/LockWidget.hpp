#ifndef UE4SS_SDK_LockWidget_HPP
#define UE4SS_SDK_LockWidget_HPP

class ULockWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* Lock;                                                               // 0x02C8 (size: 0x8)
    class UOverlay* MovingLock;                                                       // 0x02D0 (size: 0x8)
    class UImage* MovingLockBackdrop;                                                 // 0x02D8 (size: 0x8)
    class UImage* Pin;                                                                // 0x02E0 (size: 0x8)
    class UImage* Screwdriver;                                                        // 0x02E8 (size: 0x8)

    void Construct();
    void ExecuteUbergraph_LockWidget(int32 EntryPoint);
}; // Size: 0x2F0

#endif

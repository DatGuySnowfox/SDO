#ifndef UE4SS_SDK_LockWidget_HPP
#define UE4SS_SDK_LockWidget_HPP

class ULockWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* Screwdriver;                                                        // 0x0348 (size: 0x8)
    class UImage* Pin;                                                                // 0x0350 (size: 0x8)
    class UImage* MovingLockBackdrop;                                                 // 0x0358 (size: 0x8)
    class UOverlay* MovingLock;                                                       // 0x0360 (size: 0x8)
    class UImage* Lock;                                                               // 0x0368 (size: 0x8)

    void Construct();
    void ExecuteUbergraph_LockWidget(int32 EntryPoint);
}; // Size: 0x370

#endif

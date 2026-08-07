#ifndef UE4SS_SDK_BP_AudioPoint_HPP
#define UE4SS_SDK_BP_AudioPoint_HPP

class ABP_AudioPoint_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x02A8 (size: 0x8)
    class USoundBase* Sound;                                                          // 0x02B0 (size: 0x8)

    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_AudioPoint(int32 EntryPoint);
}; // Size: 0x2B8

#endif

#ifndef UE4SS_SDK_BP_SkySettings_HPP
#define UE4SS_SDK_BP_SkySettings_HPP

class ABP_SkySettings_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)

    void ReceiveBeginPlay();
    void Saturation(float NewValue);
    void NightBright(float NewValue);
    void NightLength(float NewValue);
    void DayLength(float NewValue);
    void ExecuteUbergraph_BP_SkySettings(int32 EntryPoint);
}; // Size: 0x2A8

#endif

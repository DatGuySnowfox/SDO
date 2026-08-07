#ifndef UE4SS_SDK_BP_RangeFinderLocalAttachment_HPP
#define UE4SS_SDK_BP_RangeFinderLocalAttachment_HPP

class ABP_RangeFinderLocalAttachment_C : public ABP_AMainLocalAttachment_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    class UBatteryComponent_C* BatteryComponent;                                      // 0x02D8 (size: 0x8)
    FTimerHandle BatteryTimer;                                                        // 0x02E0 (size: 0x8)
    FTimerHandle RangeFinderTimer;                                                    // 0x02E8 (size: 0x8)

    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void Event_Battery(bool TurnOn);
    void Battery_Flashlight();
    void Event_RangeFinderTrace();
    void Event_RF(bool TurnOn);
    void ExecuteUbergraph_BP_RangeFinderLocalAttachment(int32 EntryPoint);
}; // Size: 0x2F0

#endif

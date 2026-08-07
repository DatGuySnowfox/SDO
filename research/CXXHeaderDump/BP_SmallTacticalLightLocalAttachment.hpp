#ifndef UE4SS_SDK_BP_SmallTacticalLightLocalAttachment_HPP
#define UE4SS_SDK_BP_SmallTacticalLightLocalAttachment_HPP

class ABP_SmallTacticalLightLocalAttachment_C : public ABP_AMainLocalAttachment_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    class UBatteryComponent_C* BatteryComponent;                                      // 0x02D8 (size: 0x8)
    class USpotLightComponent* SpotLight;                                             // 0x02E0 (size: 0x8)
    FTimerHandle BatteryTimer;                                                        // 0x02E8 (size: 0x8)

    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void Event_Battery(bool TurnOn);
    void Battery_Flashlight();
    void ExecuteUbergraph_BP_SmallTacticalLightLocalAttachment(int32 EntryPoint);
}; // Size: 0x2F0

#endif

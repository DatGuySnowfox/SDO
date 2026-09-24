#ifndef UE4SS_SDK_BP_TacticalLaserLightComboLocalAttachment_HPP
#define UE4SS_SDK_BP_TacticalLaserLightComboLocalAttachment_HPP

class ABP_TacticalLaserLightComboLocalAttachment_C : public ABP_AMainLocalAttachment_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02E0 (size: 0x8)
    class UNiagaraComponent* NS_LaserSight;                                           // 0x02E8 (size: 0x8)
    class UBatteryComponent_C* BatteryComponent;                                      // 0x02F0 (size: 0x8)
    class USpotLightComponent* SpotLight;                                             // 0x02F8 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x0300 (size: 0x8)
    FTimerHandle BatteryTimer;                                                        // 0x0308 (size: 0x8)
    FTimerHandle LaserTimer;                                                          // 0x0310 (size: 0x8)

    void LaserEndPoint(FVector& Vector);
    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void Event_Battery(bool TurnOn);
    void Battery_Flashlight();
    void ReceiveDestroyed();
    void Event_Laser();
    void ExecuteUbergraph_BP_TacticalLaserLightComboLocalAttachment(int32 EntryPoint);
}; // Size: 0x318

#endif

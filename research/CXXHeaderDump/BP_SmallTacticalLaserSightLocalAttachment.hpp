#ifndef UE4SS_SDK_BP_SmallTacticalLaserSightLocalAttachment_HPP
#define UE4SS_SDK_BP_SmallTacticalLaserSightLocalAttachment_HPP

class ABP_SmallTacticalLaserSightLocalAttachment_C : public ABP_AMainLocalAttachment_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02E0 (size: 0x8)
    class UNiagaraComponent* NS_LaserSight;                                           // 0x02E8 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x02F0 (size: 0x8)
    FTimerHandle LaserTimer;                                                          // 0x02F8 (size: 0x8)

    void LaserEndPoint(FVector& Vector);
    void ReceiveBeginPlay();
    void Event_Laser();
    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void ExecuteUbergraph_BP_SmallTacticalLaserSightLocalAttachment(int32 EntryPoint);
}; // Size: 0x300

#endif

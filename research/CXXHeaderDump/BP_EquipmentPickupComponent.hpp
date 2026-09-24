#ifndef UE4SS_SDK_BP_EquipmentPickupComponent_HPP
#define UE4SS_SDK_BP_EquipmentPickupComponent_HPP

class UBP_EquipmentPickupComponent_C : public UActorComponent
{
    int32 AnimBlendspace;                                                             // 0x00B8 (size: 0x4)
    class UAnimMontage* EquipMontage;                                                 // 0x00C0 (size: 0x8)
    class UAnimMontage* UnequipMontage;                                               // 0x00C8 (size: 0x8)
    bool IsFirearm?;                                                                  // 0x00D0 (size: 0x1)

}; // Size: 0xD1

#endif

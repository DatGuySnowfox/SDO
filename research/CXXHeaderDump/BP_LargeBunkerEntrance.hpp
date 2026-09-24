#ifndef UE4SS_SDK_BP_LargeBunkerEntrance_HPP
#define UE4SS_SDK_BP_LargeBunkerEntrance_HPP

class ABP_LargeBunkerEntrance_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* DoorR;                                                // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* DoorL;                                                // 0x02C8 (size: 0x8)
    float DoorTimeline_DoorR_540FF6654A72D9E502311B95C0899C77;                        // 0x02D0 (size: 0x4)
    float DoorTimeline_DoorL_540FF6654A72D9E502311B95C0899C77;                        // 0x02D4 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> DoorTimeline__Direction_540FF6654A72D9E502311B95C0899C77; // 0x02D8 (size: 0x1)
    class UTimelineComponent* DoorTimeline;                                           // 0x02E0 (size: 0x8)
    bool Open?;                                                                       // 0x02E8 (size: 0x1)
    class USoundBase* Sound;                                                          // 0x02F0 (size: 0x8)

    void DoorTimeline__FinishedFunc();
    void DoorTimeline__UpdateFunc();
    void SetPower_TurnedOn(bool TurnOn);
    void SetPower_Warning(bool SetWarning);
    void CallInteractOnObject();
    void ExecuteUbergraph_BP_LargeBunkerEntrance(int32 EntryPoint);
}; // Size: 0x2F8

#endif

#ifndef UE4SS_SDK_BP_DoubleBunkerDoor_HPP
#define UE4SS_SDK_BP_DoubleBunkerDoor_HPP

class ABP_DoubleBunkerDoor_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Door2;                                                // 0x02B8 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* Door;                                                 // 0x02C8 (size: 0x8)
    float DoorTimeline_DoorL_006AE8DB4E978995669D27877E0F44AE;                        // 0x02D0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> DoorTimeline__Direction_006AE8DB4E978995669D27877E0F44AE; // 0x02D4 (size: 0x1)
    class UTimelineComponent* DoorTimeline;                                           // 0x02D8 (size: 0x8)
    bool Open?;                                                                       // 0x02E0 (size: 0x1)
    class UStaticMesh* Mesh;                                                          // 0x02E8 (size: 0x8)
    class USoundBase* Sound;                                                          // 0x02F0 (size: 0x8)

    void UserConstructionScript();
    void DoorTimeline__FinishedFunc();
    void DoorTimeline__UpdateFunc();
    void SetPower_TurnedOn(bool TurnOn);
    void SetPower_Warning(bool SetWarning);
    void CallInteractOnObject();
    void ExecuteUbergraph_BP_DoubleBunkerDoor(int32 EntryPoint);
}; // Size: 0x2F8

#endif

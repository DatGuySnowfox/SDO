#ifndef UE4SS_SDK_BP_PoliceStationGarageDoor_HPP
#define UE4SS_SDK_BP_PoliceStationGarageDoor_HPP

class ABP_PoliceStationGarageDoor_C : public ABP_MasterDoor_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USkeletalMeshComponent* Door;                                               // 0x02A8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B0 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02B8 (size: 0x8)
    bool Open?;                                                                       // 0x02C0 (size: 0x1)
    bool DoorOpening?;                                                                // 0x02C1 (size: 0x1)
    bool Powered?;                                                                    // 0x02C2 (size: 0x1)

    void OnNotifyEnd_2203FE894E82A21CC5DE86828FE291C5(FName NotifyName);
    void OnNotifyBegin_2203FE894E82A21CC5DE86828FE291C5(FName NotifyName);
    void OnInterrupted_2203FE894E82A21CC5DE86828FE291C5(FName NotifyName);
    void OnBlendOut_2203FE894E82A21CC5DE86828FE291C5(FName NotifyName);
    void OnCompleted_2203FE894E82A21CC5DE86828FE291C5(FName NotifyName);
    void OnNotifyEnd_56D76FBC4D43E45E4D2A4F8DE1E0A0F3(FName NotifyName);
    void OnNotifyBegin_56D76FBC4D43E45E4D2A4F8DE1E0A0F3(FName NotifyName);
    void OnInterrupted_56D76FBC4D43E45E4D2A4F8DE1E0A0F3(FName NotifyName);
    void OnBlendOut_56D76FBC4D43E45E4D2A4F8DE1E0A0F3(FName NotifyName);
    void OnCompleted_56D76FBC4D43E45E4D2A4F8DE1E0A0F3(FName NotifyName);
    void SetPower_TurnedOn(bool TurnOn);
    void CallInteractOnObject();
    void MC_OpenDoor(class AActor* Actor);
    void SetPower_Warning(bool SetWarning);
    void Event_OpenDoor();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_PoliceStationGarageDoor(int32 EntryPoint);
}; // Size: 0x2C3

#endif

#ifndef UE4SS_SDK_BP_Event_JetActor_HPP
#define UE4SS_SDK_BP_Event_JetActor_HPP

class ABP_Event_JetActor_C : public ABP_EventMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x02A8 (size: 0x8)
    class UArrowComponent* DirectionOfLineTracer;                                     // 0x02B0 (size: 0x8)
    class UArrowComponent* DirectionOfFlight;                                         // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C0 (size: 0x8)
    double Speed;                                                                     // 0x02C8 (size: 0x8)

    void ReceiveTick(float DeltaSeconds);
    void ExecuteUbergraph_BP_Event_JetActor(int32 EntryPoint);
}; // Size: 0x2D0

#endif

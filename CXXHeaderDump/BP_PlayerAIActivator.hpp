#ifndef UE4SS_SDK_BP_PlayerAIActivator_HPP
#define UE4SS_SDK_BP_PlayerAIActivator_HPP

class UBP_PlayerAIActivator_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    double AI Deactivation Proximity;                                                 // 0x00A8 (size: 0x8)
    double AI Activation Proximity;                                                   // 0x00B0 (size: 0x8)
    TArray<class AActor*> Last Proximity Activation;                                  // 0x00B8 (size: 0x10)
    TArray<class AActor*> Last Proximity Deactivation;                                // 0x00C8 (size: 0x10)

    void Update Proximity Deactivation();
    void Update Proximity Activation();
    void Server Start Activator Timer();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_PlayerAIActivator(int32 EntryPoint);
}; // Size: 0xD8

#endif

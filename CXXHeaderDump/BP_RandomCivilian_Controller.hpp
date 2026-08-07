#ifndef UE4SS_SDK_BP_RandomCivilian_Controller_HPP
#define UE4SS_SDK_BP_RandomCivilian_Controller_HPP

class ABP_RandomCivilian_Controller_C : public ABP_MainFriendlyAIController_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x03C0 (size: 0x8)
    class AActor* SensedActor;                                                        // 0x03C8 (size: 0x8)

    void ReceiveBeginPlay();
    void Death(class AActor* Actor);
    void ExecuteUbergraph_BP_RandomCivilian_Controller(int32 EntryPoint);
}; // Size: 0x3D0

#endif

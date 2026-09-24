#ifndef UE4SS_SDK_BP_CrossbowPickup_HPP
#define UE4SS_SDK_BP_CrossbowPickup_HPP

class ABP_CrossbowPickup_C : public ABP_FirearmPickup_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0420 (size: 0x8)
    class UStaticMeshComponent* Bolt;                                                 // 0x0428 (size: 0x8)

    void ReceiveBeginPlay();
    void Event_Fire();
    void Jig_OnMontageNotify(FName NotifyName);
    void ExecuteUbergraph_BP_CrossbowPickup(int32 EntryPoint);
}; // Size: 0x430

#endif

#ifndef UE4SS_SDK_BP_AlternatePOI_Listener_HPP
#define UE4SS_SDK_BP_AlternatePOI_Listener_HPP

class ABP_AlternatePOI_Listener_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class ABP_AlternatePOI_C* AlternatePOIToChange;                                   // 0x02A8 (size: 0x8)

    void ReceiveBeginPlay();
    void SetAlternate();
    void ExecuteUbergraph_BP_AlternatePOI_Listener(int32 EntryPoint);
}; // Size: 0x2B0

#endif

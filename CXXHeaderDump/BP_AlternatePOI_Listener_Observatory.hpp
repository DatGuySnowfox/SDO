#ifndef UE4SS_SDK_BP_AlternatePOI_Listener_Observatory_HPP
#define UE4SS_SDK_BP_AlternatePOI_Listener_Observatory_HPP

class ABP_AlternatePOI_Listener_Observatory_C : public ABP_AlternatePOI_Listener_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_AlternatePOI_Listener_Observatory(int32 EntryPoint);
}; // Size: 0x2B8

#endif

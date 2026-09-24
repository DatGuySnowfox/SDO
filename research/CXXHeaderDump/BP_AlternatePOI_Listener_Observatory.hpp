#ifndef UE4SS_SDK_BP_AlternatePOI_Listener_Observatory_HPP
#define UE4SS_SDK_BP_AlternatePOI_Listener_Observatory_HPP

class ABP_AlternatePOI_Listener_Observatory_C : public ABP_AlternatePOI_Listener_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C8 (size: 0x8)

    void OnCompleted_6C1985B9478DA674691C0F8AE6B36548();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_AlternatePOI_Listener_Observatory(int32 EntryPoint);
}; // Size: 0x2D0

#endif

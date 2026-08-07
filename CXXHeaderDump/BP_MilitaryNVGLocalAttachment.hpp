#ifndef UE4SS_SDK_BP_MilitaryNVGLocalAttachment_HPP
#define UE4SS_SDK_BP_MilitaryNVGLocalAttachment_HPP

class ABP_MilitaryNVGLocalAttachment_C : public ABP_AMainLocalAttachment_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    float NVGTL_TL_AEEF810E440DD1FEDF79D39833A5FF84;                                  // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> NVGTL__Direction_AEEF810E440DD1FEDF79D39833A5FF84; // 0x02DC (size: 0x1)
    class UTimelineComponent* NVGTL;                                                  // 0x02E0 (size: 0x8)

    void NVGTL__FinishedFunc();
    void NVGTL__UpdateFunc();
    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void ExecuteUbergraph_BP_MilitaryNVGLocalAttachment(int32 EntryPoint);
}; // Size: 0x2E8

#endif

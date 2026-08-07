#ifndef UE4SS_SDK_BP_SpecOpsNVGLocalAttachment_HPP
#define UE4SS_SDK_BP_SpecOpsNVGLocalAttachment_HPP

class ABP_SpecOpsNVGLocalAttachment_C : public ABP_AMainLocalAttachment_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    float NVGTL_TL_F30B97124F77A6BEA413A1B19A8FF24D;                                  // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> NVGTL__Direction_F30B97124F77A6BEA413A1B19A8FF24D; // 0x02DC (size: 0x1)
    class UTimelineComponent* NVGTL;                                                  // 0x02E0 (size: 0x8)

    void NVGTL__FinishedFunc();
    void NVGTL__UpdateFunc();
    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void ExecuteUbergraph_BP_SpecOpsNVGLocalAttachment(int32 EntryPoint);
}; // Size: 0x2E8

#endif

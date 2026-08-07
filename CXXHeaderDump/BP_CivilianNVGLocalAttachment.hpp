#ifndef UE4SS_SDK_BP_CivilianNVGLocalAttachment_HPP
#define UE4SS_SDK_BP_CivilianNVGLocalAttachment_HPP

class ABP_CivilianNVGLocalAttachment_C : public ABP_AMainLocalAttachment_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    float NVGTL_TL_9DF085DC414A828FAE04DBA2A9257E5B;                                  // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> NVGTL__Direction_9DF085DC414A828FAE04DBA2A9257E5B; // 0x02DC (size: 0x1)
    class UTimelineComponent* NVGTL;                                                  // 0x02E0 (size: 0x8)

    void NVGTL__FinishedFunc();
    void NVGTL__UpdateFunc();
    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void ExecuteUbergraph_BP_CivilianNVGLocalAttachment(int32 EntryPoint);
}; // Size: 0x2E8

#endif

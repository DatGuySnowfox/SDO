#ifndef UE4SS_SDK_BP_KeycardBriefcaseContainer_HPP
#define UE4SS_SDK_BP_KeycardBriefcaseContainer_HPP

class ABP_KeycardBriefcaseContainer_C : public ABP_LootContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_BP_KeycardBriefcaseContainer(int32 EntryPoint);
}; // Size: 0x448

#endif

#ifndef UE4SS_SDK_BP_MasterQuestObject_HPP
#define UE4SS_SDK_BP_MasterQuestObject_HPP

class ABP_MasterQuestObject_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02A8 (size: 0x8)
    FString QuestArgument;                                                            // 0x02B0 (size: 0x10)

    void GetQuestArgument(FString Argument);
    void ExecuteUbergraph_BP_MasterQuestObject(int32 EntryPoint);
}; // Size: 0x2C0

#endif

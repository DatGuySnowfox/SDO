#ifndef UE4SS_SDK_BP_Quest_Spawner_Object_HPP
#define UE4SS_SDK_BP_Quest_Spawner_Object_HPP

class ABP_Quest_Spawner_Object_C : public ABP_MasterQuestObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    class UTextRenderComponent* TextRender;                                           // 0x02D8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02E0 (size: 0x8)
    double ZOffset;                                                                   // 0x02E8 (size: 0x8)
    FBP_Quest_Spawner_Object_CAIDeath AIDeath;                                        // 0x02F0 (size: 0x10)
    void AIDeath();
    FName QuestTag;                                                                   // 0x0300 (size: 0x8)
    TSubclassOf<class AActor> ObjectToSpawn;                                          // 0x0308 (size: 0x8)

    void 1_Spawn Object For Testing();
    void UserConstructionScript();
    void Quest_Spawn();
    void ExecuteUbergraph_BP_Quest_Spawner_Object(int32 EntryPoint);
    void AIDeath__DelegateSignature();
}; // Size: 0x310

#endif

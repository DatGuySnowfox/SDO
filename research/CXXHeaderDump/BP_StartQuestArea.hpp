#ifndef UE4SS_SDK_BP_StartQuestArea_HPP
#define UE4SS_SDK_BP_StartQuestArea_HPP

class ABP_StartQuestArea_C : public ABP_MasterQuestObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02C8 (size: 0x8)
    FTimerHandle OverlapTimer;                                                        // 0x02D0 (size: 0x8)
    TSubclassOf<class UQuest> Quest;                                                  // 0x02D8 (size: 0x8)
    bool Debug?;                                                                      // 0x02E0 (size: 0x1)

    void UserConstructionScript();
    void ReceiveBeginPlay();
    void Event_Overlap();
    void BndEvt__BP_StartQuestArea_Box_K2Node_ComponentBoundEvent_2_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void ExecuteUbergraph_BP_StartQuestArea(int32 EntryPoint);
}; // Size: 0x2E1

#endif

#ifndef UE4SS_SDK_Journal_InfoUI_HPP
#define UE4SS_SDK_Journal_InfoUI_HPP

class UJournal_InfoUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* ZombiesKilledBox;                                               // 0x0348 (size: 0x8)
    class UTextBlock* YearText;                                                       // 0x0350 (size: 0x8)
    class UTextBlock* TempText;                                                       // 0x0358 (size: 0x8)
    class UImage* SkillsMarker;                                                       // 0x0360 (size: 0x8)
    class UTextBlock* SexBox;                                                         // 0x0368 (size: 0x8)
    class UImage* QuestMarker;                                                        // 0x0370 (size: 0x8)
    class UTextBlock* PlayerLevelText;                                                // 0x0378 (size: 0x8)
    class UImage* Note;                                                               // 0x0380 (size: 0x8)
    class UTextBlock* NameBox;                                                        // 0x0388 (size: 0x8)
    class UTextBlock* MonthText;                                                      // 0x0390 (size: 0x8)
    class UTextBlock* MinuteText;                                                     // 0x0398 (size: 0x8)
    class UImage* License;                                                            // 0x03A0 (size: 0x8)
    class UTextBlock* JobBox;                                                         // 0x03A8 (size: 0x8)
    class UImage* InfoMarker;                                                         // 0x03B0 (size: 0x8)
    class UTextBlock* InfestationsDestroyedBox;                                       // 0x03B8 (size: 0x8)
    class UTextBlock* HumansKilledBox;                                                // 0x03C0 (size: 0x8)
    class UTextBlock* HourText;                                                       // 0x03C8 (size: 0x8)
    class UTextBlock* DistanceTravelledBox;                                           // 0x03D0 (size: 0x8)
    class UTextBlock* DayText;                                                        // 0x03D8 (size: 0x8)
    class UTextBlock* DaysSurvivedBox;                                                // 0x03E0 (size: 0x8)
    class UTextBlock* DateSlashText;                                                  // 0x03E8 (size: 0x8)
    class UImage* Clock;                                                              // 0x03F0 (size: 0x8)
    class UTextBlock* BossZombiesKilledBox;                                           // 0x03F8 (size: 0x8)
    class UTextBlock* AnimalsKilledBox;                                               // 0x0400 (size: 0x8)
    class UTextBlock* AgeBox;                                                         // 0x0408 (size: 0x8)
    class ABP_PlayerController_C* Controller;                                         // 0x0410 (size: 0x8)

    void UpdateLevelUI(int32 Value);
    void BindEvents();
    FText GetAge();
    FText GetSex();
    FText GetJob();
    FText GetName();
    FText GetZombiesKilled();
    FText GetBossZombiesKilled();
    FText GetHumansKilled();
    FText GetAnimalsKilled();
    FText GetInfestationsDestroyed();
    FText GetDistanceTravelled();
    FText GetDaysSurvived();
    void OnCompleted_604A8261490D580D021134B619D9CBAD();
    void Construct();
    void EventSetTime();
    void ExecuteUbergraph_Journal_InfoUI(int32 EntryPoint);
}; // Size: 0x418

#endif

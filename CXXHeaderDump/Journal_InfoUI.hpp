#ifndef UE4SS_SDK_Journal_InfoUI_HPP
#define UE4SS_SDK_Journal_InfoUI_HPP

class UJournal_InfoUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* AgeBox;                                                         // 0x02C8 (size: 0x8)
    class UTextBlock* AnimalsKilledBox;                                               // 0x02D0 (size: 0x8)
    class UTextBlock* BossZombiesKilledBox;                                           // 0x02D8 (size: 0x8)
    class UImage* Clock;                                                              // 0x02E0 (size: 0x8)
    class UTextBlock* DateSlashText;                                                  // 0x02E8 (size: 0x8)
    class UTextBlock* DaysSurvivedBox;                                                // 0x02F0 (size: 0x8)
    class UTextBlock* DayText;                                                        // 0x02F8 (size: 0x8)
    class UTextBlock* DistanceTravelledBox;                                           // 0x0300 (size: 0x8)
    class UTextBlock* HourText;                                                       // 0x0308 (size: 0x8)
    class UTextBlock* HumansKilledBox;                                                // 0x0310 (size: 0x8)
    class UTextBlock* InfestationsDestroyedBox;                                       // 0x0318 (size: 0x8)
    class UImage* InfoMarker;                                                         // 0x0320 (size: 0x8)
    class UTextBlock* JobBox;                                                         // 0x0328 (size: 0x8)
    class UImage* License;                                                            // 0x0330 (size: 0x8)
    class UTextBlock* MinuteText;                                                     // 0x0338 (size: 0x8)
    class UTextBlock* MonthText;                                                      // 0x0340 (size: 0x8)
    class UTextBlock* NameBox;                                                        // 0x0348 (size: 0x8)
    class UImage* Note;                                                               // 0x0350 (size: 0x8)
    class UTextBlock* PlayerLevelText;                                                // 0x0358 (size: 0x8)
    class UImage* QuestMarker;                                                        // 0x0360 (size: 0x8)
    class UTextBlock* SeasonText;                                                     // 0x0368 (size: 0x8)
    class UTextBlock* SexBox;                                                         // 0x0370 (size: 0x8)
    class UImage* SkillsMarker;                                                       // 0x0378 (size: 0x8)
    class UTextBlock* YearText;                                                       // 0x0380 (size: 0x8)
    class UTextBlock* ZombiesKilledBox;                                               // 0x0388 (size: 0x8)
    class ABP_PlayerController_C* Controller;                                         // 0x0390 (size: 0x8)

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
}; // Size: 0x398

#endif

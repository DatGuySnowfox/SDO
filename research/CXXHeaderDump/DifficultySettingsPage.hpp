#ifndef UE4SS_SDK_DifficultySettingsPage_HPP
#define UE4SS_SDK_DifficultySettingsPage_HPP

class UDifficultySettingsPage_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UVerticalRadioSelect_C* CategorySelect;                                     // 0x02C8 (size: 0x8)
    class UWidgetSwitcher* CategorySwitcher;                                          // 0x02D0 (size: 0x8)
    class UDifficulty_AI_C* Difficulty_AI_UI;                                         // 0x02D8 (size: 0x8)
    class UDifficulty_General_C* Difficulty_GeneralUI;                                // 0x02E0 (size: 0x8)
    class UDifficulty_Loot_C* Difficulty_Loot;                                        // 0x02E8 (size: 0x8)
    class UDifficulty_Other_C* Difficulty_Other;                                      // 0x02F0 (size: 0x8)
    class UDifficulty_Player_C* Difficulty_Player;                                    // 0x02F8 (size: 0x8)
    class UDifficulty_Vehicles_C* Difficulty_Vehicles;                                // 0x0300 (size: 0x8)
    FSlateColor HoveredTextColor;                                                     // 0x0308 (size: 0x14)

    void CategoryChanged(FString Value);
    void Construct();
    void ExecuteUbergraph_DifficultySettingsPage(int32 EntryPoint);
}; // Size: 0x31C

#endif

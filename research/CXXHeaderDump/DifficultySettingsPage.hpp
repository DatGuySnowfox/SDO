#ifndef UE4SS_SDK_DifficultySettingsPage_HPP
#define UE4SS_SDK_DifficultySettingsPage_HPP

class UDifficultySettingsPage_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UDifficulty_Vehicles_C* Difficulty_Vehicles;                                // 0x0348 (size: 0x8)
    class UDifficulty_Player_C* Difficulty_Player;                                    // 0x0350 (size: 0x8)
    class UDifficulty_Other_C* Difficulty_Other;                                      // 0x0358 (size: 0x8)
    class UDifficulty_Loot_C* Difficulty_Loot;                                        // 0x0360 (size: 0x8)
    class UDifficulty_General_C* Difficulty_GeneralUI;                                // 0x0368 (size: 0x8)
    class UDifficulty_AI_C* Difficulty_AI_UI;                                         // 0x0370 (size: 0x8)
    class UWidgetSwitcher* CategorySwitcher;                                          // 0x0378 (size: 0x8)
    class UVerticalRadioSelect_C* CategorySelect;                                     // 0x0380 (size: 0x8)
    FSlateColor HoveredTextColor;                                                     // 0x0388 (size: 0x14)

    void CategoryChanged(FString Value);
    void Construct();
    void ExecuteUbergraph_DifficultySettingsPage(int32 EntryPoint);
}; // Size: 0x39C

#endif

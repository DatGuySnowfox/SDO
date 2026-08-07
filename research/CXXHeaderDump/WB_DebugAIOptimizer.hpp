#ifndef UE4SS_SDK_WB_DebugAIOptimizer_HPP
#define UE4SS_SDK_WB_DebugAIOptimizer_HPP

class UWB_DebugAIOptimizer_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* Text_Despawned_1;                                               // 0x02C8 (size: 0x8)
    class UTextBlock* Text_NotUpdated_1;                                              // 0x02D0 (size: 0x8)
    class UTextBlock* Text_PendingDespawn_1;                                          // 0x02D8 (size: 0x8)
    class UTextBlock* Text_PendingSpawn_1;                                            // 0x02E0 (size: 0x8)
    class UTextBlock* Text_Spawned_1;                                                 // 0x02E8 (size: 0x8)
    class UTextBlock* Text_SpawnedClose_1;                                            // 0x02F0 (size: 0x8)
    class UTextBlock* Text_SpawnedFar_1;                                              // 0x02F8 (size: 0x8)
    class UTextBlock* Text_SpawnedMedium_1;                                           // 0x0300 (size: 0x8)
    bool DrawDistanceTexts;                                                           // 0x0308 (size: 0x1)
    bool DrawDebugLines;                                                              // 0x0309 (size: 0x1)
    FTimerHandle TimerHandle_DrawDebug;                                               // 0x0310 (size: 0x8)
    double UpdateInterval;                                                            // 0x0318 (size: 0x8)
    bool ShowDespawnedSubjects;                                                       // 0x0320 (size: 0x1)
    bool SubsystemEnabled;                                                            // 0x0321 (size: 0x1)

    void DrawDebug();
    void Construct();
    void Destruct();
    void OnSubsystemEnabledChanged_Event_0(bool bIsEnabled);
    void ExecuteUbergraph_WB_DebugAIOptimizer(int32 EntryPoint);
}; // Size: 0x322

#endif

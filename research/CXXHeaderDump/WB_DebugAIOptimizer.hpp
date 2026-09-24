#ifndef UE4SS_SDK_WB_DebugAIOptimizer_HPP
#define UE4SS_SDK_WB_DebugAIOptimizer_HPP

class UWB_DebugAIOptimizer_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Text_SpawnedMedium_1;                                           // 0x0348 (size: 0x8)
    class UTextBlock* Text_SpawnedFar_1;                                              // 0x0350 (size: 0x8)
    class UTextBlock* Text_SpawnedClose_1;                                            // 0x0358 (size: 0x8)
    class UTextBlock* Text_Spawned_1;                                                 // 0x0360 (size: 0x8)
    class UTextBlock* Text_PendingSpawn_1;                                            // 0x0368 (size: 0x8)
    class UTextBlock* Text_PendingDespawn_1;                                          // 0x0370 (size: 0x8)
    class UTextBlock* Text_NotUpdated_1;                                              // 0x0378 (size: 0x8)
    class UTextBlock* Text_Despawned_1;                                               // 0x0380 (size: 0x8)
    bool DrawDistanceTexts;                                                           // 0x0388 (size: 0x1)
    bool DrawDebugLines;                                                              // 0x0389 (size: 0x1)
    FTimerHandle TimerHandle_DrawDebug;                                               // 0x0390 (size: 0x8)
    double UpdateInterval;                                                            // 0x0398 (size: 0x8)
    bool ShowDespawnedSubjects;                                                       // 0x03A0 (size: 0x1)
    bool SubsystemEnabled;                                                            // 0x03A1 (size: 0x1)

    void DrawDebug();
    void Construct();
    void Destruct();
    void OnSubsystemEnabledChanged_Event_0(bool bIsEnabled);
    void ExecuteUbergraph_WB_DebugAIOptimizer(int32 EntryPoint);
}; // Size: 0x3A2

#endif

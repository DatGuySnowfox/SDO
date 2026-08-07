#ifndef UE4SS_SDK_BP_SurroundeadGameMode_HPP
#define UE4SS_SDK_BP_SurroundeadGameMode_HPP

class ABP_SurroundeadGameMode_C : public AGameModeBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0338 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x0340 (size: 0x8)
    bool Autosave;                                                                    // 0x0348 (size: 0x1)
    class UDiscordRpc* DiscordRpc;                                                    // 0x0350 (size: 0x8)

    void ResetStats();
    void AutoSaveGame();
    void ReceiveBeginPlay();
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void Event_BindDaysSurvived();
    void EventBindMidnight();
    void Event_AutoSave();
    void EventSave(bool NewValue);
    void ExecuteUbergraph_BP_SurroundeadGameMode(int32 EntryPoint);
}; // Size: 0x358

#endif

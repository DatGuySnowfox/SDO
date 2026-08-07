#ifndef UE4SS_SDK_BP_PlayerController_HPP
#define UE4SS_SDK_BP_PlayerController_HPP

class ABP_PlayerController_C : public ABP_MasterPlayerController_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0858 (size: 0x8)
    class UNarrativeComponent* NarrativeComponent;                                    // 0x0860 (size: 0x8)
    class ULevellingComponent_C* LevellingComponent;                                  // 0x0868 (size: 0x8)
    class UTechTreeComponent_C* TechTreeComponent;                                    // 0x0870 (size: 0x8)
    class UPassiveSkillsComponent_C* PassiveSkillsComponent;                          // 0x0878 (size: 0x8)
    class UWidgetComponent* Widget;                                                   // 0x0880 (size: 0x8)
    bool AutoSaveEnabled;                                                             // 0x0888 (size: 0x1)
    FStruct_KeepInventory KeepInventory;                                              // 0x0890 (size: 0x30)
    FName Level;                                                                      // 0x08C0 (size: 0x8)
    FString Forename;                                                                 // 0x08C8 (size: 0x10)
    FString Surname;                                                                  // 0x08D8 (size: 0x10)
    FString Sex;                                                                      // 0x08E8 (size: 0x10)
    FString Age;                                                                      // 0x08F8 (size: 0x10)
    TEnumAsByte<Enum_Occupation::Type> Occupation;                                    // 0x0908 (size: 0x1)
    int32 ZombieKills;                                                                // 0x090C (size: 0x4)
    int32 BossZombieKills;                                                            // 0x0910 (size: 0x4)
    int32 AnimalKills;                                                                // 0x0914 (size: 0x4)
    int32 HumanKills;                                                                 // 0x0918 (size: 0x4)
    int32 DaysSurvived;                                                               // 0x091C (size: 0x4)
    double DistanceTravelled;                                                         // 0x0920 (size: 0x8)
    int32 InfestationsDestroyed;                                                      // 0x0928 (size: 0x4)
    bool RespawnPointEnabled;                                                         // 0x092C (size: 0x1)
    FTransform RespawnLoc;                                                            // 0x0930 (size: 0x60)

    void GetJoystickDirection(TEnumAsByte<EJoystickTypes::Type> Stick, FVector2D& StickInput);
    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void LoadPlayerInventoryFromSlot();
    void OnFailed_0E13F8DC4FF1E81D4DC55BB17E7EC154();
    void OnCompleted_0E13F8DC4FF1E81D4DC55BB17E7EC154();
    void OnFailed_156F96D5447DEF3BA76DC896CEEDA381();
    void OnCompleted_156F96D5447DEF3BA76DC896CEEDA381();
    void OnFailed_F36E30354316ADD99BBAD29E4E315233();
    void OnCompleted_F36E30354316ADD99BBAD29E4E315233();
    void OnFailed_9CDBA9034C950D925C1539BE037C23EF();
    void OnCompleted_9CDBA9034C950D925C1539BE037C23EF();
    void ActorPreLoad();
    void ActorSaved();
    void ActorPreSave();
    void ActorLoaded();
    void Death_PlayerStats();
    void Svr_RequestRespawn_SpawnPoint();
    void Svr_RequestRespawnSuicide();
    void Client_SUICIDE();
    void Svr_RequestRespawn_Random();
    void Client_Died();
    void ReceivePossess(class APawn* PossessedPawn);
    void CameraShake(TSubclassOf<class ULegacyCameraShake> CameraShake, bool Force?);
    void CameraShakeClient(TSubclassOf<class ULegacyCameraShake> Shake, float Scale);
    void BindPlayerRespawn();
    void ReceiveBeginPlay();
    void SensitivityChanged(float NewValue);
    void EventLightShafts(bool NewValue);
    void EventVolFog(bool NewValue);
    void LoadGameFromSlot();
    void SaveGameToSlot();
    void ExecuteUbergraph_BP_PlayerController(int32 EntryPoint);
}; // Size: 0x990

#endif

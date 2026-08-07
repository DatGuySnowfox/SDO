#ifndef UE4SS_SDK_GameFunctionLibrary_HPP
#define UE4SS_SDK_GameFunctionLibrary_HPP

class UGameFunctionLibrary_C : public UBlueprintFunctionLibrary
{

    void GetRadiationLevel(TEnumAsByte<Enum_RadiationLevel::Type> Level, class UObject* __WorldContext, double& Value);
    void GetLootTableName(TEnumAsByte<Enum_ContainerLootTables::Type> Selection, class UObject* __WorldContext, FText& Text);
    void GetRandomActorOfClass(TSubclassOf<class AActor> ActorClass, class UObject* __WorldContext, class AActor*& Actor);
    void SetItemsToSell(TEnumAsByte<Enum_VendorTypes::Type> Table, class UBP_JigMultiplayer_C* JigComp, class UObject* __WorldContext);
    void GetBehindObject?(class AActor* Actor, class AActor* Interactor, FVector Location, class UObject* __WorldContext, bool& BehindObject?);
    void LootDegredationMultiplier(double Chance, class UObject* __WorldContext, double& NewChance);
    void GetGameDirectory(class UObject* __WorldContext, FString& Path);
    void SetItemsToCraft(TEnumAsByte<Enum_CraftingTableTypes::Type> Table, class UBP_JigMultiplayer_C* JigComp, class UObject* __WorldContext);
    void GetKey(const class UInputAction* Action, class UObject* __WorldContext, FKey& Key);
    void IsKeyDown(const class UInputAction* Action, class UObject* __WorldContext, bool& Down);
    void LookForMapping(FKeyEvent& Key, const FText Mapping, const class UInputAction* Action, class UObject* __WorldContext, bool& Found);
    void IsOutsideOfPlayerView?(const FVector Loc, class AActor* Actor, class UObject* __WorldContext, bool& NotInPlayerView?);
    void AddUniqueLoot(TArray<FName>& UnqiueItemsSet, class UBP_JigMultiplayer_C* JigComp, int32 Container, class UObject* __WorldContext);
    void GetLockPickingComponent(class UObject* __WorldContext, class ULockPickingComponent_C*& LevellingComponent);
    void SetDifficulty(FStruct_Difficulty Settings, class UObject* __WorldContext);
    void ChangeDifficulty(TEnumAsByte<Enum_Difficulty::Type> Difficulty, class UObject* __WorldContext);
    double XpMultiplierCalc(double Min, double Max, class UObject* __WorldContext);
    void GetCharacterFromController(class AController* Object, class UObject* __WorldContext, class ABP_PlayerCharacter_C*& Character);
    void ScalingDamageMultiplier(double Dmg, class UObject* __WorldContext, double& NewDmg);
    void ScalingHealthMultiplier(double MaxHealth, class UObject* __WorldContext, double& NewMaxHealth);
    void Centre Cursor(class UObject* __WorldContext);
    void GetHUD(class UObject* __WorldContext, class AHUD_Game_C*& HUD);
    void CreateZoneAreaUI(bool Entering Area?, FText Area Name, class UTexture* Texture, FLinearColor Color, class UObject* __WorldContext);
    void CreateNotificationUI(FText Message, class UTexture2D* Icon, FLinearColor Icon Colour, double Delay, bool Force Notification?, class UObject* __WorldContext);
    void GetGameHUD(class UObject* __WorldContext, class AHUD_Game_C*& HUD);
    void LootDifficultySetting(FName CVar, TEnumAsByte<Enum_LootDifficulties::Type> Selection1, class UObject* __WorldContext);
    void LootTypeMultiplier(TEnumAsByte<Enum_LootType::Type> Selection, double Chance, class UObject* __WorldContext, double& Multiplier);
    void SetLootTable_AI_DA(TEnumAsByte<Enum_AILootTables::Type> Table, class UBP_JigMultiplayer_C* JigComp, int32 Container, class UObject* __WorldContext);
    void SetLootTable_Container_DA(TEnumAsByte<Enum_ContainerLootTables::Type> Table, class UBP_JigMultiplayer_C* JigComp, int32 Container, class UObject* __WorldContext);
    void DestroyMainMenuActors(class UObject* __WorldContext);
    void ApplyRadiationDamage(class AActor* Actor, double ChanceForRadDamage, double RadDamage, bool CanMaskProtect?, class UObject* __WorldContext);
    void ApplyMedicalEffect(class AActor* Actor, TArray<FName>& Medical Effects, double ChanceForBleed, double ChanceForHeavyBleed, double ChanceForBrokenBone, class UObject* __WorldContext);
    void GetSkillTreeManager(class UObject* __WorldContext, class UTechTreeComponent_C*& TechTree);
    void GetPassiveSkillsComponent(class UObject* __WorldContext, class UPassiveSkillsComponent_C*& SkillsComponent);
    void GetGameMode(class UObject* __WorldContext, class ABP_SurroundeadGameMode_C*& GameMode);
    void ResetPlayerStats(class UObject* __WorldContext);
    void GetGameInstance(class UObject* __WorldContext, class USD_GameInstance_C*& Instance);
    void DestroyAllActors(class UObject* __WorldContext);
    void GetUDW(class UObject* __WorldContext, class AUltra_Dynamic_Weather_C*& UDW);
    void GetLevellingComponent(class UObject* __WorldContext, class ULevellingComponent_C*& LevellingComponent);
    void CastToController(class UObject* __WorldContext, class ABP_PlayerController_C*& Controller);
    void CastToCharacter(class UObject* __WorldContext, class ABP_PlayerCharacter_C*& Character);
    void GetUDS(class UObject* __WorldContext, class AUltra_Dynamic_Sky_C*& UDS);
}; // Size: 0x28

#endif

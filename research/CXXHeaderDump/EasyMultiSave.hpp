#ifndef UE4SS_SDK_EasyMultiSave_HPP
#define UE4SS_SDK_EasyMultiSave_HPP

#include "EasyMultiSave_enums.hpp"

struct FActorSaveData
{
}; // Size: 0xB0

struct FComponentSaveData
{
}; // Size: 0x80

struct FControllerSaveData
{
}; // Size: 0x38

struct FGameObjectSaveData
{
}; // Size: 0x20

struct FLevelArchive
{
}; // Size: 0x68

struct FLevelScriptSaveData
{
}; // Size: 0x28

struct FLevelStackArchive
{
}; // Size: 0x50

struct FMultiLevelStreamingData
{
}; // Size: 0x70

struct FPawnSaveData
{
}; // Size: 0x50

struct FPlayerArchive
{
}; // Size: 0xB0

struct FPlayerPositionArchive
{
}; // Size: 0x48

struct FPlayerStackArchive
{
}; // Size: 0x100

struct FRawObjectSaveData
{
    class UObject* Object;                                                            // 0x0000 (size: 0x8)
    FString ID;                                                                       // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FSaveSlotInfo
{
    FString Name;                                                                     // 0x0000 (size: 0x10)
    FDateTime Timestamp;                                                              // 0x0010 (size: 0x8)
    FName Level;                                                                      // 0x0018 (size: 0x8)
    TArray<FString> Players;                                                          // 0x0020 (size: 0x10)

}; // Size: 0x30

struct FSaveVersionInfo
{
}; // Size: 0x20

class IEMSActorSaveInterface : public IInterface
{

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void ActorSaved();
    void ActorPreSave();
    void ActorPreLoad();
    void ActorLoaded();
}; // Size: 0x28

class IEMSCompSaveInterface : public IInterface
{

    void ComponentPreSave();
    void ComponentLoaded();
}; // Size: 0x28

class UEMSAsyncCheck : public UBlueprintAsyncActionBase
{
    FEMSAsyncCheckOnCompleted OnCompleted;                                            // 0x0030 (size: 0x10)
    void CheckCompletedPin();
    FEMSAsyncCheckOnFailed OnFailed;                                                  // 0x0040 (size: 0x10)
    void CheckFailedPin();
    class UEMSObject* EMS;                                                            // 0x0050 (size: 0x8)

    class UEMSAsyncCheck* CheckSaveFiles(class UObject* WorldContextObject, ESaveFileCheckType CheckType, FString CustomSaveName, bool bCompareGameVersion);
}; // Size: 0x78

class UEMSAsyncLoadGame : public UBlueprintAsyncActionBase
{
    FEMSAsyncLoadGameOnCompleted OnCompleted;                                         // 0x0030 (size: 0x10)
    void AsyncLoadOutputPin();
    FEMSAsyncLoadGameOnFailed OnFailed;                                               // 0x0040 (size: 0x10)
    void AsyncLoadFailedPin();
    class UEMSObject* EMS;                                                            // 0x0058 (size: 0x8)

    void FinishTaskThreadSafe(const TWeakObjectPtr<class UEMSAsyncLoadGame> InTask);
    void AutoLoadLevelActors(class UEMSObject* EMSObject);
    class UEMSAsyncLoadGame* AsyncLoadActors(class UObject* WorldContextObject, int32 Data, bool bFullReload);
}; // Size: 0x88

class UEMSAsyncSaveGame : public UBlueprintAsyncActionBase
{
    FEMSAsyncSaveGameOnCompleted OnCompleted;                                         // 0x0030 (size: 0x10)
    void AsyncSaveOutputPin();
    FEMSAsyncSaveGameOnFailed OnFailed;                                               // 0x0040 (size: 0x10)
    void AsyncSaveFailedPin();
    class UEMSObject* EMS;                                                            // 0x0058 (size: 0x8)

    void AutoSaveLevelActors(class UEMSObject* EMSObject);
    class UEMSAsyncSaveGame* AsyncSaveActors(class UObject* WorldContextObject, int32 Data);
}; // Size: 0x68

class UEMSAsyncStream : public UBlueprintAsyncActionBase
{
    class ULevel* StreamingLevel;                                                     // 0x0030 (size: 0x8)
    class UEMSObject* EMS;                                                            // 0x0040 (size: 0x8)
    TArray<class AActor*> StreamActors;                                               // 0x0048 (size: 0x10)
    TMap<class FName, class TWeakObjectPtr<AActor>> StreamActorsMap;                  // 0x0058 (size: 0x50)
    FMultiLevelStreamingData PrunedData;                                              // 0x00A8 (size: 0x70)

    bool InitStreamingLoadTask(class UEMSObject* EMSObject, class ULevel* InLevel);
}; // Size: 0x130

class UEMSAsyncWait : public UBlueprintAsyncActionBase
{
    FEMSAsyncWaitOnCompleted OnCompleted;                                             // 0x0030 (size: 0x10)
    void AsyncWaitOutputPin();
    class UEMSObject* EMS;                                                            // 0x0040 (size: 0x8)

    class UEMSAsyncWait* AsyncWaitForOperation(class UObject* WorldContextObject, EAsyncCheckType CheckType);
}; // Size: 0x58

class UEMSCustomSaveGame : public USaveGame
{
    FString SaveGameName;                                                             // 0x0028 (size: 0x10)
    bool bUseSaveSlot;                                                                // 0x0038 (size: 0x1)

}; // Size: 0x50

class UEMSFunctionLibrary : public UBlueprintFunctionLibrary
{

    void SetCurrentSaveUserName(class UObject* WorldContextObject, FString username);
    void SetCurrentSaveGameName(class UObject* WorldContextObject, FString SaveGameName);
    void SetActorSaveProperties(class UObject* WorldContextObject, bool bSkipSave, bool bPersistent, bool bSkipTransform, ELoadedStateMod LoadedState);
    bool SaveRawObject(class AActor* WorldContextActor, FRawObjectSaveData Data);
    bool SavePlayerActorsCustom(class AController* Controller, FString Filename);
    bool SaveCustom(class UObject* WorldContextObject, class UEMSCustomSaveGame* SaveGame);
    class UObject* LoadRawObject(class AActor* WorldContextActor, FRawObjectSaveData Data);
    bool LoadPlayerActorsCustom(class AController* Controller, FString Filename);
    bool IsWorldPartition(class UObject* WorldContextObject);
    bool IsSavingOrLoading(class UObject* WorldContextObject);
    bool IsLevelStreamingActive(class UObject* WorldContextObject);
    class UTexture2D* ImportSaveThumbnail(class UObject* WorldContextObject, FString SaveGameName);
    TArray<FString> GetSortedSaveSlots(class UObject* WorldContextObject);
    class UEMSInfoSaveGame* GetSlotInfoSaveGame(class UObject* WorldContextObject, FString& SaveGameName);
    class UEMSInfoSaveGame* GetNamedSlotInfo(class UObject* WorldContextObject, FString SaveGameName);
    class UEMSCustomSaveGame* GetCustomSave(class UObject* WorldContextObject, TSubclassOf<class UEMSCustomSaveGame> SaveGameClass, FString SaveSlot, FString Filename);
    TArray<FString> GetAllSaveUsers(class UObject* WorldContextObject);
    void ExportSaveThumbnail(class UObject* WorldContextObject, class UTextureRenderTarget2D* TextureRenderTarget, FString SaveGameName);
    bool DoesSaveSlotExist(class UObject* WorldContextObject, FString SaveGameName, bool bComplete);
    void DeleteSaveUser(class UObject* WorldContextObject, FString username);
    void DeleteCustomSave(class UObject* WorldContextObject, class UEMSCustomSaveGame* SaveGame);
    bool DeleteCustomPlayerFile(class UObject* WorldContextObject, FString Filename);
    void DeleteAllSaveDataForSlot(class UObject* WorldContextObject, FString SaveGameName);
    void ClearWorldPartition(class UObject* WorldContextObject);
    void ClearMultiLevelSave(class UObject* WorldContextObject);
}; // Size: 0x28

class UEMSInfoSaveGame : public USaveGame
{
    FSaveSlotInfo SlotInfo;                                                           // 0x0028 (size: 0x30)

}; // Size: 0x58

class UEMSObject : public UGameInstanceSubsystem
{
    FString CurrentSaveGameName;                                                      // 0x0030 (size: 0x10)
    FString CurrentSaveUserName;                                                      // 0x0040 (size: 0x10)
    FEMSObjectOnPlayerLoaded OnPlayerLoaded;                                          // 0x0050 (size: 0x10)
    void EmsLoadPlayerComplete(const class APlayerController* LoadedPlayer);
    FEMSObjectOnLevelLoaded OnLevelLoaded;                                            // 0x0060 (size: 0x10)
    void EmsLoadLevelComplete(const TArray<TSoftObjectPtr<AActor>>& LoadedActors);
    FEMSObjectOnPartitionLoaded OnPartitionLoaded;                                    // 0x0070 (size: 0x10)
    void EmsLoadPartitionComplete(const TArray<TSoftObjectPtr<AActor>>& LoadedActors);
    TArray<TWeakObjectPtr<AActor>> ActorList;                                         // 0x0138 (size: 0x10)
    TMap<class FName, class TWeakObjectPtr<AActor>> ActorMap;                         // 0x0148 (size: 0x50)
    TArray<FLevelArchive> LevelArchiveList;                                           // 0x0198 (size: 0x10)
    FMultiLevelStreamingData MultiLevelStreamData;                                    // 0x01A8 (size: 0x70)
    FPlayerStackArchive PlayerStackData;                                              // 0x0218 (size: 0x100)
    TArray<FActorSaveData> SavedActors;                                               // 0x0318 (size: 0x10)
    TArray<FActorSaveData> SavedActorsPruned;                                         // 0x0328 (size: 0x10)
    TArray<FLevelScriptSaveData> SavedScripts;                                        // 0x0338 (size: 0x10)
    FGameObjectSaveData SavedGameMode;                                                // 0x0348 (size: 0x20)
    FGameObjectSaveData SavedGameState;                                               // 0x0368 (size: 0x20)
    FControllerSaveData SavedController;                                              // 0x0388 (size: 0x38)
    FPawnSaveData SavedPawn;                                                          // 0x03C0 (size: 0x50)
    FGameObjectSaveData SavedPlayerState;                                             // 0x0410 (size: 0x20)
    TMap<class FString, class UEMSInfoSaveGame*> CachedSaveSlots;                     // 0x0430 (size: 0x50)
    TMap<class FString, class UEMSCustomSaveGame*> CachedCustomSaves;                 // 0x0480 (size: 0x50)
    TArray<FActorSaveData> WorldPartitionActors;                                      // 0x04D0 (size: 0x10)
    TArray<FActorSaveData> DestroyedActors;                                           // 0x04E0 (size: 0x10)
    TMap<class TWeakObjectPtr<AActor>, class FGameObjectSaveData> RawObjectData;      // 0x04F0 (size: 0x50)
    TArray<TSoftObjectPtr<AActor>> RealLoadedActors;                                  // 0x0540 (size: 0x10)

}; // Size: 0x550

class UEMSPluginSettings : public UObject
{
    FString DefaultSaveGameName;                                                      // 0x0028 (size: 0x10)
    FSoftClassPath SlotInfoSaveGameClass;                                             // 0x0038 (size: 0x20)
    EFileSaveMethod FileSaveMethod;                                                   // 0x0058 (size: 0x1)
    bool bAutoBackup;                                                                 // 0x0059 (size: 0x1)
    bool bAutoSaveStructs;                                                            // 0x005A (size: 0x1)
    bool bAutoDestroyActors;                                                          // 0x005B (size: 0x1)
    bool bAdvancedSpawnCheck;                                                         // 0x005C (size: 0x1)
    bool bMultiThreadSaving;                                                          // 0x005D (size: 0x1)
    ELoadMethod LoadMethod;                                                           // 0x005E (size: 0x1)
    int32 DeferredLoadStackSize;                                                      // 0x0060 (size: 0x4)
    float AsyncWaitDelay;                                                             // 0x0064 (size: 0x4)
    bool bPreSaveOnGameThread;                                                        // 0x0068 (size: 0x1)
    EMultiLevelSaveMethod MultiLevelSaving;                                           // 0x0069 (size: 0x1)
    bool bDynamicLevelStreaming;                                                      // 0x006A (size: 0x1)
    EWorldPartitionMethod WorldPartitionSaving;                                       // 0x006B (size: 0x1)
    EWorldPartitionInit WorldPartitionInit;                                           // 0x006C (size: 0x1)
    float WorldPartitionInitTime;                                                     // 0x0070 (size: 0x4)
    float WorldPartitionInitThreshold;                                                // 0x0074 (size: 0x4)
    EThumbnailImageFormat ThumbnailFormat;                                            // 0x0078 (size: 0x1)
    uint32 SaveGameVersion;                                                           // 0x007C (size: 0x4)
    EOldPackageEngine MigratedSaveEngineVersion;                                      // 0x0080 (size: 0x1)
    bool bMigratedSaveActorVersionCheck;                                              // 0x0081 (size: 0x1)
    TMap<class FString, class FSoftClassPath> RuntimeClasses;                         // 0x0088 (size: 0x50)
    float WorldPartitionInitPollingRate;                                              // 0x00D8 (size: 0x4)
    bool bPersistentPlayer;                                                           // 0x00DC (size: 0x1)
    bool bPersistentGameMode;                                                         // 0x00DD (size: 0x1)

}; // Size: 0xE0

#endif

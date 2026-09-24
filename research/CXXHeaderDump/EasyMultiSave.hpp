#ifndef UE4SS_SDK_EasyMultiSave_HPP
#define UE4SS_SDK_EasyMultiSave_HPP

#include "EasyMultiSave_enums.hpp"

struct FActorInitContext
{
    class AActor* Actor;                                                              // 0x0000 (size: 0x8)

}; // Size: 0x10

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

struct FLoaderInitData
{
    TMap<FName, TWeakObjectPtr<class AActor>> InActorMap;                             // 0x0018 (size: 0x50)

}; // Size: 0x68

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
    TArray<FName> Levels;                                                             // 0x0020 (size: 0x10)
    TArray<FString> Players;                                                          // 0x0030 (size: 0x10)

}; // Size: 0x40

class IEMSActorSaveInterface : public IInterface
{

    void ComponentsToSave(TArray<UActorComponent*>& Components);
    void ActorSaved();
    void ActorPreSave();
    void ActorPreLoad();
    void ActorLoaded();
}; // Size: 0x28

class IEMSCompSaveInterface : public IInterface
{

    void ComponentSaved();
    void ComponentPreSave();
    void ComponentPreLoad();
    void ComponentLoaded();
}; // Size: 0x28

class UEMSAsyncCheck : public UBlueprintAsyncActionBase
{
    FEMSAsyncCheckOnCompleted OnCompleted;                                            // 0x0030 (size: 0x10)
    void CheckCompletedPin();
    FEMSAsyncCheckOnFailed OnFailed;                                                  // 0x0040 (size: 0x10)
    void CheckFailedPin();
    FEMSAsyncCheckOnVersionMismatch OnVersionMismatch;                                // 0x0050 (size: 0x10)
    void CheckVersionMismatchPin();
    class UEMSObject* EMS;                                                            // 0x0060 (size: 0x8)

    class UEMSAsyncCheck* CheckSaveFiles(class UObject* WorldContextObject, ESaveFileCheckType CheckType, FString CustomSaveName, bool bComplexCheck);
}; // Size: 0x88

class UEMSAsyncLoadGame : public UBlueprintAsyncActionBase
{
    FEMSAsyncLoadGameOnCompleted OnCompleted;                                         // 0x0030 (size: 0x10)
    void AsyncLoadOutputPin();
    FEMSAsyncLoadGameOnFailed OnFailed;                                               // 0x0040 (size: 0x10)
    void AsyncLoadFailedPin();
    class UEMSObject* EMS;                                                            // 0x0068 (size: 0x8)

    void AutoLoadLevelActors(class UEMSObject* EMSObject);
    class UEMSAsyncLoadGame* AsyncLoadActors(class UObject* WorldContextObject, int32 Data, bool bFullReload);
}; // Size: 0x80

class UEMSAsyncSaveGame : public UBlueprintAsyncActionBase
{
    FEMSAsyncSaveGameOnCompleted OnCompleted;                                         // 0x0030 (size: 0x10)
    void AsyncSaveOutputPin();
    FEMSAsyncSaveGameOnFailed OnFailed;                                               // 0x0040 (size: 0x10)
    void AsyncSaveFailedPin();
    class UEMSObject* EMS;                                                            // 0x0058 (size: 0x8)

    void AutoSaveLevelActors(class UEMSObject* EMSObject);
    class UEMSAsyncSaveGame* AsyncSaveActors(class UObject* WorldContextObject, int32 Data);
}; // Size: 0x70

class UEMSAsyncStream : public UBlueprintAsyncActionBase
{
    class ULevel* StreamingLevel;                                                     // 0x0030 (size: 0x8)
    class UEMSObject* EMS;                                                            // 0x0050 (size: 0x8)
    TMap<FName, TWeakObjectPtr<class AActor>> StreamActorsMap;                        // 0x0058 (size: 0x50)
    FMultiLevelStreamingData PrunedData;                                              // 0x00A8 (size: 0x70)

    bool InitStreamingLoadTask(class UEMSObject* EMSObject, class ULevel* InLevel);
}; // Size: 0x120

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

    void SetCurrentSaveUserName(class UObject* WorldContextObject, FString UserName);
    void SetCurrentSaveGameName(class UObject* WorldContextObject, FString SaveGameName);
    void SetActorSaveProperties(class UObject* WorldContextObject, bool bSkipSave, bool bPersistent, bool bSkipTransform, ELoadedStateMod LoadedState);
    bool SaveRawObject(class UObject* WorldContextObject, FRawObjectSaveData Data);
    bool SavePlayerActorsCustom(class AController* Controller, FString Filename);
    bool SaveCustom(class UObject* WorldContextObject, class UEMSCustomSaveGame* SaveGame);
    void ResetCustomSave(class UObject* WorldContextObject, class UEMSCustomSaveGame* SaveGame, EResetCustomSaveType Type);
    class UObject* LoadRawObject(class UObject* WorldContextObject, FRawObjectSaveData Data);
    bool LoadPlayerActorsCustom(class AController* Controller, FString Filename);
    bool IsWorldPartition(class UObject* WorldContextObject);
    bool IsSavingOrLoading(class UObject* WorldContextObject);
    bool IsLoadedState(class AActor* Actor);
    bool IsLevelStreamingActive(class UObject* WorldContextObject);
    class UTexture2D* ImportSaveThumbnail(class UObject* WorldContextObject, FString SaveGameName);
    TArray<FString> GetSortedSaveSlots(class UObject* WorldContextObject);
    class UEMSInfoSaveGame* GetSlotInfoSaveGame(class UObject* WorldContextObject, FString& SaveGameName);
    class UEMSInfoSaveGame* GetNamedSlotInfo(class UObject* WorldContextObject, FString SaveGameName);
    class UEMSCustomSaveGame* GetCustomSave(class UObject* WorldContextObject, TSubclassOf<class UEMSCustomSaveGame> SaveGameClass, FString SaveSlot, FString Filename);
    FString GetCurrentSaveUser(class UObject* WorldContextObject);
    FString GetBackupName(class UObject* WorldContextObject, FString BaseName);
    TArray<FString> GetAllSaveUsers(class UObject* WorldContextObject);
    void ExportSaveThumbnail(class UObject* WorldContextObject, class UTextureRenderTarget2D* TextureRenderTarget, FString SaveGameName);
    bool DoesSaveSlotExist(class UObject* WorldContextObject, FString SaveGameName, bool bComplete);
    bool DoesCustomSaveFileExist(class UObject* WorldContextObject, FString SaveSlot, FString Filename);
    void DeleteSaveUser(class UObject* WorldContextObject, FString UserName);
    void DeleteCustomSave(class UObject* WorldContextObject, class UEMSCustomSaveGame* SaveGame);
    bool DeleteCustomPlayerFile(class UObject* WorldContextObject, FString Filename);
    void DeleteAllSaveDataForSlot(class UObject* WorldContextObject, FString SaveGameName, EDeleteSlotType Data);
    void ClearWorldPartition(class UObject* WorldContextObject);
    void ClearMultiLevelSave(class UObject* WorldContextObject);
}; // Size: 0x28

class UEMSInfoSaveGame : public USaveGame
{
    FSaveSlotInfo SlotInfo;                                                           // 0x0028 (size: 0x40)

}; // Size: 0x68

class UEMSObject : public UEMSObjectBase
{
    FEMSObjectOnPlayerLoaded OnPlayerLoaded;                                          // 0x0148 (size: 0x10)
    void EmsLoadPlayerComplete(const class APlayerController* LoadedPlayer);
    FEMSObjectOnLevelLoaded OnLevelLoaded;                                            // 0x0158 (size: 0x10)
    void EmsLoadLevelComplete(const TArray<TSoftObjectPtr<class AActor>>& LoadedActors);
    FEMSObjectOnPartitionLoaded OnPartitionLoaded;                                    // 0x0168 (size: 0x10)
    void EmsLoadLevelComplete(const TArray<TSoftObjectPtr<class AActor>>& LoadedActors);
    TSet<TWeakObjectPtr<class AActor>> ActorList;                                     // 0x01E8 (size: 0x50)
    TMap<FName, TWeakObjectPtr<class AActor>> ActorMap;                               // 0x0238 (size: 0x50)
    TArray<FLevelArchive> LevelArchiveList;                                           // 0x0288 (size: 0x10)
    FMultiLevelStreamingData MultiLevelStreamData;                                    // 0x0298 (size: 0x70)
    TArray<FActorSaveData> SavedActors;                                               // 0x0308 (size: 0x10)
    TArray<FActorSaveData> SavedActorsPruned;                                         // 0x0318 (size: 0x10)
    TArray<FLevelScriptSaveData> SavedScripts;                                        // 0x0328 (size: 0x10)
    FGameObjectSaveData SavedGameMode;                                                // 0x0338 (size: 0x20)
    FGameObjectSaveData SavedGameState;                                               // 0x0358 (size: 0x20)
    TSet<FActorSaveData> WorldPartitionActors;                                        // 0x0378 (size: 0x50)
    TSet<FActorSaveData> DestroyedActors;                                             // 0x03C8 (size: 0x50)
    TArray<TSoftObjectPtr<class AActor>> RealLoadedActors;                            // 0x0418 (size: 0x10)
    FPlayerStackArchive PlayerStackData;                                              // 0x0478 (size: 0x100)
    FPlayerArchive SavedPlayer;                                                       // 0x0578 (size: 0xB0)

}; // Size: 0x628

class UEMSObjectAdv : public UEMSObjectBase
{

    bool SaveObjectCollection(const TArray<FRawObjectSaveData>& Objects, bool bUseSlot, FString Filename);
    bool LoadObjectCollection(const TArray<FRawObjectSaveData>& Objects, bool bUseSlot, FString Filename);
}; // Size: 0x148

class UEMSObjectBase : public UGameInstanceSubsystem
{
    FString CurrentSaveGameName;                                                      // 0x0070 (size: 0x10)
    FString CurrentSaveUserName;                                                      // 0x0080 (size: 0x10)
    double LastSlotSaveTime;                                                          // 0x0090 (size: 0x8)
    TMap<FString, UEMSInfoSaveGame*> CachedSaveSlots;                                 // 0x0098 (size: 0x50)
    TMap<FString, UEMSCustomSaveGame*> CachedCustomSaves;                             // 0x00E8 (size: 0x50)
    TArray<FName> MultiSaveLevels;                                                    // 0x0138 (size: 0x10)

}; // Size: 0x148

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
    TMap<FString, FSoftClassPath> RuntimeClasses;                                     // 0x0088 (size: 0x50)
    TMap<FString, FString> LevelRedirects;                                            // 0x00D8 (size: 0x50)
    float WorldPartitionInitPollingRate;                                              // 0x0128 (size: 0x4)

}; // Size: 0x220

#endif

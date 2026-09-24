#ifndef UE4SS_SDK_ActorIO_HPP
#define UE4SS_SDK_ActorIO_HPP

struct FActionExecutionContext
{
    class UActorIOAction* ActionPtr;                                                  // 0x0000 (size: 0x8)

}; // Size: 0x68

struct FActorIOEvent
{
}; // Size: 0x60

struct FActorIOEventList
{
    TArray<FActorIOEvent> EventRegistry;                                              // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FActorIOFunction
{
}; // Size: 0x40

struct FActorIOFunctionList
{
    TArray<FActorIOFunction> FunctionRegistry;                                        // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FActorIOMessage
{
}; // Size: 0x50

struct FLogicSpawnerEntry
{
    TSubclassOf<class AActor> SpawnClass;                                             // 0x0000 (size: 0x8)
    class AActor* SpawnPoint;                                                         // 0x0008 (size: 0x8)
    float SpawnDelay;                                                                 // 0x0010 (size: 0x4)

}; // Size: 0x18

class ALogicActorBase : public AActor
{
    class USceneComponent* SceneComponent;                                            // 0x02B0 (size: 0x8)

}; // Size: 0x2B8

class ALogicBranch : public ALogicActorBase
{
    bool bInitialValue;                                                               // 0x02B8 (size: 0x1)
    FLogicBranchOnTrue OnTrue;                                                        // 0x02C0 (size: 0x10)
    void SimpleActionDelegate();
    FLogicBranchOnFalse OnFalse;                                                      // 0x02D0 (size: 0x10)
    void SimpleActionDelegate();
    bool bCurrentValue;                                                               // 0x02E0 (size: 0x1)

    void ToggleAndTest();
    void Toggle();
    void Test();
    void SetValueAndTest(bool bValue);
    void SetValue(bool bValue);
    bool GetValue();
}; // Size: 0x2E8

class ALogicCase : public ALogicActorBase
{
    TArray<FString> Cases;                                                            // 0x02B8 (size: 0x10)
    FLogicCaseOnCase01 OnCase01;                                                      // 0x02C8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase02 OnCase02;                                                      // 0x02D8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase03 OnCase03;                                                      // 0x02E8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase04 OnCase04;                                                      // 0x02F8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase05 OnCase05;                                                      // 0x0308 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase06 OnCase06;                                                      // 0x0318 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase07 OnCase07;                                                      // 0x0328 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase08 OnCase08;                                                      // 0x0338 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase09 OnCase09;                                                      // 0x0348 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase10 OnCase10;                                                      // 0x0358 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase11 OnCase11;                                                      // 0x0368 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase12 OnCase12;                                                      // 0x0378 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase13 OnCase13;                                                      // 0x0388 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase14 OnCase14;                                                      // 0x0398 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase15 OnCase15;                                                      // 0x03A8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnCase16 OnCase16;                                                      // 0x03B8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCaseOnDefault OnDefault;                                                    // 0x03C8 (size: 0x10)
    void SimpleActionDelegate();

    void Test(FString InValue);
}; // Size: 0x3D8

class ALogicCompare : public ALogicActorBase
{
    FString InitialValue;                                                             // 0x02B8 (size: 0x10)
    FString CompareValue;                                                             // 0x02C8 (size: 0x10)
    float ErrorToleranceForNumericValues;                                             // 0x02D8 (size: 0x4)
    FLogicCompareOnEquals OnEquals;                                                   // 0x02E0 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCompareOnNotEquals OnNotEquals;                                             // 0x02F0 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCompareOnLessThen OnLessThen;                                               // 0x0300 (size: 0x10)
    void SimpleActionDelegate();
    FLogicCompareOnGreaterThen OnGreaterThen;                                         // 0x0310 (size: 0x10)
    void SimpleActionDelegate();
    FString CurrentValue;                                                             // 0x0320 (size: 0x10)

    void SetValueAndCompare(FString InValue);
    void SetValue(FString InValue);
    void SetCompareValue(FString InValue);
    void Compare();
}; // Size: 0x330

class ALogicCounter : public ALogicActorBase
{
    int32 InitialValue;                                                               // 0x02B8 (size: 0x4)
    int32 TargetValue;                                                                // 0x02BC (size: 0x4)
    bool bClampValue;                                                                 // 0x02C0 (size: 0x1)
    FLogicCounterOnValueChanged OnValueChanged;                                       // 0x02C8 (size: 0x10)
    void OnCounterValue(int32 Value);
    FLogicCounterOnTargetValueChanged OnTargetValueChanged;                           // 0x02D8 (size: 0x10)
    void OnCounterValue(int32 Value);
    FLogicCounterOnTargetValueReached OnTargetValueReached;                           // 0x02E8 (size: 0x10)
    void OnCounterValue(int32 Value);
    FLogicCounterOnGetValue OnGetValue;                                               // 0x02F8 (size: 0x10)
    void OnCounterValue(int32 Value);
    int32 CurrentValue;                                                               // 0x0308 (size: 0x4)

    void Subtract(int32 Amount);
    void SetValue(int32 Value);
    void SetTargetValue(int32 Value);
    int32 GetValue();
    void Add(int32 Amount);
}; // Size: 0x310

class ALogicDataLayer : public ALogicActorBase
{
    class UDataLayerAsset* DataLayerAsset;                                            // 0x02B8 (size: 0x8)
    bool bLoadRecursive;                                                              // 0x02C0 (size: 0x1)
    FLogicDataLayerOnDataLayerLoaded OnDataLayerLoaded;                               // 0x02C8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicDataLayerOnDataLayerUnloaded OnDataLayerUnloaded;                           // 0x02D8 (size: 0x10)
    void SimpleActionDelegate();

    void UnloadDataLayer();
    void OnDataLayerLoadStateChanged(const class UDataLayerInstance* InDataLayer, EDataLayerRuntimeState InState);
    void LoadDataLayer();
    bool IsDataLayerLoaded();
}; // Size: 0x2F0

class ALogicGlobalEvent : public ALogicActorBase
{
    FLogicGlobalEventOnWorldInitialized OnWorldInitialized;                           // 0x02B8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicGlobalEventOnActorBeginPlay OnActorBeginPlay;                               // 0x02C8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicGlobalEventOnActorEndPlay OnActorEndPlay;                                   // 0x02D8 (size: 0x10)
    void SimpleActionDelegate();
    FLogicGlobalEventOnWorldTeardown OnWorldTeardown;                                 // 0x02E8 (size: 0x10)
    void SimpleActionDelegate();

    void CallLevelBlueprintFunction(FString Command);
}; // Size: 0x2F8

class ALogicRelay : public ALogicActorBase
{
    bool bIsEnabled;                                                                  // 0x02B8 (size: 0x1)
    FLogicRelayOnTrigger OnTrigger;                                                   // 0x02C0 (size: 0x10)
    void SimpleActionDelegate();

    void Trigger();
    bool IsEnabled();
    void Enable();
    void Disable();
}; // Size: 0x2D0

class ALogicSpawner : public ALogicActorBase
{
    TArray<FLogicSpawnerEntry> ActorsToSpawn;                                         // 0x02B8 (size: 0x10)
    bool bSpawnActorsOnStart;                                                         // 0x02C8 (size: 0x1)
    FLogicSpawnerOnActorSpawned OnActorSpawned;                                       // 0x02D0 (size: 0x10)
    void OnSpawnerSpawnActor(class AActor* ActorPtr);
    FLogicSpawnerOnSpawnFinished OnSpawnFinished;                                     // 0x02E0 (size: 0x10)
    void SimpleActionDelegate();
    FLogicSpawnerOnGetSpawnedActor OnGetSpawnedActor;                                 // 0x02F0 (size: 0x10)
    void OnGetSpawnedActor(class AActor* ActorPtr);
    TArray<AActor*> SpawnedActors;                                                    // 0x0300 (size: 0x10)

    void SpawnActors();
    void ProcessEvent_OnGetSpawnedActor(class AActor* ActorPtr);
    void ProcessEvent_OnActorSpawned(class AActor* ActorPtr);
    class AActor* GetSpawnedActorForEntry(int32 EntryIdx);
    int32 GetSpawnedActorCount();
    void DestroySpawnedActors();
    void DestroySpawnedActorForEntry(int32 EntryIdx);
}; // Size: 0x310

class ALogicTimeline : public ALogicActorBase
{
    FRuntimeFloatCurve Curve;                                                         // 0x02B8 (size: 0x88)
    float PlayRate;                                                                   // 0x0340 (size: 0x4)
    bool bLoop;                                                                       // 0x0344 (size: 0x1)
    bool bIgnoreTimeDilation;                                                         // 0x0345 (size: 0x1)
    FLogicTimelineOnTimelineValueChanged OnTimelineValueChanged;                      // 0x0348 (size: 0x10)
    void OnTimelineValueChanged(float Value);
    FLogicTimelineOnTimelineFinished OnTimelineFinished;                              // 0x0358 (size: 0x10)
    void SimpleActionDelegate();
    FTimeline Timeline;                                                               // 0x0368 (size: 0x98)
    class UCurveFloat* TimelineCurve;                                                 // 0x0400 (size: 0x8)

    void Stop();
    void ReverseFromEnd();
    void Reverse();
    void ProcessEvent_OnTimelineValueChanged(float InValue);
    void PlayFromStart();
    void Play();
}; // Size: 0x408

class ALogicTimer : public ALogicActorBase
{
    float Time;                                                                       // 0x02B8 (size: 0x4)
    float TimeRandomization;                                                          // 0x02BC (size: 0x4)
    bool bLoop;                                                                       // 0x02C0 (size: 0x1)
    bool bAutoStart;                                                                  // 0x02C1 (size: 0x1)
    FLogicTimerOnTimer OnTimer;                                                       // 0x02C8 (size: 0x10)
    void SimpleActionDelegate();

    void StopTimer();
    void StartTimerWithParams(float InTime, float InTimeRandomization, bool bInLoop);
    void StartTimer();
    bool IsTimerActive();
}; // Size: 0x2E0

class IActorIOInterface : public IInterface
{

    void K2_RegisterIOFunctions(FActorIOFunctionList& FunctionRegistry);
    void K2_RegisterIOEvents(FActorIOEventList& EventRegistry);
    void K2_GetLocalNamedArguments();
    bool ConditionalAbortIOAction(class UActorIOAction* Action);
}; // Size: 0x28

class UActorIOAction : public UObject
{
    FName EventId;                                                                    // 0x0028 (size: 0x8)
    TSoftObjectPtr<class AActor> TargetActor;                                         // 0x0030 (size: 0x28)
    FName FunctionId;                                                                 // 0x0058 (size: 0x8)
    FString FunctionArguments;                                                        // 0x0060 (size: 0x10)
    float Delay;                                                                      // 0x0070 (size: 0x4)
    bool bExecuteOnlyOnce;                                                            // 0x0074 (size: 0x1)
    bool bWasExecuted;                                                                // 0x0075 (size: 0x1)

    bool IsTargetActorAlive();
    bool GetWasExecuted();
    class UActorIOComponent* GetOwnerIOComponent();
    class AActor* GetOwnerActor();
    void ExecuteAction();
}; // Size: 0x88

class UActorIOComponent : public UActorComponent
{
    TArray<UActorIOAction*> Actions;                                                  // 0x00B8 (size: 0x10)

}; // Size: 0xC8

class UActorIOLibrary : public UBlueprintFunctionLibrary
{

    void K2_SetNamedArgument(class UObject* WorldContextObject, FString ArgumentName, FString ArgumentValue);
    void K2_RegisterIOFunction(class UObject* WorldContextObject, FActorIOFunctionList& Registry, FName FunctionId, const FText& DisplayNameText, const FText& ToolTipText, FString FunctionToExec, FName SubobjectName);
    void K2_RegisterIOEvent(class UObject* WorldContextObject, FActorIOEventList& Registry, FName EventId, const FText& DisplayNameText, const FText& ToolTipText, FName EventDispatcherName, FName EventProcessorName, FName SubobjectName);
    void K2_AbortAction(class UObject* WorldContextObject);
}; // Size: 0x28

class UActorIOSettings : public UDeveloperSettings
{
    TSubclassOf<class UActorIOSubsystemBase> ActorIOSubsystemClass;                   // 0x0038 (size: 0x8)

}; // Size: 0x40

class UActorIOSubsystemBase : public UTickableWorldSubsystem
{
    FActionExecutionContext ActionExecContext;                                        // 0x0040 (size: 0x68)
    TArray<FActorIOMessage> PendingMessages;                                          // 0x00A8 (size: 0x10)

    void ProcessEvent_OnActorOverlap(class AActor* OverlappedActor, class AActor* OtherActor);
    void ProcessEvent_OnActorDestroyed(class AActor* Actor, TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void K2_RegisterNativeFunctionsForObject(class AActor* InObject, FActorIOFunctionList& FunctionRegistry);
    void K2_RegisterNativeEventsForObject(class AActor* InObject, FActorIOEventList& EventRegistry);
    void K2_GetGlobalNamedArguments();
}; // Size: 0xB8

#endif

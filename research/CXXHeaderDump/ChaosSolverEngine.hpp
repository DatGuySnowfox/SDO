#ifndef UE4SS_SDK_ChaosSolverEngine_HPP
#define UE4SS_SDK_ChaosSolverEngine_HPP

#include "ChaosSolverEngine_enums.hpp"

struct FBreakEventCallbackWrapper
{
}; // Size: 0x30

struct FChaosDebugSubstepControl
{
    bool bPause;                                                                      // 0x0000 (size: 0x1)
    bool bSubstep;                                                                    // 0x0001 (size: 0x1)
    bool bStep;                                                                       // 0x0002 (size: 0x1)

}; // Size: 0x3

struct FChaosHandlerSet
{
    TSet<UObject*> ChaosHandlers;                                                     // 0x0008 (size: 0x50)

}; // Size: 0x58

struct FChaosPhysicsCollisionInfo
{
    class UPrimitiveComponent* Component;                                             // 0x0000 (size: 0x8)
    class UPrimitiveComponent* OtherComponent;                                        // 0x0008 (size: 0x8)
    FVector Location;                                                                 // 0x0010 (size: 0x18)
    FVector Normal;                                                                   // 0x0028 (size: 0x18)
    FVector AccumulatedImpulse;                                                       // 0x0040 (size: 0x18)
    FVector Velocity;                                                                 // 0x0058 (size: 0x18)
    FVector OtherVelocity;                                                            // 0x0070 (size: 0x18)
    FVector AngularVelocity;                                                          // 0x0088 (size: 0x18)
    FVector OtherAngularVelocity;                                                     // 0x00A0 (size: 0x18)
    float Mass;                                                                       // 0x00B8 (size: 0x4)
    float OtherMass;                                                                  // 0x00BC (size: 0x4)

}; // Size: 0xC0

struct FChaosVDChannelStateChangeCommandMessage
{
    FChaosVDDataChannelState NewState;                                                // 0x0000 (size: 0x18)

}; // Size: 0x18

struct FChaosVDChannelStateChangeResponseMessage
{
    FGuid InstanceId;                                                                 // 0x0000 (size: 0x10)
    FChaosVDDataChannelState NewState;                                                // 0x0010 (size: 0x18)

}; // Size: 0x28

struct FChaosVDDataChannelState
{
    FString ChannelName;                                                              // 0x0000 (size: 0x10)
    bool bIsEnabled;                                                                  // 0x0010 (size: 0x1)
    bool bCanChangeChannelState;                                                      // 0x0011 (size: 0x1)

}; // Size: 0x18

struct FChaosVDFullSessionInfoRequestMessage
{
}; // Size: 0x1

struct FChaosVDFullSessionInfoResponseMessage
{
    FGuid InstanceId;                                                                 // 0x0000 (size: 0x10)
    TArray<FChaosVDDataChannelState> DataChannelsStates;                              // 0x0010 (size: 0x10)
    bool bIsRecording;                                                                // 0x0020 (size: 0x1)

}; // Size: 0x28

struct FChaosVDRecordingStatusMessage
{
    FGuid InstanceId;                                                                 // 0x0000 (size: 0x10)
    bool bIsRecording;                                                                // 0x0010 (size: 0x1)
    float ElapsedTime;                                                                // 0x0014 (size: 0x4)
    FChaosVDTraceDetails TraceDetails;                                                // 0x0018 (size: 0x38)

}; // Size: 0x50

struct FChaosVDSessionPing
{
    FGuid ControllerInstanceId;                                                       // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FChaosVDSessionPong
{
    FGuid InstanceId;                                                                 // 0x0000 (size: 0x10)
    FGuid SessionID;                                                                  // 0x0010 (size: 0x10)
    FString SessionName;                                                              // 0x0020 (size: 0x10)
    uint8 BuildTargetType;                                                            // 0x0030 (size: 0x1)

}; // Size: 0x38

struct FChaosVDStartRecordingCommandMessage
{
    EChaosVDRecordingMode RecordingMode;                                              // 0x0000 (size: 0x1)
    FString Target;                                                                   // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FChaosVDStopRecordingCommandMessage
{
}; // Size: 0x1

struct FCrumblingEventCallbackWrapper
{
}; // Size: 0x30

struct FDataflowRigidSolverProxy : public FDataflowPhysicsSolverProxy
{
}; // Size: 0x90

struct FRemovalEventCallbackWrapper
{
}; // Size: 0x30

class AChaosSolverActor : public AActor
{
    FChaosSolverConfiguration Properties;                                             // 0x02B0 (size: 0x84)
    float TimeStepMultiplier;                                                         // 0x0334 (size: 0x4)
    int32 CollisionIterations;                                                        // 0x0338 (size: 0x4)
    int32 PushOutIterations;                                                          // 0x033C (size: 0x4)
    int32 PushOutPairIterations;                                                      // 0x0340 (size: 0x4)
    float ClusterConnectionFactor;                                                    // 0x0344 (size: 0x4)
    EClusterConnectionTypeEnum ClusterUnionConnectionType;                            // 0x0348 (size: 0x1)
    bool DoGenerateCollisionData;                                                     // 0x0349 (size: 0x1)
    FSolverCollisionFilterSettings CollisionFilterSettings;                           // 0x034C (size: 0x10)
    bool DoGenerateBreakingData;                                                      // 0x035C (size: 0x1)
    FSolverBreakingFilterSettings BreakingFilterSettings;                             // 0x0360 (size: 0x10)
    bool DoGenerateTrailingData;                                                      // 0x0370 (size: 0x1)
    FSolverTrailingFilterSettings TrailingFilterSettings;                             // 0x0374 (size: 0x10)
    float MassScale;                                                                  // 0x0384 (size: 0x4)
    bool bHasFloor;                                                                   // 0x0388 (size: 0x1)
    float FloorHeight;                                                                // 0x038C (size: 0x4)
    FChaosDebugSubstepControl ChaosDebugSubstepControl;                               // 0x0390 (size: 0x3)
    class UBillboardComponent* SpriteComponent;                                       // 0x0398 (size: 0x8)
    FDataflowSimulationAsset SimulationAsset;                                         // 0x03A0 (size: 0x58)
    class UChaosGameplayEventDispatcher* GameplayEventDispatcherComponent;            // 0x0498 (size: 0x8)

    void SetSolverActive(bool bActive);
    void SetAsCurrentWorldSolver();
}; // Size: 0x4A8

class IChaosNotifyHandlerInterface : public IInterface
{
}; // Size: 0x28

class UChaosDebugDrawComponent : public UActorComponent
{
}; // Size: 0xC0

class UChaosDebugDrawSubsystem : public UWorldSubsystem
{
}; // Size: 0x30

class UChaosEventListenerComponent : public UActorComponent
{
}; // Size: 0xC0

class UChaosGameplayEventDispatcher : public UChaosEventListenerComponent
{
    TMap<UPrimitiveComponent*, FChaosHandlerSet> CollisionEventRegistrations;         // 0x0180 (size: 0x50)
    TMap<UPrimitiveComponent*, FBreakEventCallbackWrapper> BreakEventRegistrations;   // 0x01D0 (size: 0x50)
    TMap<UPrimitiveComponent*, FRemovalEventCallbackWrapper> RemovalEventRegistrations; // 0x0220 (size: 0x50)
    TMap<UPrimitiveComponent*, FCrumblingEventCallbackWrapper> CrumblingEventRegistrations; // 0x0270 (size: 0x50)

}; // Size: 0x2D0

class UChaosSolver : public UObject
{
}; // Size: 0x28

class UChaosSolverEngineBlueprintLibrary : public UBlueprintFunctionLibrary
{

    FHitResult ConvertPhysicsCollisionToHitResult(const FChaosPhysicsCollisionInfo& PhysicsCollision);
}; // Size: 0x28

class UChaosSolverSettings : public UDeveloperSettings
{
    FSoftClassPath DefaultChaosSolverActorClass;                                      // 0x0040 (size: 0x20)

}; // Size: 0x60

#endif

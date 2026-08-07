#ifndef UE4SS_SDK_CombatComponents_HPP
#define UE4SS_SDK_CombatComponents_HPP

#include "CombatComponents_enums.hpp"

struct FCCCollidingComponent
{
    class UPrimitiveComponent* Component;                                             // 0x0000 (size: 0x8)
    TArray<FName> Sockets;                                                            // 0x0008 (size: 0x10)
    TArray<class AActor*> HitActors;                                                  // 0x0018 (size: 0x10)

}; // Size: 0x28

class ICCRotatingOwnerInterface : public IInterface
{

    FRotator GetDesiredRotation();
}; // Size: 0x28

class UCCActivateCollisionNotifyWindow : public UAnimNotify_PlayMontageNotifyWindow
{
    ECCCollisionPart CollisionPart;                                                   // 0x0038 (size: 0x1)

}; // Size: 0x40

class UCCClearHitActorsNotify : public UAnimNotify_PlayMontageNotify
{
}; // Size: 0x40

class UCCCollisionHandlerComponent : public UActorComponent
{
    uint8 bTraceComplex;                                                              // 0x00A0 (size: 0x1)
    float TraceRadius;                                                                // 0x00A4 (size: 0x4)
    float TraceCheckInterval;                                                         // 0x00A8 (size: 0x4)
    TArray<class TSubclassOf<AActor>> IgnoredClasses;                                 // 0x00B0 (size: 0x10)
    TArray<FName> IgnoredCollisionProfileNames;                                       // 0x00C0 (size: 0x10)
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToCollideWith;                   // 0x00D0 (size: 0x10)
    TArray<class AActor*> IgnoredActors;                                              // 0x00E0 (size: 0x10)
    ECCCollisionPart ActivatedCollisionPart;                                          // 0x00F0 (size: 0x1)
    TArray<FCCCollidingComponent> ActiveCollidingComponents;                          // 0x00F8 (size: 0x10)
    uint8 bIsCollisionActivated;                                                      // 0x0108 (size: 0x1)
    FTimerHandle TimerHandle_TraceCheck;                                              // 0x0110 (size: 0x8)
    TMap<class FName, class FVector> LastFrameSocketLocations;                        // 0x0118 (size: 0x50)
    FCCCollisionHandlerComponentOnHit OnHit;                                          // 0x0170 (size: 0x10)
    void OnHit(const FHitResult& HitResult, class UPrimitiveComponent* CollidingComponent);
    FCCCollisionHandlerComponentOnCollisionActivated OnCollisionActivated;            // 0x0198 (size: 0x10)
    void OnCollisionActivated(ECCCollisionPart CollisionPart);
    FCCCollisionHandlerComponentOnCollisionDeactivated OnCollisionDeactivated;        // 0x01C0 (size: 0x10)
    void OnCollisionDeactivated();
    uint8 bDebug;                                                                     // 0x01E8 (size: 0x1)

    void UpdateCollidingComponents(const TArray<FCCCollidingComponent>& collidingComponents);
    void UpdateCollidingComponent(class UPrimitiveComponent* Component, const TArray<FName>& Sockets);
    void TraceCheckLoop();
    void SetActiveCollisionPart(ECCCollisionPart CollisionPart);
    void OnRep_IsCollisionActivated();
    bool IsCollisionActivated();
    ECCCollisionPart GetActivatedCollisionPart();
    void DrawHitSphere(FVector Location);
    void DrawDebugTrace(FVector Start, FVector End);
    void DeactivateCollision();
    void ClearHitActors();
    void ActivateCollision(ECCCollisionPart CollisionPart);
}; // Size: 0x1F0

class UCCRotateOwnerNotify : public UAnimNotify_PlayMontageNotify
{
    float DegreesPerSecond;                                                           // 0x0040 (size: 0x4)
    float MaxPossibleRotation;                                                        // 0x0044 (size: 0x4)

}; // Size: 0x48

class UCCRotateOwnerNotifyWindow : public UAnimNotify_PlayMontageNotifyWindow
{
    float DegreesPerSecond;                                                           // 0x0038 (size: 0x4)

}; // Size: 0x40

class UCCRotatingOwnerComponent : public UActorComponent
{
    uint8 bIsRotating;                                                                // 0x00A0 (size: 0x1)
    FCCRotatingOwnerComponentOnRotatingStart OnRotatingStart;                         // 0x00B0 (size: 0x10)
    void OnRotatingStart();
    FCCRotatingOwnerComponentOnRotatingEnd OnRotatingEnd;                             // 0x00D8 (size: 0x10)
    void OnRotatingEnd();

    void StopRotating();
    void StartRotatingWithLimit(float MaxPossibleRotation, float degressPerSecond);
    void StartRotating(float Time, float degressPerSecond);
    void SetOwnerRotation(const FRotator& NewRotation);
    void NotifyOnRotatingStart();
    void NotifyOnRotatingEnd();
    bool IsRotating();
}; // Size: 0x100

#endif

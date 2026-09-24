#ifndef UE4SS_SDK_CableComponent_HPP
#define UE4SS_SDK_CableComponent_HPP

class ACableActor : public AActor
{
    class UCableComponent* CableComponent;                                            // 0x02A8 (size: 0x8)

}; // Size: 0x2B0

class UCableComponent : public UMeshComponent
{
    bool bAttachStart;                                                                // 0x0568 (size: 0x1)
    bool bAttachEnd;                                                                  // 0x0569 (size: 0x1)
    FComponentReference AttachEndTo;                                                  // 0x0570 (size: 0x28)
    FName AttachEndToSocketName;                                                      // 0x0598 (size: 0x8)
    FVector EndLocation;                                                              // 0x05A0 (size: 0x18)
    float CableLength;                                                                // 0x05B8 (size: 0x4)
    int32 NumSegments;                                                                // 0x05BC (size: 0x4)
    float SubstepTime;                                                                // 0x05C0 (size: 0x4)
    int32 SolverIterations;                                                           // 0x05C4 (size: 0x4)
    bool bEnableStiffness;                                                            // 0x05C8 (size: 0x1)
    bool bUseSubstepping;                                                             // 0x05C9 (size: 0x1)
    bool bSkipCableUpdateWhenNotVisible;                                              // 0x05CA (size: 0x1)
    bool bSkipCableUpdateWhenNotOwnerRecentlyRendered;                                // 0x05CB (size: 0x1)
    bool bEnableCollision;                                                            // 0x05CC (size: 0x1)
    float CollisionFriction;                                                          // 0x05D0 (size: 0x4)
    FVector CableForce;                                                               // 0x05D8 (size: 0x18)
    float CableGravityScale;                                                          // 0x05F0 (size: 0x4)
    float CableWidth;                                                                 // 0x05F4 (size: 0x4)
    int32 NumSides;                                                                   // 0x05F8 (size: 0x4)
    float TileMaterial;                                                               // 0x05FC (size: 0x4)
    bool bResetAfterTeleport;                                                         // 0x0600 (size: 0x1)
    float TeleportDistanceThreshold;                                                  // 0x0604 (size: 0x4)
    float TeleportRotationThreshold;                                                  // 0x0608 (size: 0x4)
    bool bTeleportAfterReattach;                                                      // 0x060C (size: 0x1)

    void SetAttachEndToComponent(class USceneComponent* Component, FName SocketName);
    void SetAttachEndTo(class AActor* Actor, FName ComponentProperty, FName SocketName);
    void GetCableParticleLocations(TArray<FVector>& Locations);
    class USceneComponent* GetAttachedComponent();
    class AActor* GetAttachedActor();
}; // Size: 0x740

#endif

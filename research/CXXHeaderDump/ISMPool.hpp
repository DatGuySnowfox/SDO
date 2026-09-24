#ifndef UE4SS_SDK_ISMPool_HPP
#define UE4SS_SDK_ISMPool_HPP

class AISMPoolActor : public AActor
{
    class UISMPoolComponent* ISMPoolComp;                                             // 0x02A8 (size: 0x8)
    class UISMPoolDebugDrawComponent* ISMPoolDebugDrawComp;                           // 0x02B0 (size: 0x8)

}; // Size: 0x2B8

class UISMPoolComponent : public USceneComponent
{
}; // Size: 0x370

class UISMPoolDebugDrawComponent : public UDebugDrawComponent
{
    bool bShowGlobalStats;                                                            // 0x0580 (size: 0x1)
    bool bShowStats;                                                                  // 0x0581 (size: 0x1)
    bool bShowBounds;                                                                 // 0x0582 (size: 0x1)
    class UInstancedStaticMeshComponent* SelectedComponent;                           // 0x0588 (size: 0x8)

}; // Size: 0x5A0

class UISMPoolSubSystem : public UWorldSubsystem
{

    void OnActorEndPlay(class AActor* InSource, TEnumAsByte<EEndPlayReason::Type> Reason);
}; // Size: 0x80

#endif

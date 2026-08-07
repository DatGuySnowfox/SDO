#ifndef UE4SS_SDK_Buildable_WoodenFoundationTriangle_HPP
#define UE4SS_SDK_Buildable_WoodenFoundationTriangle_HPP

class ABuildable_WoodenFoundationTriangle_C : public ABuildable_ModularMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0438 (size: 0x8)
    class UChildActorComponent* StairsHalf9;                                          // 0x0440 (size: 0x8)
    class UChildActorComponent* StairsHalf8;                                          // 0x0448 (size: 0x8)
    class UChildActorComponent* StairsHalf7;                                          // 0x0450 (size: 0x8)
    class UChildActorComponent* StairsHalf6;                                          // 0x0458 (size: 0x8)
    class UChildActorComponent* StairsHalf5;                                          // 0x0460 (size: 0x8)
    class UChildActorComponent* StairsHalf4;                                          // 0x0468 (size: 0x8)
    class UChildActorComponent* StairsHalf3;                                          // 0x0470 (size: 0x8)
    class UChildActorComponent* StairsHalf2;                                          // 0x0478 (size: 0x8)
    class UChildActorComponent* FoundationTriangle3;                                  // 0x0480 (size: 0x8)
    class UChildActorComponent* FoundationTriangle2;                                  // 0x0488 (size: 0x8)
    class USceneComponent* FoundationTriangles;                                       // 0x0490 (size: 0x8)
    class UChildActorComponent* FoundationTriangle1;                                  // 0x0498 (size: 0x8)
    class UChildActorComponent* StairsHalf1;                                          // 0x04A0 (size: 0x8)
    class USceneComponent* StairsHalf;                                                // 0x04A8 (size: 0x8)
    class UChildActorComponent* WallHalf2;                                            // 0x04B0 (size: 0x8)
    class UChildActorComponent* WallHalf4;                                            // 0x04B8 (size: 0x8)
    class UChildActorComponent* WallHalf5;                                            // 0x04C0 (size: 0x8)
    class UChildActorComponent* WallHalf3;                                            // 0x04C8 (size: 0x8)
    class UChildActorComponent* WallHalf1;                                            // 0x04D0 (size: 0x8)
    class UChildActorComponent* WallHalf6;                                            // 0x04D8 (size: 0x8)
    class USceneComponent* WallHalves;                                                // 0x04E0 (size: 0x8)
    class UChildActorComponent* Foundation1;                                          // 0x04E8 (size: 0x8)
    class UChildActorComponent* Foundation2;                                          // 0x04F0 (size: 0x8)
    class UChildActorComponent* Foundation3;                                          // 0x04F8 (size: 0x8)
    class USceneComponent* Foundations;                                               // 0x0500 (size: 0x8)
    class UChildActorComponent* Wall2;                                                // 0x0508 (size: 0x8)
    class UChildActorComponent* Wall1;                                                // 0x0510 (size: 0x8)
    class UChildActorComponent* Wall3;                                                // 0x0518 (size: 0x8)
    class USceneComponent* Walls;                                                     // 0x0520 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_Buildable_WoodenFoundationTriangle(int32 EntryPoint);
}; // Size: 0x528

#endif

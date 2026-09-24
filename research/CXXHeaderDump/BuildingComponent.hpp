#ifndef UE4SS_SDK_BuildingComponent_HPP
#define UE4SS_SDK_BuildingComponent_HPP

class UBuildingComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00D0 (size: 0x8)
    bool BuildModeOn?;                                                                // 0x00D8 (size: 0x1)
    bool CanBuild;                                                                    // 0x00D9 (size: 0x1)
    FGuid BuildUID;                                                                   // 0x00DC (size: 0x10)
    FTransform BuildTransform;                                                        // 0x00F0 (size: 0x60)
    class UCameraComponent* Camera;                                                   // 0x0150 (size: 0x8)
    class UStaticMeshComponent* BuildGhost;                                           // 0x0158 (size: 0x8)
    FVector AddLocation;                                                              // 0x0160 (size: 0x18)
    FVector Normal;                                                                   // 0x0178 (size: 0x18)
    class AActor* Hit Actor;                                                          // 0x0190 (size: 0x8)
    class AActor* PickupRef;                                                          // 0x0198 (size: 0x8)
    FHitResult HitInfo;                                                               // 0x01A0 (size: 0x100)
    double RotationZ;                                                                 // 0x02A0 (size: 0x8)
    TArray<FTransform> SocketTransforms;                                              // 0x02A8 (size: 0x10)
    bool SnappingEnabled?;                                                            // 0x02B8 (size: 0x1)
    class UJigsawItem_DataAsset_C* DARef;                                             // 0x02C0 (size: 0x8)
    bool ReplacingObject?;                                                            // 0x02C8 (size: 0x1)
    class AActor* ReplacingActor;                                                     // 0x02D0 (size: 0x8)

    void GetNearestTransform(FVector Location, TArray<FTransform>& Transforms, FTransform& ResultTransform);
    void RotateSnap(FTransform SocketTransform, class AActor* Actor, FRotator& Rotator);
    bool IsPlaceableOnLandscape(const class UObject* Object);
    void BoundsCheck(float DivideScale, bool InvertBool, FVector BoundsOverlapOffset, bool& Hit);
    void ChangeRotationZ(bool Increase?, double Value, bool& Success);
    void CheckForSnap(TEnumAsByte<Enum_BuildingSnapTypes::Type> Type);
    void SpawnBuild(FTransform SpawnTransform);
    void BuildCycle();
    void GiveBuildColour(bool IsGreen?);
    void SpawnBuildGhost(class UStaticMesh* Mesh);
    void LoadComponent();
    void Event_LaunchBuildMode(class UJigsawItem_DataAsset_C* DA, bool Replacing?, class AActor* ReplacingActor);
    void Event_NoBuildZone();
    void Svr_SpawnBuild(FTransform SpawnTransform);
    void Event_StopBuildMode();
    void ExecuteUbergraph_BuildingComponent(int32 EntryPoint);
}; // Size: 0x2D8

#endif

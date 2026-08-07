#ifndef UE4SS_SDK_BuildingComponent_HPP
#define UE4SS_SDK_BuildingComponent_HPP

class UBuildingComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    bool BuildModeOn?;                                                                // 0x00C0 (size: 0x1)
    bool CanBuild;                                                                    // 0x00C1 (size: 0x1)
    FGuid BuildUID;                                                                   // 0x00C4 (size: 0x10)
    FTransform BuildTransform;                                                        // 0x00E0 (size: 0x60)
    class UCameraComponent* Camera;                                                   // 0x0140 (size: 0x8)
    class UStaticMeshComponent* BuildGhost;                                           // 0x0148 (size: 0x8)
    FVector AddLocation;                                                              // 0x0150 (size: 0x18)
    FVector Normal;                                                                   // 0x0168 (size: 0x18)
    class AActor* Hit Actor;                                                          // 0x0180 (size: 0x8)
    class AActor* PickupRef;                                                          // 0x0188 (size: 0x8)
    FHitResult HitInfo;                                                               // 0x0190 (size: 0xE8)
    double RotationZ;                                                                 // 0x0278 (size: 0x8)
    TArray<FTransform> SocketTransforms;                                              // 0x0280 (size: 0x10)
    bool SnappingEnabled?;                                                            // 0x0290 (size: 0x1)
    class UJigsawItem_DataAsset_C* DARef;                                             // 0x0298 (size: 0x8)
    bool ReplacingObject?;                                                            // 0x02A0 (size: 0x1)
    class AActor* ReplacingActor;                                                     // 0x02A8 (size: 0x8)

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
}; // Size: 0x2B0

#endif

#ifndef UE4SS_SDK_Save_Player_HPP
#define UE4SS_SDK_Save_Player_HPP

class USave_Player_C : public UEMSCustomSaveGame
{
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x0050 (size: 0x10)
    TArray<FGuid> MainUIDs;                                                           // 0x0060 (size: 0x10)
    TArray<FS_InvWeight> Weights;                                                     // 0x0070 (size: 0x10)
    TArray<FGuid> ActionbarUIDs;                                                      // 0x0080 (size: 0x10)
    class UMaterialInstance* SkinColor;                                               // 0x0090 (size: 0x8)
    class UStaticMesh* HairType;                                                      // 0x0098 (size: 0x8)
    class UMaterialInstance* HairColor;                                               // 0x00A0 (size: 0x8)
    class UStaticMesh* BeardType;                                                     // 0x00A8 (size: 0x8)
    class UMaterialInstance* BeardColor;                                              // 0x00B0 (size: 0x8)
    FString Forename;                                                                 // 0x00B8 (size: 0x10)
    FString Surname;                                                                  // 0x00C8 (size: 0x10)
    FString Sex;                                                                      // 0x00D8 (size: 0x10)
    FString Age;                                                                      // 0x00E8 (size: 0x10)
    class UStaticMesh* EyebrowsType;                                                  // 0x00F8 (size: 0x8)
    TEnumAsByte<Enum_Occupation::Type> Occupation;                                    // 0x0100 (size: 0x1)
    int32 ZombieKills;                                                                // 0x0104 (size: 0x4)
    int32 BossZombieKills;                                                            // 0x0108 (size: 0x4)
    int32 AnimalKills;                                                                // 0x010C (size: 0x4)
    int32 HumanKills;                                                                 // 0x0110 (size: 0x4)
    int32 DaysSurvived;                                                               // 0x0114 (size: 0x4)
    double DistanceTravelled;                                                         // 0x0118 (size: 0x8)
    int32 InfestationsDestroyed;                                                      // 0x0120 (size: 0x4)
    FTransform PlayerLocation;                                                        // 0x0130 (size: 0x60)
    FTransform RespawnLocation;                                                       // 0x0190 (size: 0x60)
    bool RespawnLocationSet?;                                                         // 0x01F0 (size: 0x1)
    class UStaticMesh* AccessoryType1;                                                // 0x01F8 (size: 0x8)
    class UStaticMesh* AccessoryType2;                                                // 0x0200 (size: 0x8)
    class UStaticMesh* AccessoryType3;                                                // 0x0208 (size: 0x8)
    class UStaticMesh* MouthType;                                                     // 0x0210 (size: 0x8)

}; // Size: 0x218

#endif

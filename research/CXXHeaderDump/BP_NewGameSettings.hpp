#ifndef UE4SS_SDK_BP_NewGameSettings_HPP
#define UE4SS_SDK_BP_NewGameSettings_HPP

class ABP_NewGameSettings_C : public AActor
{
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A8 (size: 0x8)
    FString Forename;                                                                 // 0x02B0 (size: 0x10)
    FString Surname;                                                                  // 0x02C0 (size: 0x10)
    class UStaticMesh* EyebrowsMesh;                                                  // 0x02D0 (size: 0x8)
    class UMaterialInstance* SkinColor;                                               // 0x02D8 (size: 0x8)
    TEnumAsByte<Enum_Occupation::Type> Occupation;                                    // 0x02E0 (size: 0x1)
    class UStaticMesh* HairMesh;                                                      // 0x02E8 (size: 0x8)
    class UMaterialInstance* HairColor;                                               // 0x02F0 (size: 0x8)
    class UStaticMesh* BeardMesh;                                                     // 0x02F8 (size: 0x8)
    class UMaterialInstance* BeardColor;                                              // 0x0300 (size: 0x8)
    FString Age;                                                                      // 0x0308 (size: 0x10)
    class UStaticMesh* MouthMesh;                                                     // 0x0318 (size: 0x8)
    class UStaticMesh* Accessory1Mesh;                                                // 0x0320 (size: 0x8)
    class UStaticMesh* Accessory2Mesh;                                                // 0x0328 (size: 0x8)
    class UStaticMesh* Accessory3Mesh;                                                // 0x0330 (size: 0x8)
    FString Sex;                                                                      // 0x0338 (size: 0x10)

}; // Size: 0x348

#endif

#ifndef UE4SS_SDK_BP_CharacterCreator_HPP
#define UE4SS_SDK_BP_CharacterCreator_HPP

class ABP_CharacterCreator_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UPointLightComponent* PointLight1;                                          // 0x02A0 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Mouth;                                                // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Accessory3;                                           // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Accessory2;                                           // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* Accessory1;                                           // 0x02C8 (size: 0x8)
    class UStaticMeshComponent* EyebrowsMesh;                                         // 0x02D0 (size: 0x8)
    class USkeletalMeshComponent* Torso;                                              // 0x02D8 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x02E0 (size: 0x8)
    class UStaticMeshComponent* HairMesh;                                             // 0x02E8 (size: 0x8)
    class UStaticMeshComponent* BeardMesh;                                            // 0x02F0 (size: 0x8)
    class USkeletalMeshComponent* LowerLegs;                                          // 0x02F8 (size: 0x8)
    class USkeletalMeshComponent* Legs;                                               // 0x0300 (size: 0x8)
    class USkeletalMeshComponent* Feet;                                               // 0x0308 (size: 0x8)
    class USkeletalMeshComponent* head;                                               // 0x0310 (size: 0x8)
    class USkeletalMeshComponent* Arms;                                               // 0x0318 (size: 0x8)
    class USkeletalMeshComponent* Hands;                                              // 0x0320 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x0328 (size: 0x8)
    float LightTL_NewTrack_0_5B74E44B4A6536A3886F7A9607115F97;                        // 0x0330 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> LightTL__Direction_5B74E44B4A6536A3886F7A9607115F97; // 0x0334 (size: 0x1)
    class UTimelineComponent* LightTL;                                                // 0x0338 (size: 0x8)

    void UserConstructionScript();
    void LightTL__FinishedFunc();
    void LightTL__UpdateFunc();
    void SetDefault();
    void ToggleLight(bool TurnOn);
    void ExecuteUbergraph_BP_CharacterCreator(int32 EntryPoint);
}; // Size: 0x340

#endif

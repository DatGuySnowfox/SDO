#ifndef UE4SS_SDK_BP_PlayerCharacter_HPP
#define UE4SS_SDK_BP_PlayerCharacter_HPP

class ABP_PlayerCharacter_C : public ABP_MasterPlayer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0660 (size: 0x8)
    class UCameraComponent* FirstPersonCamera;                                        // 0x0668 (size: 0x8)
    class USpringArmComponent* SpringArmFP;                                           // 0x0670 (size: 0x8)
    class UFishingComponent_C* FishingComponent;                                      // 0x0678 (size: 0x8)
    class UDLWE_Interaction_C* FootR;                                                 // 0x0680 (size: 0x8)
    class UDLWE_Interaction_C* FootL;                                                 // 0x0688 (size: 0x8)
    class USkeletalMeshComponent* Torso;                                              // 0x0690 (size: 0x8)
    class UMediaSoundComponent* MediaSound;                                           // 0x0698 (size: 0x8)
    class UArrowComponent* ForwardTrace;                                              // 0x06A0 (size: 0x8)
    class UArrowComponent* ForwardArrow;                                              // 0x06A8 (size: 0x8)
    class UPostProcessComponent* WaterPostProcess;                                    // 0x06B0 (size: 0x8)
    class USpringArmComponent* SpringArmFlashlight;                                   // 0x06B8 (size: 0x8)
    class URadioComponent_C* RadioComponent;                                          // 0x06C0 (size: 0x8)
    class UJigItemOverItemComp_C* JigItemOverItemComp;                                // 0x06C8 (size: 0x8)
    class USceneCaptureComponent2D* ThumbnailCapture;                                 // 0x06D0 (size: 0x8)
    class UBP_JigHelperComp_C* BP_JigHelperComp;                                      // 0x06D8 (size: 0x8)
    class UBPC_MinimapSystem_C* MinimapSystemComponent;                               // 0x06E0 (size: 0x8)
    class USkeletalMeshComponent* Biceps;                                             // 0x06E8 (size: 0x8)
    class USkeletalMeshComponent* LowerThighs;                                        // 0x06F0 (size: 0x8)
    class USwimmingComponent_C* SwimmingComponent;                                    // 0x06F8 (size: 0x8)
    class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;             // 0x0700 (size: 0x8)
    class ULockPickingComponent_C* LockPickingComponent;                              // 0x0708 (size: 0x8)
    class USpotLightComponent* Flashlight;                                            // 0x0710 (size: 0x8)
    class UStaticMeshComponent* Mouth;                                                // 0x0718 (size: 0x8)
    class UStaticMeshComponent* Accessory3;                                           // 0x0720 (size: 0x8)
    class UStaticMeshComponent* Accessory2;                                           // 0x0728 (size: 0x8)
    class UStaticMeshComponent* Accessory1;                                           // 0x0730 (size: 0x8)
    class USkeletalMeshComponent* Clothing_Feet;                                      // 0x0738 (size: 0x8)
    class USkeletalMeshComponent* Clothing_Legs;                                      // 0x0740 (size: 0x8)
    class USkeletalMeshComponent* Clothing_Torso;                                     // 0x0748 (size: 0x8)
    class USkeletalMeshComponent* Head;                                               // 0x0750 (size: 0x8)
    class USkeletalMeshComponent* Clothing_Gloves;                                    // 0x0758 (size: 0x8)
    class USkeletalMeshComponent* Arms;                                               // 0x0760 (size: 0x8)
    class UStaticMeshComponent* EyebrowsMesh;                                         // 0x0768 (size: 0x8)
    class USkeletalMeshComponent* Feet;                                               // 0x0770 (size: 0x8)
    class USkeletalMeshComponent* LowerLegs;                                          // 0x0778 (size: 0x8)
    class USkeletalMeshComponent* Legs;                                               // 0x0780 (size: 0x8)
    class USkeletalMeshComponent* Hands;                                              // 0x0788 (size: 0x8)
    class USkeletalMeshComponent* Clothing_Armor;                                     // 0x0790 (size: 0x8)
    class UStaticMeshComponent* HairMesh;                                             // 0x0798 (size: 0x8)
    class UStaticMeshComponent* BeardMesh;                                            // 0x07A0 (size: 0x8)
    class UMedicalComponent_C* MedicalComponent;                                      // 0x07A8 (size: 0x8)
    class UVehicleDrivingComponent_C* VehicleDrivingComponent;                        // 0x07B0 (size: 0x8)
    class UBuildingComponent_C* BuildingComponent;                                    // 0x07B8 (size: 0x8)
    class UAIOInvokerComponent* AIOInvoker;                                           // 0x07C0 (size: 0x8)
    class URadiationComponent_C* RadiationComponent;                                  // 0x07C8 (size: 0x8)
    class UHungerThirstComponent_C* Hunger&ThirstComponent;                           // 0x07D0 (size: 0x8)
    class UStaminaComponent_C* StaminaComponent;                                      // 0x07D8 (size: 0x8)
    class UPhotoModeComponent_C* PhotoModeComponent;                                  // 0x07E0 (size: 0x8)
    class UJigContextMenuComp_C* JigContextMenuComp;                                  // 0x07E8 (size: 0x8)
    class UBP_JigComponent_C* BP_JigMultiplayer;                                      // 0x07F0 (size: 0x8)
    float TL_VehicleCameraArmLength_Lerp_7588131C4E4F5C300E533FA3285934E6;            // 0x07F8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> TL_VehicleCameraArmLength__Direction_7588131C4E4F5C300E533FA3285934E6; // 0x07FC (size: 0x1)
    class UTimelineComponent* TL_VehicleCameraArmLength;                              // 0x0800 (size: 0x8)
    float Timeline_Resetleaning_Reset_FA84921041E9C0ADBB6D768EBDA4A87D;               // 0x0808 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_Resetleaning__Direction_FA84921041E9C0ADBB6D768EBDA4A87D; // 0x080C (size: 0x1)
    class UTimelineComponent* Timeline_Resetleaning;                                  // 0x0810 (size: 0x8)
    float PlayerZoom_TP_Track_3A775CB94419B1D2414D34B5F1658F0B;                       // 0x0818 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> PlayerZoom_TP__Direction_3A775CB94419B1D2414D34B5F1658F0B; // 0x081C (size: 0x1)
    class UTimelineComponent* PlayerZoom_TP;                                          // 0x0820 (size: 0x8)
    float PlayerUnZoom_TP_Track_FC96BE38448431843E81FB9EC80A1BF6;                     // 0x0828 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> PlayerUnZoom_TP__Direction_FC96BE38448431843E81FB9EC80A1BF6; // 0x082C (size: 0x1)
    class UTimelineComponent* PlayerUnZoom_TP;                                        // 0x0830 (size: 0x8)
    float PlayerUnZoom_FP_Track_5DF13A304BBE69C98A28DBB5C19631D0;                     // 0x0838 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> PlayerUnZoom_FP__Direction_5DF13A304BBE69C98A28DBB5C19631D0; // 0x083C (size: 0x1)
    class UTimelineComponent* PlayerUnZoom_FP;                                        // 0x0840 (size: 0x8)
    float PlayerZoom_FP_Track_5954C52C43BD7C27E8039A983E6F3B0B;                       // 0x0848 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> PlayerZoom_FP__Direction_5954C52C43BD7C27E8039A983E6F3B0B; // 0x084C (size: 0x1)
    class UTimelineComponent* PlayerZoom_FP;                                          // 0x0850 (size: 0x8)
    float RotationYawTimeline_Yaw_4446BE074AC61F8DB39CFE97B47049A9;                   // 0x0858 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> RotationYawTimeline__Direction_4446BE074AC61F8DB39CFE97B47049A9; // 0x085C (size: 0x1)
    class UTimelineComponent* RotationYawTimeline;                                    // 0x0860 (size: 0x8)
    float Timeline_FishRodMontage_Rotate_2DBCA9244154411C98EB89975785F951;            // 0x0868 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_FishRodMontage__Direction_2DBCA9244154411C98EB89975785F951; // 0x086C (size: 0x1)
    class UTimelineComponent* Timeline_FishRodMontage;                                // 0x0870 (size: 0x8)
    float ResetCameraPositionTimeline_Reset_C6AFBB944191D66513989F94599769DF;         // 0x0878 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> ResetCameraPositionTimeline__Direction_C6AFBB944191D66513989F94599769DF; // 0x087C (size: 0x1)
    class UTimelineComponent* ResetCameraPositionTimeline;                            // 0x0880 (size: 0x8)
    float Timeline_SwitchCameraRight_Right_C5E1DC9B4F17A9588445EABA8385FF44;          // 0x0888 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_SwitchCameraRight__Direction_C5E1DC9B4F17A9588445EABA8385FF44; // 0x088C (size: 0x1)
    class UTimelineComponent* Timeline_SwitchCameraRight;                             // 0x0890 (size: 0x8)
    float Timeline_SwitchCameraLeft_Left_B5F81ED646608D51851FEAADA49111F4;            // 0x0898 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_SwitchCameraLeft__Direction_B5F81ED646608D51851FEAADA49111F4; // 0x089C (size: 0x1)
    class UTimelineComponent* Timeline_SwitchCameraLeft;                              // 0x08A0 (size: 0x8)
    float ADSZoomOut_ADSLocation_9155FDF44A60A85A49930CAD5DFC1A50;                    // 0x08A8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> ADSZoomOut__Direction_9155FDF44A60A85A49930CAD5DFC1A50; // 0x08AC (size: 0x1)
    class UTimelineComponent* ADSZoomOut;                                             // 0x08B0 (size: 0x8)
    float ADSZoomIn_ADSLocation_150AD15340B715D99EB18493329A1927;                     // 0x08B8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> ADSZoomIn__Direction_150AD15340B715D99EB18493329A1927; // 0x08BC (size: 0x1)
    class UTimelineComponent* ADSZoomIn;                                              // 0x08C0 (size: 0x8)
    float CrouchSpringArm_SpringArmZLocation_524941E14908417C68AEE6A68D059678;        // 0x08C8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> CrouchSpringArm__Direction_524941E14908417C68AEE6A68D059678; // 0x08CC (size: 0x1)
    class UTimelineComponent* CrouchSpringArm;                                        // 0x08D0 (size: 0x8)
    class ABP_FirearmPickup_C* CurrentFiringWeapon;                                   // 0x08D8 (size: 0x8)
    class UBP_Ingame_C* InGameUI;                                                     // 0x08E0 (size: 0x8)
    TArray<UJSIContainer_C*> MainJSIContainers;                                       // 0x08E8 (size: 0x10)
    double ThirdPersonCameraTraceDistance;                                            // 0x08F8 (size: 0x8)
    double FirstPersonCameraTraceDistance;                                            // 0x0900 (size: 0x8)
    FS_ServerEquippedItems ServerEquippedItems;                                       // 0x0908 (size: 0x9D8)
    TMap<uint8, int32> EquipmentUIDs;                                                 // 0x12E0 (size: 0x50)
    TMap<FName, uint8> EquipmentIDSlots;                                              // 0x1330 (size: 0x50)
    double BinocularsZoomValue;                                                       // 0x1380 (size: 0x8)
    TEnumAsByte<Enum_Firearms::Type> WeaponType;                                      // 0x1388 (size: 0x1)
    bool PlayerDead?;                                                                 // 0x1389 (size: 0x1)
    bool IsADS?;                                                                      // 0x138A (size: 0x1)
    bool HoldingFirearm?;                                                             // 0x138B (size: 0x1)
    bool UsingBinoculars?;                                                            // 0x138C (size: 0x1)
    bool FlashlightOn?;                                                               // 0x138D (size: 0x1)
    class UBinoculars_C* BinocularsUI;                                                // 0x1390 (size: 0x8)
    bool Sprinting?;                                                                  // 0x1398 (size: 0x1)
    bool HoldingMelee?;                                                               // 0x1399 (size: 0x1)
    bool DistractNoisePlaying?;                                                       // 0x139A (size: 0x1)
    bool IsPlayerRolling?;                                                            // 0x139B (size: 0x1)
    class UJSIContainer_C* OnMagInstalled;                                            // 0x13A0 (size: 0x8)
    bool PlayerNearWall?;                                                             // 0x13A8 (size: 0x1)
    bool PlayerUsingNightVision?;                                                     // 0x13A9 (size: 0x1)
    FRotator PlayerRotation;                                                          // 0x13B0 (size: 0x18)
    class ABP_PlayerController_C* PlayerController;                                   // 0x13C8 (size: 0x8)
    bool InMeleeStance?;                                                              // 0x13D0 (size: 0x1)
    FTimerHandle RegenHealthTimer;                                                    // 0x13D8 (size: 0x8)
    bool PlayerInFirstPerson?;                                                        // 0x13E0 (size: 0x1)
    bool Should Climb;                                                                // 0x13E1 (size: 0x1)
    bool Is Climbing?;                                                                // 0x13E2 (size: 0x1)
    FVector Wall Location;                                                            // 0x13E8 (size: 0x18)
    FVector Wall Normal;                                                              // 0x1400 (size: 0x18)
    FVector Wall Height;                                                              // 0x1418 (size: 0x18)
    FVector Back Wall Height;                                                         // 0x1430 (size: 0x18)
    bool Wall Thick;                                                                  // 0x1448 (size: 0x1)
    bool Can Climb;                                                                   // 0x1449 (size: 0x1)
    double InvertValue;                                                               // 0x1450 (size: 0x8)
    bool InvertValueEnabled;                                                          // 0x1458 (size: 0x1)
    FTransform SaveLocation;                                                          // 0x1460 (size: 0x60)
    bool ActionBarUIToggle;                                                           // 0x14C0 (size: 0x1)
    bool PlayerUsingScope?;                                                           // 0x14C1 (size: 0x1)
    bool AutoRun?;                                                                    // 0x14C2 (size: 0x1)
    bool EnemiesClose;                                                                // 0x14C3 (size: 0x1)
    bool Combat;                                                                      // 0x14C4 (size: 0x1)
    double CurrentFieldOfView;                                                        // 0x14C8 (size: 0x8)
    double CameraOffset;                                                              // 0x14D0 (size: 0x8)
    double CameraOffsetVertical;                                                      // 0x14D8 (size: 0x8)
    bool JournalOpen?;                                                                // 0x14E0 (size: 0x1)
    bool RadialMenuOpen?;                                                             // 0x14E1 (size: 0x1)
    bool ClimbingLadder?;                                                             // 0x14E2 (size: 0x1)
    FVector Distance_Location;                                                        // 0x14E8 (size: 0x18)
    FTimerHandle StopScopeSwayTimer;                                                  // 0x1500 (size: 0x8)
    bool InVehicle?;                                                                  // 0x1508 (size: 0x1)
    FBP_PlayerCharacter_CLoadInventory LoadInventory;                                 // 0x1510 (size: 0x10)
    void LoadInventory();
    bool FreeLook?;                                                                   // 0x1520 (size: 0x1)
    class UScopesUI_C* ScopesUI;                                                      // 0x1528 (size: 0x8)
    bool Reloading?;                                                                  // 0x1530 (size: 0x1)
    bool DeathbringerSkillUsed?;                                                      // 0x1531 (size: 0x1)
    float ThirdPersonTargetArmLength;                                                 // 0x1534 (size: 0x4)
    bool Sitting?;                                                                    // 0x1538 (size: 0x1)
    bool Attacking?;                                                                  // 0x1539 (size: 0x1)
    bool CrouchSprinting?;                                                            // 0x153A (size: 0x1)
    float CurrentFieldOfViewFP;                                                       // 0x153C (size: 0x4)
    bool IsPlayerMale?;                                                               // 0x1540 (size: 0x1)
    class UMaterialInstance* SkinColor;                                               // 0x1548 (size: 0x8)
    FBP_PlayerCharacter_CKeepInventoryOnDeath KeepInventoryOnDeath;                   // 0x1550 (size: 0x10)
    void KeepInventoryOnDeath();
    bool SprintingRestricted?;                                                        // 0x1560 (size: 0x1)
    class UMaterialInstance* Hair Color;                                              // 0x1568 (size: 0x8)
    class UMaterialInstance* Beard Color;                                             // 0x1570 (size: 0x8)
    bool CurrentlyChangingWeapon?;                                                    // 0x1578 (size: 0x1)
    FBP_PlayerCharacter_CAssignTechTree AssignTechTree;                               // 0x1580 (size: 0x10)
    void AssignTechTree();
    FRotator SmoothRotYStart;                                                         // 0x1590 (size: 0x18)
    bool IsRotating?;                                                                 // 0x15A8 (size: 0x1)
    bool NoClipEnabled;                                                               // 0x15A9 (size: 0x1)
    class UAudioComponent* UnderwaterAudio;                                           // 0x15B0 (size: 0x8)
    class UAudioComponent* WaterAudio;                                                // 0x15B8 (size: 0x8)
    FTimerHandle BatteryTimerFlashlight;                                              // 0x15C0 (size: 0x8)
    bool PlayerZoomingIn?;                                                            // 0x15C8 (size: 0x1)
    bool ShowStartingQuests?;                                                         // 0x15C9 (size: 0x1)
    bool HipFire?;                                                                    // 0x15CA (size: 0x1)
    double ThirdPersonBuildingTraceDistance;                                          // 0x15D0 (size: 0x8)
    double FirstPersonBuildingTraceDistance;                                          // 0x15D8 (size: 0x8)
    float Sensitivity;                                                                // 0x15E0 (size: 0x4)
    double AimingSensitivity;                                                         // 0x15E8 (size: 0x8)
    double ScopeSensitivity;                                                          // 0x15F0 (size: 0x8)
    FBP_PlayerCharacter_CPlayerDead PlayerDead;                                       // 0x15F8 (size: 0x10)
    void PlayerDead();
    FBP_PlayerCharacter_CSetFirstPersonView SetFirstPersonView;                       // 0x1608 (size: 0x10)
    void SetFirstPersonView(bool EnterFP?);
    double Lean;                                                                      // 0x1618 (size: 0x8)
    double Socket Offset Y;                                                           // 0x1620 (size: 0x8)
    bool LeanLeft?;                                                                   // 0x1628 (size: 0x1)
    bool LeanRight?;                                                                  // 0x1629 (size: 0x1)
    FRotator HeadRot;                                                                 // 0x1630 (size: 0x18)
    FTimerHandle BurningTimer;                                                        // 0x1648 (size: 0x8)
    FPostProcessSettings DefaultCameraPP;                                             // 0x1650 (size: 0x760)
    bool TraceToWorldEnabled;                                                         // 0x1DB0 (size: 0x1)
    bool Burning?;                                                                    // 0x1DB1 (size: 0x1)
    FVector LeftHandLoc;                                                              // 0x1DB8 (size: 0x18)
    class UParticleSystemComponent* BurningEffect;                                    // 0x1DD0 (size: 0x8)
    class UDebugMenu_C* DebugMenu;                                                    // 0x1DD8 (size: 0x8)
    bool PrimaryWeaponEquipped?;                                                      // 0x1DE0 (size: 0x1)
    bool SecondaryWeaponEquipped?;                                                    // 0x1DE1 (size: 0x1)
    bool SidearmWeaponEquipped?;                                                      // 0x1DE2 (size: 0x1)
    bool MeleeWeaponEquipped?;                                                        // 0x1DE3 (size: 0x1)
    bool FacewearEquipped?;                                                           // 0x1DE4 (size: 0x1)
    bool HeadwearEquipped?;                                                           // 0x1DE5 (size: 0x1)
    bool EyewearEquipped?;                                                            // 0x1DE6 (size: 0x1)
    bool AccessoryEquipped?;                                                          // 0x1DE7 (size: 0x1)
    bool ClothingTorsoEquipped?;                                                      // 0x1DE8 (size: 0x1)
    bool ClothingGlovesEquipped?;                                                     // 0x1DE9 (size: 0x1)
    bool ClothingLegsEquipped?;                                                       // 0x1DEA (size: 0x1)
    bool ClothingFeetEquipped?;                                                       // 0x1DEB (size: 0x1)
    bool ContainerEquipped?;                                                          // 0x1DEC (size: 0x1)
    bool BodyArmorEquipped?;                                                          // 0x1DED (size: 0x1)
    bool BackpackEquipped?;                                                           // 0x1DEE (size: 0x1)
    bool ThrowableEquipped?;                                                          // 0x1DEF (size: 0x1)
    bool FlashlightEquipped?;                                                         // 0x1DF0 (size: 0x1)
    bool FishingRodEquipped?;                                                         // 0x1DF1 (size: 0x1)
    bool SpawnStartingItems?;                                                         // 0x1DF2 (size: 0x1)
    FVector DirectionFreeLook;                                                        // 0x1DF8 (size: 0x18)
    FRotator FLRotation;                                                              // 0x1E10 (size: 0x18)
    class UJigsawItem_DataAsset_C* PendingReloadMagID;                                // 0x1E28 (size: 0x8)
    bool ShowDaysSurvivedOnSpawn?;                                                    // 0x1E30 (size: 0x1)
    class UStaticMeshComponent* HeldItemInMontage;                                    // 0x1E38 (size: 0x8)

    void AI Is Dead?(bool& Dead);
    void AI Can Interact?(bool& Can Interact);
    void Actor Attack Target(class AActor*& Attack Target);
    void ActorDead?(bool& Dead?);
    void ComponentsToSave(TArray<UActorComponent*>& Components);
    void GetInteractionDistances(double& InteractDistance);
    void IsPlayerInVehicle?(bool& InVehicle);
    void IsGPSEquipped?(bool& GPS?);
    void IsBurning?(bool& Burning?);
    void GetInGameUI(class UBP_Ingame_C*& UI);
    void GetPlayerRef(class ABP_PlayerCharacter_C*& Player);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<FGameplayTag, FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<FString, FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void JigMP_GetLootWidget(class UWidget*& Result, bool& Valid?);
    void JigMP_OnContainersSwap(class UJSIContainer_C* Container1, class UJSIContainer_C* Container2, bool& Result);
    void JigMP_OnItemDropped(FGuid ItemUID, FGuid FromContainerUID, class AActor* ActorRef, bool& Result);
    void JigMP_OnItemRemoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* ToComponent, bool& Result);
    void JigMP_OnItemAdded(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* FromComponent, bool& Result);
    void JigMP_OnItemMoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, bool& Result);
    void JigMP_OnTwoContainersSwap(FName FromContainer, FName ToContainer, bool& Result);
    void JigMP_OnUpdateChamberUID(FGuid NewUID, bool& Result);
    void JigMP_OnItemConsumed(FRepItemInfo ConsumedItem, int32 Amount, int32 Remaining, FRepItemInfo InsideOf, FName FromContainer, FString CustomData, bool CallbackToPlayer?, bool& Result);
    void JigMP_OnMainContainerItemRemoved(FGuid FromContainerUID, FGuid ItemUID, FName ContainerName, FRepItemInfo ItemRemovedInfo, bool& Result);
    void JigMP_OnMainContainerItemAdded(FGuid ToContainerUID, FGuid ItemUID, FName ToContainerName, FRepItemInfo ItemInfo, FName FromMain, bool& Result, class AActor*& ActorRef);
    void JigMP_OnPickupAdded(FRepItemInfo Info, FGuid ItemUID, FGuid ToContainerUID, FName ToContainer, bool& Result);
    void JigMP_OnPickupEquipped(class AActor* ActorRef, FName ToContainerName, FGuid UID, FGuid ToContainerUID, FRepItemInfo Info, bool& Result, class AActor*& OverrideActor);
    void JigMP_OnRequestDropItem(FRepItemInfo ItemInfo, FName ContainerName, class AActor*& ActorRef);
    void SetPreviewActor(class AActor* Actor, bool& Result);
    void GetTraceActorsToIgnore(TArray<AActor*>& Actors);
    void OnClientDataRequested(TArray<FGuid>& ActionbarUIDs, bool& Result);
    void OnSpecialContainerattachmentUpdated(class UJSI_Slot_C* ItemRef, bool& Result);
    void OnLoadSavedDataRequested(bool& Result);
    void OnInteractActorOverDistance(class AActor* ActorRef, bool& Result);
    void JigCheckWeaponAmmoCount(bool& Result);
    void GetAnimationInfo(FGameplayTag& ActiveSlot, double& Leaning, bool& ADS, bool& Crouched, bool& IsFirstPerson?, class UJigsawItem_DataAsset_C*& EquippedDA);
    void OnPickupInteractExecuted(class AActor* PickupRef, class UJSIContainer_C* TargetContainer, bool& Result);
    void RequestServerData(class UActorComponent* TargetComp, bool& Result);
    void AddJigWidgetToContent(class UUserWidget* WidgetRef, FText ContentName, bool& Result);
    void CalcFPCamera(FTransform& CameraTransform);
    void LandedSoundEffect();
    void AfterDeath(bool Suicide?);
    void HandleNotifies(FName NotifyName);
    void FreeLookMaths(double Value, FVector& Vector);
    void ToggleTacticalAttachment(bool ADS?);
    void GetArmorDamageReduction(TArray<FS_ItemStat>& Stats, double& Reduction);
    void TraceToWorldChecks();
    void SetStartingItems();
    void OnRep_FishingRodEquipped?();
    void OnRep_FlashlightEquipped?();
    void OnRep_ThrowableEquipped?();
    void OnRep_BackpackEquipped?();
    void OnRep_ContainerEquipped?();
    void OnRep_BodyArmorEquipped?();
    void OnRep_ClothingFeetEquipped?();
    void OnRep_ClothingLegsEquipped?();
    void OnRep_ClothingGlovesEquipped?();
    void OnRep_ClothingTorsoEquipped?();
    void OnRep_AccessoryEquipped?();
    void OnRep_EyewearEquipped?();
    void OnRep_HeadwearEquipped?();
    void OnRep_FacewearEquipped?();
    void OnRep_MeleeWeaponEquipped?();
    void OnRep_SidearmWeaponEquipped?();
    void OnRep_SecondaryWeaponEquipped?();
    void OnRep_PrimaryWeaponEquipped?();
    void CheckNextState(FGameplayTag State, bool& Return);
    void SetWeaponsAndBackpackVisibility(bool Hide?);
    void GrenadeThrowSpeed(double& Speed);
    void SetCurrentWeaponVisibility(bool Hidden?);
    void SetFPMeshesVisibility(bool FP?);
    void GetInventoryWidgetRef(class UBP_Inventory_C*& Inventory);
    void BinocularsVisibility(bool Visible?, double BinocularsZoomValue);
    void OnRep_ClimbingLadder?();
    void HotBarConsume(class UJSI_Slot_C* ItemRef);
    void MarkEnemy();
    void NightVisionBatteryCheck(bool& Successful);
    void SwimFast();
    void WaterAudioCheck();
    void CameraWaterEffect();
    void SetCombatStateOnUnequip();
    void NightVisionCheck();
    void PlayerStompTrace();
    void SetUIVisibility(ESlateVisibility InVisibility);
    void AssignSkillTree();
    void ActionBarEquipUI(bool Unequip, FGameplayTag Item);
    void PlayHitAudio();
    void TraceForAI();
    void LoadKeepPlayerInventoryFromDeath();
    void PlayerShoveTrace();
    void UpdateBodyParts(FName Name);
    void CalculatePlayerDamage(double Damage, const class UDamageType* DamageType, double& NewDamage);
    void SetSexMesh();
    void BodyPartVisibility(FBodyPartSettings Parts, bool IsPlayerMale?, FName Body Part, bool UpdateAllBodyParts?);
    void Crouch Sprint();
    void Update Item Durability(double - Durability, const class UJSI_Slot_C*& Slot);
    void Update Equipped Item Durability(double - Durability, FGameplayTag Slot);
    void SetPostProcessHealth(bool Damage);
    void AimedFocus();
    void Set Aimed Focus(bool Aimed);
    void SpawnDeadPlayerLoot();
    void DeathBringer(bool& Used?);
    void CameraShake(bool Sniper?);
    void ChangePlayerPerspective(bool EnterFP?, bool OverrideFPBool?);
    void SetVisibilityFirstPerson();
    void HideAttachments(class AActor* Actor, bool NewHidden);
    void LoadPlayerInventory();
    void Footstep Sound(class USoundBase* Sound, FVector Location, float VolumeMultiplier, float MaxRange);
    void GetInVehicle(class ABP_VehicleMaster_C* Target);
    void SetPlayerItemsVisibility(bool Hidden);
    void ScopeFunction(double Zoom, class UWidget* Scope);
    void SightFunction(double Zoom);
    void RadialMenuSelection(class UUI_RadialMenu_C* RadialMenu);
    void ClearUI();
    void ShowCompassWidget(bool Show?);
    void RegenHealth();
    void SetFOVBackAiming();
    void NewFOVAiming(double ZoomValue);
    void DestroyActors();
    void NightVisionOn(bool Enabled);
    void WeaponWallTrace();
    void SetEnableScope(bool Enable?, class UWidget* Scope);
    void ChamberAfterReloadWeapon(bool SkipChamberAnim, bool& Result);
    void UpdateCombatState(int32 BlendSpace);
    void FlashlightToggle(bool FlashlightOn?);
    void FlashlightOff(bool PlaySound?);
    bool IsAiming?();
    void GetCurrentActiveWeapon(class AActor*& EquippedWeapon);
    void UpdateAmmoCount();
    void ServerFuncChangeActiveSlot(FGameplayTag ToActiveSlot);
    void EquipActorToSocket(class UJigsawItem_DataAsset_C* DA, class AActor* ActorRef, bool IsSecondary);
    void UserConstructionScript();
    void ADSZoomIn__FinishedFunc();
    void ADSZoomIn__UpdateFunc();
    void ADSZoomOut__FinishedFunc();
    void ADSZoomOut__UpdateFunc();
    void Timeline_Resetleaning__FinishedFunc();
    void Timeline_Resetleaning__UpdateFunc();
    void Timeline_SwitchCameraRight__FinishedFunc();
    void Timeline_SwitchCameraRight__UpdateFunc();
    void Timeline_SwitchCameraLeft__FinishedFunc();
    void Timeline_SwitchCameraLeft__UpdateFunc();
    void PlayerUnZoom_TP__FinishedFunc();
    void PlayerUnZoom_TP__UpdateFunc();
    void PlayerUnZoom_FP__FinishedFunc();
    void PlayerUnZoom_FP__UpdateFunc();
    void PlayerZoom_TP__FinishedFunc();
    void PlayerZoom_TP__UpdateFunc();
    void PlayerZoom_FP__FinishedFunc();
    void PlayerZoom_FP__UpdateFunc();
    void CrouchSpringArm__FinishedFunc();
    void CrouchSpringArm__UpdateFunc();
    void RotationYawTimeline__FinishedFunc();
    void RotationYawTimeline__UpdateFunc();
    void ResetCameraPositionTimeline__FinishedFunc();
    void ResetCameraPositionTimeline__UpdateFunc();
    void Timeline_FishRodMontage__FinishedFunc();
    void Timeline_FishRodMontage__UpdateFunc();
    void TL_VehicleCameraArmLength__FinishedFunc();
    void TL_VehicleCameraArmLength__UpdateFunc();
    void OnNotifyEnd_18A4BA1B4C352187E5F63BBC06D7E487(FName NotifyName);
    void OnNotifyBegin_18A4BA1B4C352187E5F63BBC06D7E487(FName NotifyName);
    void OnInterrupted_18A4BA1B4C352187E5F63BBC06D7E487(FName NotifyName);
    void OnBlendOut_18A4BA1B4C352187E5F63BBC06D7E487(FName NotifyName);
    void OnCompleted_18A4BA1B4C352187E5F63BBC06D7E487(FName NotifyName);
    void OnNotifyEnd_794CB1FB48F1E8C23FD1B5916BE712BC(FName NotifyName);
    void OnNotifyBegin_794CB1FB48F1E8C23FD1B5916BE712BC(FName NotifyName);
    void OnInterrupted_794CB1FB48F1E8C23FD1B5916BE712BC(FName NotifyName);
    void OnBlendOut_794CB1FB48F1E8C23FD1B5916BE712BC(FName NotifyName);
    void OnCompleted_794CB1FB48F1E8C23FD1B5916BE712BC(FName NotifyName);
    void OnNotifyEnd_18A4BA1B4C352187E5F63BBCEA2A96EB(FName NotifyName);
    void OnNotifyBegin_18A4BA1B4C352187E5F63BBCEA2A96EB(FName NotifyName);
    void OnInterrupted_18A4BA1B4C352187E5F63BBCEA2A96EB(FName NotifyName);
    void OnBlendOut_18A4BA1B4C352187E5F63BBCEA2A96EB(FName NotifyName);
    void OnCompleted_18A4BA1B4C352187E5F63BBCEA2A96EB(FName NotifyName);
    void OnNotifyEnd_794CB1FB48F1E8C23FD1B591871A60D0(FName NotifyName);
    void OnNotifyBegin_794CB1FB48F1E8C23FD1B591871A60D0(FName NotifyName);
    void OnInterrupted_794CB1FB48F1E8C23FD1B591871A60D0(FName NotifyName);
    void OnBlendOut_794CB1FB48F1E8C23FD1B591871A60D0(FName NotifyName);
    void OnCompleted_794CB1FB48F1E8C23FD1B591871A60D0(FName NotifyName);
    void OnNotifyEnd_18A4BA1B4C352187E5F63BBCF14AEA42(FName NotifyName);
    void OnNotifyBegin_18A4BA1B4C352187E5F63BBCF14AEA42(FName NotifyName);
    void OnInterrupted_18A4BA1B4C352187E5F63BBCF14AEA42(FName NotifyName);
    void OnBlendOut_18A4BA1B4C352187E5F63BBCF14AEA42(FName NotifyName);
    void OnCompleted_18A4BA1B4C352187E5F63BBCF14AEA42(FName NotifyName);
    void InpActEvt_IA_FishingRod_K2Node_EnhancedInputActionEvent_78(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_WeaponScroll_K2Node_EnhancedInputActionEvent_77(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Hotbar8_K2Node_EnhancedInputActionEvent_76(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Hotbar7_K2Node_EnhancedInputActionEvent_75(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Hotbar6_K2Node_EnhancedInputActionEvent_74(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Unequip_K2Node_EnhancedInputActionEvent_73(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_EquipMelee_K2Node_EnhancedInputActionEvent_72(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_EquipSidearm_K2Node_EnhancedInputActionEvent_71(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_EquipSecondary_K2Node_EnhancedInputActionEvent_70(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_EquipPrimary_K2Node_EnhancedInputActionEvent_69(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void OnNotifyEnd_8ACC82B24257259750B1BA93E4DD7458(FName NotifyName);
    void OnNotifyBegin_8ACC82B24257259750B1BA93E4DD7458(FName NotifyName);
    void OnInterrupted_8ACC82B24257259750B1BA93E4DD7458(FName NotifyName);
    void OnBlendOut_8ACC82B24257259750B1BA93E4DD7458(FName NotifyName);
    void OnCompleted_8ACC82B24257259750B1BA93E4DD7458(FName NotifyName);
    void InpActEvt_IA_ToggleTactical_K2Node_EnhancedInputActionEvent_68(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_ChangeFireMode_K2Node_EnhancedInputActionEvent_67(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Reload_K2Node_EnhancedInputActionEvent_66(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PrimaryAction_K2Node_EnhancedInputActionEvent_65(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PrimaryAction_K2Node_EnhancedInputActionEvent_64(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void OnNotifyEnd_E4F8DD374225FD503766049460992D4F(FName NotifyName);
    void OnNotifyBegin_E4F8DD374225FD503766049460992D4F(FName NotifyName);
    void OnInterrupted_E4F8DD374225FD503766049460992D4F(FName NotifyName);
    void OnBlendOut_E4F8DD374225FD503766049460992D4F(FName NotifyName);
    void OnCompleted_E4F8DD374225FD503766049460992D4F(FName NotifyName);
    void OnNotifyEnd_92A8FD864C220BDC1EC15CB067573728(FName NotifyName);
    void OnNotifyBegin_92A8FD864C220BDC1EC15CB067573728(FName NotifyName);
    void OnInterrupted_92A8FD864C220BDC1EC15CB067573728(FName NotifyName);
    void OnBlendOut_92A8FD864C220BDC1EC15CB067573728(FName NotifyName);
    void OnCompleted_92A8FD864C220BDC1EC15CB067573728(FName NotifyName);
    void OnNotifyEnd_794CB1FB48F1E8C23FD1B5919C7A1C79(FName NotifyName);
    void OnNotifyBegin_794CB1FB48F1E8C23FD1B5919C7A1C79(FName NotifyName);
    void OnInterrupted_794CB1FB48F1E8C23FD1B5919C7A1C79(FName NotifyName);
    void OnBlendOut_794CB1FB48F1E8C23FD1B5919C7A1C79(FName NotifyName);
    void OnCompleted_794CB1FB48F1E8C23FD1B5919C7A1C79(FName NotifyName);
    void InpActEvt_IA_Inventory_K2Node_EnhancedInputActionEvent_63(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Interact_K2Node_EnhancedInputActionEvent_62(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Interact_K2Node_EnhancedInputActionEvent_61(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_MouseScrollUp_K2Node_InputKeyEvent_14(FKey Key);
    void InpActEvt_MouseScrollDown_K2Node_InputKeyEvent_13(FKey Key);
    void OnNotifyEnd_18A4BA1B4C352187E5F63BBC6D3AE71F(FName NotifyName);
    void OnNotifyBegin_18A4BA1B4C352187E5F63BBC6D3AE71F(FName NotifyName);
    void OnInterrupted_18A4BA1B4C352187E5F63BBC6D3AE71F(FName NotifyName);
    void OnBlendOut_18A4BA1B4C352187E5F63BBC6D3AE71F(FName NotifyName);
    void OnCompleted_18A4BA1B4C352187E5F63BBC6D3AE71F(FName NotifyName);
    void OnCompleted_1FEA8E0440CCD023EE61E688DA21B216();
    void InpActEvt_IA_Aim_K2Node_EnhancedInputActionEvent_60(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Aim_K2Node_EnhancedInputActionEvent_59(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void OnNotifyEnd_794CB1FB48F1E8C23FD1B591000A1124(FName NotifyName);
    void OnNotifyBegin_794CB1FB48F1E8C23FD1B591000A1124(FName NotifyName);
    void OnInterrupted_794CB1FB48F1E8C23FD1B591000A1124(FName NotifyName);
    void OnBlendOut_794CB1FB48F1E8C23FD1B591000A1124(FName NotifyName);
    void OnCompleted_794CB1FB48F1E8C23FD1B591000A1124(FName NotifyName);
    void InpActEvt_IA_SwitchCameraRight_K2Node_EnhancedInputActionEvent_58(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_SwitchCameraRight_K2Node_EnhancedInputActionEvent_57(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_SwitchCameraLeft_K2Node_EnhancedInputActionEvent_56(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_SwitchCameraLeft_K2Node_EnhancedInputActionEvent_55(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_ScopeZoom_K2Node_EnhancedInputActionEvent_54(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_HoldBreath_K2Node_EnhancedInputActionEvent_53(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_HoldBreath_K2Node_EnhancedInputActionEvent_52(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_HoldBreath_K2Node_EnhancedInputActionEvent_51(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_ZoomCamera_K2Node_EnhancedInputActionEvent_50(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_ZoomCamera_K2Node_EnhancedInputActionEvent_49(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_ZoomCamera_K2Node_EnhancedInputActionEvent_48(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Jump_K2Node_EnhancedInputActionEvent_47(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Jump_K2Node_EnhancedInputActionEvent_46(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Screenshot_K2Node_EnhancedInputActionEvent_45(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PhotoMode_K2Node_EnhancedInputActionEvent_44(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_HideUI_K2Node_EnhancedInputActionEvent_43(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PrimaryAction_K2Node_EnhancedInputActionEvent_42(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Binoculars_K2Node_EnhancedInputActionEvent_41(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Flashlight_K2Node_EnhancedInputActionEvent_40(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Crouch_K2Node_EnhancedInputActionEvent_39(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Crouch_K2Node_EnhancedInputActionEvent_38(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_FloatUpwards_K2Node_EnhancedInputActionEvent_37(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_CameraRotation_K2Node_EnhancedInputActionEvent_36(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_MoveRight_K2Node_EnhancedInputActionEvent_35(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_MoveBackwards_K2Node_EnhancedInputActionEvent_34(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_MoveLeft_K2Node_EnhancedInputActionEvent_33(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_MoveForward_K2Node_EnhancedInputActionEvent_32(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_CancelAutoRun_K2Node_EnhancedInputActionEvent_31(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_AutoRun_K2Node_EnhancedInputActionEvent_30(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_FreeLook_K2Node_EnhancedInputActionEvent_29(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_FreeLook_K2Node_EnhancedInputActionEvent_28(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Distract_K2Node_EnhancedInputActionEvent_27(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Aim_K2Node_EnhancedInputActionEvent_26(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Aim_K2Node_EnhancedInputActionEvent_25(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PrimaryAction_K2Node_EnhancedInputActionEvent_24(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PrimaryAction_K2Node_EnhancedInputActionEvent_23(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Roll_K2Node_EnhancedInputActionEvent_22(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void OnNotifyEnd_1CFFBA0842634D5C8F18DFBCC527D2DC(FName NotifyName);
    void OnNotifyBegin_1CFFBA0842634D5C8F18DFBCC527D2DC(FName NotifyName);
    void OnInterrupted_1CFFBA0842634D5C8F18DFBCC527D2DC(FName NotifyName);
    void OnBlendOut_1CFFBA0842634D5C8F18DFBCC527D2DC(FName NotifyName);
    void OnCompleted_1CFFBA0842634D5C8F18DFBCC527D2DC(FName NotifyName);
    void InpActEvt_IA_Sprint_K2Node_EnhancedInputActionEvent_21(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Sprint_K2Node_EnhancedInputActionEvent_20(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_NightVision_K2Node_EnhancedInputActionEvent_19(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_ChangePerspective_K2Node_EnhancedInputActionEvent_18(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_RadialMenu_K2Node_EnhancedInputActionEvent_17(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_RadialMenu_K2Node_EnhancedInputActionEvent_16(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_RadialMenu_K2Node_EnhancedInputActionEvent_15(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_Alt_MouseScrollDown_K2Node_InputKeyEvent_12(FKey Key);
    void InpActEvt_Alt_MouseScrollUp_K2Node_InputKeyEvent_11(FKey Key);
    void InpActEvt_R_K2Node_InputKeyEvent_10(FKey Key);
    void InpActEvt_R_K2Node_InputKeyEvent_9(FKey Key);
    void InpActEvt_Q_K2Node_InputKeyEvent_8(FKey Key);
    void InpActEvt_E_K2Node_InputKeyEvent_7(FKey Key);
    void InpActEvt_Shift_F_K2Node_InputKeyEvent_6(FKey Key);
    void InpActEvt_IA_Aim_K2Node_EnhancedInputActionEvent_14(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PrimaryAction_K2Node_EnhancedInputActionEvent_13(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_MouseScrollDown_K2Node_InputKeyEvent_5(FKey Key);
    void InpActEvt_Shift_R_K2Node_InputKeyEvent_4(FKey Key);
    void InpActEvt_Shift_R_K2Node_InputKeyEvent_3(FKey Key);
    void InpActEvt_Shift_MouseScrollDown_K2Node_InputKeyEvent_2(FKey Key);
    void InpActEvt_Shift_MouseScrollUp_K2Node_InputKeyEvent_1(FKey Key);
    void InpActEvt_MouseScrollUp_K2Node_InputKeyEvent_0(FKey Key);
    void InpActEvt_IA_Journal_K2Node_EnhancedInputActionEvent_12(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Throwable_K2Node_EnhancedInputActionEvent_11(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_CameraScroll_K2Node_EnhancedInputActionEvent_10(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_ResetCameraPosition_K2Node_EnhancedInputActionEvent_9(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Jump_K2Node_EnhancedInputActionEvent_8(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_SitDown_K2Node_EnhancedInputActionEvent_7(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PrimaryAction_K2Node_EnhancedInputActionEvent_6(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Shove_K2Node_EnhancedInputActionEvent_5(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PrimaryAction_K2Node_EnhancedInputActionEvent_4(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_PauseMenu_K2Node_EnhancedInputActionEvent_3(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_QuickSave_K2Node_EnhancedInputActionEvent_2(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Map_K2Node_EnhancedInputActionEvent_1(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_DebugMenu_K2Node_EnhancedInputActionEvent_0(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void OnNotifyEnd_56A189DF479DA36D9FA05EBF47AB304F(FName NotifyName);
    void OnNotifyBegin_56A189DF479DA36D9FA05EBF47AB304F(FName NotifyName);
    void OnInterrupted_56A189DF479DA36D9FA05EBF47AB304F(FName NotifyName);
    void OnBlendOut_56A189DF479DA36D9FA05EBF47AB304F(FName NotifyName);
    void OnCompleted_56A189DF479DA36D9FA05EBF47AB304F(FName NotifyName);
    void AI Dead();
    void Actor Aim Focus(class AActor* Instigator);
    void Alert Actor Defenders(class AActor* Attacked, class AActor* Attacker);
    void Turret Destroyed Effect();
    void Turret Idle Start();
    void Turret Idle Stop();
    void Turret Start Reload();
    void Turret End Reload();
    void AI Alert(class AActor* Alert Actor);
    void AI End Alert();
    void AI Animation Switch(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void AI Camera Activate(bool Activate);
    void AI Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void ActorSaved();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<FGameplayTag, FText> Options);
    void OnEndInteract();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void InitiateGameSave();
    void StartExfil(double Time);
    void CancelExfil();
    void ExecuteExfil();
    void EquipFirearm(FGameplayTag ToSlot);
    void EquipFishingRod();
    void Event_WeaponCycle(bool Forward?);
    void UnequipEverything();
    void EquipMelee();
    void SERVER_ChangeActiveSlot(FGameplayTag ToActiveSlot);
    void OnActiveWeaponSlotChanged_Event_0(FGameplayTag Slot);
    void HandleOnActiveSlot();
    void PlayChamberAfterReloadMontage(class UAnimMontage* MontageToPlay, double Speed);
    void DropInfo_OnItemEquippedChange_Event_1(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void HandleMagInstalled();
    void StopHipFire(bool ChangeCrosshair?);
    void HipFireDelay();
    void OnInventoryOpenClose_Event(bool Opened?);
    void HandleInventoryOpenClose();
    void ReceivePossessed(class AController* NewController);
    void Client_AddUI();
    void DropInfo_OnItemEquippedChange_Event_0(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void ReceiveBeginPlay();
    void Event_CharacterLag(bool NewValue);
    void Event_DOFToggle(bool NewValue);
    void Event_FieldOfViewFP(float NewValue);
    void Event_FieldOfView(float NewValue);
    void Event_Invert(bool NewValue);
    void Event_CameraOffsetHorizontal(float NewValue);
    void Event_CameraOffsetVertical(float NewValue);
    void Event_NoClip(bool NewValue);
    void Event_ScopeSensitivity(float NewValue);
    void Event_AimingSensitivity(float NewValue);
    void Event_Sensitivity(float NewValue);
    void Event_FlashlightShadow(bool NewValue);
    void CheckCombat();
    void CheckDistance();
    void Event_ToggleActionBar(bool NewValue);
    void BindEvent_DestroyCable();
    void BindEvent_SetFirstPersonView(bool EnterFP?);
    void Event_StopAimFirearm();
    void Event_AimFirearm();
    void Client_SetCombatCrosshair();
    void Client_SetInteractCrosshair();
    void Svr_SetADS(bool ADS);
    void MC_StopADS();
    void MC_ADS();
    void Svr_StopADS();
    void Svr_ADS();
    void Event_ResetLeaning();
    void Event_PlayerLeaning(bool Left?, bool ADS?);
    void AutoScopeZoom();
    void Event_ZoomOutSight();
    void Client_StopAllCameraShake(bool bImmediately);
    void Client_ADSCameraShake(bool Sniper?);
    void Event_HoldBreath();
    void Event_RangeFinder(bool Aiming?);
    void OnLanded(const FHitResult& Hit);
    void Event_Jump();
    void Svr_SetJumpVelocity(float Jump Z Velocity);
    void Svr_SetActorLocation(FVector NewLocation, bool bSweep, bool bTeleport);
    void Svr_SetActorRotation(FRotator NewRotation);
    void Svr_SetMovement(TEnumAsByte<EMovementMode> NewMovementMode, bool Disable?);
    void MC_SetCapsuleSize(float InRadius, float InHalfHeight, bool bUpdateOverlaps);
    void Event_TimerBinos();
    void BinosDistanceTimer(bool Stop?);
    void ExitBinos();
    void UseBinos();
    void MC_FlashlightRotation(FRotator NewRotation);
    void Svr_FlashlightRotation(FRotator NewRotation);
    void MC_Flashlight(bool FlashlightOn?);
    void Svr_Flashlight(bool FlashlightOn?);
    void Battery_Flashlight();
    void StartBattery_Flashlight();
    void AdjustThirdPersonCrouchSpringArm(bool Crouch?);
    void Event_UnCrouch();
    void Event_Crouch();
    void MC_ADSUnCrouch();
    void MC_ADSCrouch();
    void MC_UnCrouch();
    void MC_Crouch();
    void Event_ADS UnCrouch();
    void Svr_ADSCrouch();
    void Svr_UnCrouch();
    void Svr_Crouch();
    void ReceiveTick(float DeltaSeconds);
    void Event_AutoRun();
    void Svr_Distract();
    void MC_Distract();
    void Event_StopMeleeAim();
    void Event_MeleeAim();
    void MC_StopMeleeAim();
    void Svr_MeleeAim();
    void Svr_StopMeleeAim();
    void MC_MeleeAim();
    void Client_RollRotation();
    void Svr_PlayerRoll();
    void SetBurning(bool Burning, double Damage);
    void StopBurning();
    void Event_Fire();
    void Client_SUICIDE();
    void Client_HitEffects();
    void MC_PlayerDead();
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void Event_RegenerateHealth();
    void SendInfectionToClient(double NewInfection);
    void Client_UpdateInfectionUI(double Infection);
    void SetTraceToWorld(bool Set?);
    void SendOxygenToClient(double NewOxygen);
    void Client_UpdateOxygenUI(double Oxygen);
    void SmoothYawRotation(double Yaw);
    void Client_CameraShake(TSubclassOf<class ULegacyCameraShake> CameraShake);
    void MC_PlaySoundAtLocation(class USoundBase* Sound, FVector Location, float VolumeMultiplier);
    void Svr_ReduceStamina(double float);
    void MC_AttachClothing(class USkinnedMeshComponent* Clothing, class USkinnedAsset* Mesh, FBodyPartSettings Parts, bool IsPlayerMale?, FName Body Part, bool UpdateAllBodyParts?);
    void Svr_AttachClothing(class USkinnedMeshComponent* Clothing, class USkinnedAsset* Mesh, FBodyPartSettings Parts, bool IsPlayerMale?, FName Body Part, bool UpdateAllBodyParts?);
    void CreateNotificationUI(FText Text, class UTexture2D* Image, FLinearColor Color, double UI Delay);
    void MC_StopMontage(class UAnimMontage* Montage, bool IncludeLocal?);
    void Svr_StopMontage(class UAnimMontage* Montage, bool IncludeLocal?);
    void StopMontage(class UAnimMontage* Montage, bool IncludeLocal?);
    void PlayMontage(class UAnimMontage* Montage, double Play Rate);
    void MC_Montage(class UAnimMontage* Montage, float PlayRate, bool IncludeLocal?);
    void Svr_Montage(class UAnimMontage* Montage, float PlayRate, bool IncludeLocal?);
    void Event_DestroyCharacter();
    void Svr_DestroyComponent(class UActorComponent* Component);
    void PlayerDeath();
    void SendRadiationToClient(double NewRadiation);
    void SendThirstToClient(double NewThirst);
    void SendHungerToClient(double NewHunger);
    void SendHealthToClient(double NewHealth);
    void Client_UpdateStaminaUI(double Stamina);
    void Client_UpdateHealthUI(double Health);
    void Client_UpdateHungerUI(double NewHunger);
    void Client_UpdateThirstUI(double NewThirst);
    void Client_UpdateRadiationUI(double NewRadiation);
    void SendStaminaToClient(double NewStamina);
    void Svr_UpdateCombatState(int32 Blend Space);
    void MC_UpdateCombatState(int32 BlendSpace);
    void Grass();
    void MC_BloodSplatter();
    void Svr_BloodSplatter();
    void Svr_UpdateSprintSpeed(double SprintSpeed);
    void Svr_UpdateWalkSpeed(double WalkSpeed);
    void MC_UpdateWeaponSpeed();
    void Svr_UpdateWeaponSpeed();
    void MC_UpdateSpeed(double Speed);
    void Svr_UpdateSpeed(double Speed);
    void Event_StopSprint();
    void Event_Sprint();
    void MC_SwimFast();
    void Svr_SwimFast();
    void MC_CrouchSprint();
    void Svr_CrouchSprint();
    void ToggleSprint(bool State);
    void UpdatePlayerSpeed(double NewSpeed);
    void Event_LoadPlayer();
    void Event_Journal();
    void ResetCameraPosition(FRotator Rot);
    void Event_SitDownRegen();
    void Svr_WeaponShove();
    void ActorPreLoad();
    void ActorLoaded();
    void ActorPreSave();
    void Client_MapToggle();
    void CloseDebugMenu();
    void ResetVehicleCameraArmLength();
    void VehicleInteraction(bool Enter?, const class ABP_VehicleMaster_C*& VehicleRef);
    void PlayUsingItemMontage(class UJigsawItem_DataAsset_C* Asset, class UAnimMontage* Montage, class UStaticMesh* OverrideMesh);
    void ConsumeItem(FRepItemInfo Info);
    void ExecuteUbergraph_BP_PlayerCharacter(int32 EntryPoint);
    void SetFirstPersonView__DelegateSignature(bool EnterFP?);
    void PlayerDead__DelegateSignature();
    void AssignTechTree__DelegateSignature();
    void KeepInventoryOnDeath__DelegateSignature();
    void LoadInventory__DelegateSignature();
}; // Size: 0x1E40

#endif

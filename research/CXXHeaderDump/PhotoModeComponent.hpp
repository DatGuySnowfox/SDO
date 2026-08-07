#ifndef UE4SS_SDK_PhotoModeComponent_HPP
#define UE4SS_SDK_PhotoModeComponent_HPP

class UPhotoModeComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    class UPhotoModeWidget_C* PhotoModeWidgetRef;                                     // 0x00A8 (size: 0x8)
    class ACharacter* PlayerRef;                                                      // 0x00B0 (size: 0x8)
    class USpringArmComponent* SpringArmRef;                                          // 0x00B8 (size: 0x8)
    class UCameraComponent* CameraRef;                                                // 0x00C0 (size: 0x8)
    bool UsingPhotoMode?;                                                             // 0x00C8 (size: 0x1)
    bool PauseGameWhenUsingPhotoMode?;                                                // 0x00C9 (size: 0x1)
    FSlateColor MenuColor;                                                            // 0x00CC (size: 0x14)
    bool ShowMouseCursorByDefault?;                                                   // 0x00E0 (size: 0x1)
    bool ShowPhotoResolutionOption?;                                                  // 0x00E1 (size: 0x1)
    bool ShowMouseCursorOption?;                                                      // 0x00E2 (size: 0x1)
    bool FreeCameraOption?;                                                           // 0x00E3 (size: 0x1)
    double FreeCameraMaximumDistance;                                                 // 0x00E8 (size: 0x8)
    bool UseMaximumDistanceLimit;                                                     // 0x00F0 (size: 0x1)
    double FreeCameraSensitivity;                                                     // 0x00F8 (size: 0x8)
    bool IsMultiplayerGame?;                                                          // 0x0100 (size: 0x1)
    bool IsFirstPersonGame?;                                                          // 0x0101 (size: 0x1)
    bool ShowTakePictureOption?;                                                      // 0x0102 (size: 0x1)
    bool ResetControlRotationAfterClosingPM;                                          // 0x0103 (size: 0x1)
    double MaxUpDownCameraDistance;                                                   // 0x0108 (size: 0x8)
    double MaxLeftRightCameraDistance;                                                // 0x0110 (size: 0x8)
    TArray<FPhotoModeComponentGameLogo> GameLogos;                                    // 0x0118 (size: 0x10)
    TArray<FPostProcessPressets> PostProcessPresets;                                  // 0x0128 (size: 0x10)
    double DefaultFOV;                                                                // 0x0138 (size: 0x8)
    FRotator DefaultCameraRotation;                                                   // 0x0140 (size: 0x18)
    FPostProcessSettings DefaultPostProcess;                                          // 0x0160 (size: 0x6E0)
    bool UsingGallery?;                                                               // 0x0840 (size: 0x1)
    FPostProcessSettings DefaultPhotoModePostProcess;                                 // 0x0850 (size: 0x6E0)
    FPostProcessSettings MenuPhotoModePostProcess;                                    // 0x0F30 (size: 0x6E0)
    FInputKeys TakeScreenshotKeys;                                                    // 0x1610 (size: 0x30)
    FInputKeys OpenGalleryKeys;                                                       // 0x1640 (size: 0x30)
    FInputKeys HideMenuKeys;                                                          // 0x1670 (size: 0x30)
    FInputKeys ResetValuesKeys;                                                       // 0x16A0 (size: 0x30)
    FInputKeys NextTabKeys;                                                           // 0x16D0 (size: 0x30)
    FInputKeys PrevTabKeys;                                                           // 0x1700 (size: 0x30)
    FInputKeys OpenClosePhotoModeKeys;                                                // 0x1730 (size: 0x30)
    FInputKeys ClosePhotoModeWithAnotherKey;                                          // 0x1760 (size: 0x30)
    FInputKeys ShowOrHideMouseCursorKeys;                                             // 0x1790 (size: 0x30)
    FInputKeys OpenFreeCameraKeys;                                                    // 0x17C0 (size: 0x30)
    FInputKeys UpCameraKeys;                                                          // 0x17F0 (size: 0x30)
    FInputKeys DownCameraKeys;                                                        // 0x1820 (size: 0x30)
    bool ApplyMenuColor;                                                              // 0x1850 (size: 0x1)
    FPhotoModeComponentMenuIcons KeyboardIcons;                                       // 0x1858 (size: 0x60)
    FPhotoModeComponentMenuIcons Gamepad01Icons;                                      // 0x18B8 (size: 0x60)
    FPhotoModeComponentMenuIcons Gamepad02Icons;                                      // 0x1918 (size: 0x60)
    int32 GamepadIconStyle;                                                           // 0x1978 (size: 0x4)
    FKey LastKey;                                                                     // 0x1980 (size: 0x18)
    double DefaultSpringArmLength;                                                    // 0x1998 (size: 0x8)
    bool MouseCursor;                                                                 // 0x19A0 (size: 0x1)
    TArray<FPostProcessRendering> PostProcessRenders;                                 // 0x19A8 (size: 0x10)
    FVector DefaultSocketOffset;                                                      // 0x19B8 (size: 0x18)
    bool UseCustomScreenshotFolder?;                                                  // 0x19D0 (size: 0x1)
    FString CustomScreenshotFolderPath;                                               // 0x19D8 (size: 0x10)
    class APlayerController* PlayerControllerRef;                                     // 0x19E8 (size: 0x8)
    bool EnableGallery?;                                                              // 0x19F0 (size: 0x1)
    class UPhotoModeGalleryWidget_C* GalleryWidgetRef;                                // 0x19F8 (size: 0x8)
    bool UsingPostProcessVolume?;                                                     // 0x1A00 (size: 0x1)
    class APostProcessVolume* PostProcessVolume;                                      // 0x1A08 (size: 0x8)
    FPhotoModeComponentGalleryIcons KeyboardIcons (Gallery);                          // 0x1A10 (size: 0x30)
    FPhotoModeComponentGalleryIcons Gamepad01Icons (Gallery);                         // 0x1A40 (size: 0x30)
    FPhotoModeComponentGalleryIcons Gamepad02Icons (Gallery);                         // 0x1A70 (size: 0x30)
    FInputKeys NextPageKeys (Gallery);                                                // 0x1AA0 (size: 0x30)
    FInputKeys PrevPageKeys (Gallery);                                                // 0x1AD0 (size: 0x30)
    FInputKeys FullscreenKeys (Gallery);                                              // 0x1B00 (size: 0x30)
    FInputKeys BackKeys (Gallery);                                                    // 0x1B30 (size: 0x30)
    FInputKeys ShowMouseKeys (Gallery);                                               // 0x1B60 (size: 0x30)
    FPhotoModeIsOnConfis PhotoModeIsOnWarning;                                        // 0x1B90 (size: 0x68)
    FRotator DefaultControlRotation;                                                  // 0x1BF8 (size: 0x18)
    int32 PhotoModeViewportZOrder;                                                    // 0x1C10 (size: 0x4)
    bool ExistingPostProcess;                                                         // 0x1C14 (size: 0x1)
    TSubclassOf<class AActor> PostProcessVolumeClass;                                 // 0x1C18 (size: 0x8)
    bool isUnbound;                                                                   // 0x1C20 (size: 0x1)
    FPostProcessSettings OriginalPostProcess;                                         // 0x1C30 (size: 0x6E0)
    double GlobalTimeDilationBackup;                                                  // 0x2310 (size: 0x8)
    TEnumAsByte<EnumInputMode::Type> InputModeAfterClosingPhotoMode;                  // 0x2318 (size: 0x1)
    bool ShowAlbumOption?;                                                            // 0x2319 (size: 0x1)
    double CurrentTimeDilation;                                                       // 0x2320 (size: 0x8)
    bool AutoSizePerTab;                                                              // 0x2328 (size: 0x1)
    FInputKeys LeftCameraKeys;                                                        // 0x2330 (size: 0x30)
    FInputKeys RightCameraKeys;                                                       // 0x2360 (size: 0x30)
    FInputKeys OpenPhotoModeKeys (Gallery);                                           // 0x2390 (size: 0x30)
    FPhotoModeComponent_CPhotoModeOpened PhotoModeOpened;                             // 0x23C0 (size: 0x10)
    void PhotoModeOpened(double CurrentTimeDilation);
    FPhotoModeComponent_CPhotoModeClosed PhotoModeClosed;                             // 0x23D0 (size: 0x10)
    void PhotoModeClosed();
    bool ShowCharacterRotationOption?;                                                // 0x23E0 (size: 0x1)
    bool FocusWhenHoveringOverSlider;                                                 // 0x23E1 (size: 0x1)

    void UpdateOutputFolder(FString Custom Path);
    void GetCameraLocation(FVector& CameraLocation);
    void GetSliderCameraDistance(double& CameraDistance);
    void GetSliderRoll(double& Roll);
    void GetSliderFOV(double& FOV);
    void GetPostProcessVolume(bool& IsValid, class APostProcessVolume*& PostProcessVolume);
    void UnpausedGameAxisMovementControl(double AxisValueIn, double& AxisValueOut);
    void IconSelector - Gallery(FKey Key);
    void IconSelector - Photo Mode Menu(FKey Key);
    void ResetPhotoMode();
    void OpenGallery(FKey LastKey);
    void ResetGallery();
    void Initialization(class ACharacter* PlayerReference, class UCameraComponent* CameraComponentReference, class USpringArmComponent* SpringArmReference, const class APlayerController*& PlayerController);
    void Photo Mode(FKey Key);
    void ExecuteUbergraph_PhotoModeComponent(int32 EntryPoint);
    void PhotoModeClosed__DelegateSignature();
    void PhotoModeOpened__DelegateSignature(double CurrentTimeDilation);
}; // Size: 0x23E2

#endif

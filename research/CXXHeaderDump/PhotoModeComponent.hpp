#ifndef UE4SS_SDK_PhotoModeComponent_HPP
#define UE4SS_SDK_PhotoModeComponent_HPP

class UPhotoModeComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    class UPhotoModeWidget_C* PhotoModeWidgetRef;                                     // 0x00C0 (size: 0x8)
    class ACharacter* PlayerRef;                                                      // 0x00C8 (size: 0x8)
    class USpringArmComponent* SpringArmRef;                                          // 0x00D0 (size: 0x8)
    class UCameraComponent* CameraRef;                                                // 0x00D8 (size: 0x8)
    bool UsingPhotoMode?;                                                             // 0x00E0 (size: 0x1)
    bool PauseGameWhenUsingPhotoMode?;                                                // 0x00E1 (size: 0x1)
    FSlateColor MenuColor;                                                            // 0x00E4 (size: 0x14)
    bool ShowMouseCursorByDefault?;                                                   // 0x00F8 (size: 0x1)
    bool ShowPhotoResolutionOption?;                                                  // 0x00F9 (size: 0x1)
    bool ShowMouseCursorOption?;                                                      // 0x00FA (size: 0x1)
    bool FreeCameraOption?;                                                           // 0x00FB (size: 0x1)
    double FreeCameraMaximumDistance;                                                 // 0x0100 (size: 0x8)
    bool UseMaximumDistanceLimit;                                                     // 0x0108 (size: 0x1)
    double FreeCameraSensitivity;                                                     // 0x0110 (size: 0x8)
    bool IsMultiplayerGame?;                                                          // 0x0118 (size: 0x1)
    bool IsFirstPersonGame?;                                                          // 0x0119 (size: 0x1)
    bool ShowTakePictureOption?;                                                      // 0x011A (size: 0x1)
    bool ResetControlRotationAfterClosingPM;                                          // 0x011B (size: 0x1)
    double MaxUpDownCameraDistance;                                                   // 0x0120 (size: 0x8)
    double MaxLeftRightCameraDistance;                                                // 0x0128 (size: 0x8)
    TArray<FPhotoModeComponentGameLogo> GameLogos;                                    // 0x0130 (size: 0x10)
    TArray<FPostProcessPressets> PostProcessPresets;                                  // 0x0140 (size: 0x10)
    double DefaultFOV;                                                                // 0x0150 (size: 0x8)
    FRotator DefaultCameraRotation;                                                   // 0x0158 (size: 0x18)
    FPostProcessSettings DefaultPostProcess;                                          // 0x0170 (size: 0x760)
    bool UsingGallery?;                                                               // 0x08D0 (size: 0x1)
    FPostProcessSettings DefaultPhotoModePostProcess;                                 // 0x08E0 (size: 0x760)
    FPostProcessSettings MenuPhotoModePostProcess;                                    // 0x1040 (size: 0x760)
    FInputKeys TakeScreenshotKeys;                                                    // 0x17A0 (size: 0x30)
    FInputKeys OpenGalleryKeys;                                                       // 0x17D0 (size: 0x30)
    FInputKeys HideMenuKeys;                                                          // 0x1800 (size: 0x30)
    FInputKeys ResetValuesKeys;                                                       // 0x1830 (size: 0x30)
    FInputKeys NextTabKeys;                                                           // 0x1860 (size: 0x30)
    FInputKeys PrevTabKeys;                                                           // 0x1890 (size: 0x30)
    FInputKeys OpenClosePhotoModeKeys;                                                // 0x18C0 (size: 0x30)
    FInputKeys ClosePhotoModeWithAnotherKey;                                          // 0x18F0 (size: 0x30)
    FInputKeys ShowOrHideMouseCursorKeys;                                             // 0x1920 (size: 0x30)
    FInputKeys OpenFreeCameraKeys;                                                    // 0x1950 (size: 0x30)
    FInputKeys UpCameraKeys;                                                          // 0x1980 (size: 0x30)
    FInputKeys DownCameraKeys;                                                        // 0x19B0 (size: 0x30)
    bool ApplyMenuColor;                                                              // 0x19E0 (size: 0x1)
    FPhotoModeComponentMenuIcons KeyboardIcons;                                       // 0x19E8 (size: 0x60)
    FPhotoModeComponentMenuIcons Gamepad01Icons;                                      // 0x1A48 (size: 0x60)
    FPhotoModeComponentMenuIcons Gamepad02Icons;                                      // 0x1AA8 (size: 0x60)
    int32 GamepadIconStyle;                                                           // 0x1B08 (size: 0x4)
    FKey LastKey;                                                                     // 0x1B10 (size: 0x18)
    double DefaultSpringArmLength;                                                    // 0x1B28 (size: 0x8)
    bool MouseCursor;                                                                 // 0x1B30 (size: 0x1)
    TArray<FPostProcessRendering> PostProcessRenders;                                 // 0x1B38 (size: 0x10)
    FVector DefaultSocketOffset;                                                      // 0x1B48 (size: 0x18)
    bool UseCustomScreenshotFolder?;                                                  // 0x1B60 (size: 0x1)
    FString CustomScreenshotFolderPath;                                               // 0x1B68 (size: 0x10)
    class APlayerController* PlayerControllerRef;                                     // 0x1B78 (size: 0x8)
    bool EnableGallery?;                                                              // 0x1B80 (size: 0x1)
    class UPhotoModeGalleryWidget_C* GalleryWidgetRef;                                // 0x1B88 (size: 0x8)
    bool UsingPostProcessVolume?;                                                     // 0x1B90 (size: 0x1)
    class APostProcessVolume* PostProcessVolume;                                      // 0x1B98 (size: 0x8)
    FPhotoModeComponentGalleryIcons KeyboardIcons (Gallery);                          // 0x1BA0 (size: 0x30)
    FPhotoModeComponentGalleryIcons Gamepad01Icons (Gallery);                         // 0x1BD0 (size: 0x30)
    FPhotoModeComponentGalleryIcons Gamepad02Icons (Gallery);                         // 0x1C00 (size: 0x30)
    FInputKeys NextPageKeys (Gallery);                                                // 0x1C30 (size: 0x30)
    FInputKeys PrevPageKeys (Gallery);                                                // 0x1C60 (size: 0x30)
    FInputKeys FullscreenKeys (Gallery);                                              // 0x1C90 (size: 0x30)
    FInputKeys BackKeys (Gallery);                                                    // 0x1CC0 (size: 0x30)
    FInputKeys ShowMouseKeys (Gallery);                                               // 0x1CF0 (size: 0x30)
    FPhotoModeIsOnConfis PhotoModeIsOnWarning;                                        // 0x1D20 (size: 0x60)
    FRotator DefaultControlRotation;                                                  // 0x1D80 (size: 0x18)
    int32 PhotoModeViewportZOrder;                                                    // 0x1D98 (size: 0x4)
    bool ExistingPostProcess;                                                         // 0x1D9C (size: 0x1)
    TSubclassOf<class AActor> PostProcessVolumeClass;                                 // 0x1DA0 (size: 0x8)
    bool isUnbound;                                                                   // 0x1DA8 (size: 0x1)
    FPostProcessSettings OriginalPostProcess;                                         // 0x1DB0 (size: 0x760)
    double GlobalTimeDilationBackup;                                                  // 0x2510 (size: 0x8)
    TEnumAsByte<EnumInputMode::Type> InputModeAfterClosingPhotoMode;                  // 0x2518 (size: 0x1)
    bool ShowAlbumOption?;                                                            // 0x2519 (size: 0x1)
    double CurrentTimeDilation;                                                       // 0x2520 (size: 0x8)
    bool AutoSizePerTab;                                                              // 0x2528 (size: 0x1)
    FInputKeys LeftCameraKeys;                                                        // 0x2530 (size: 0x30)
    FInputKeys RightCameraKeys;                                                       // 0x2560 (size: 0x30)
    FInputKeys OpenPhotoModeKeys (Gallery);                                           // 0x2590 (size: 0x30)
    FPhotoModeComponent_CPhotoModeOpened PhotoModeOpened;                             // 0x25C0 (size: 0x10)
    void PhotoModeOpened(double CurrentTimeDilation);
    FPhotoModeComponent_CPhotoModeClosed PhotoModeClosed;                             // 0x25D0 (size: 0x10)
    void PhotoModeClosed();
    bool ShowCharacterRotationOption?;                                                // 0x25E0 (size: 0x1)
    bool FocusWhenHoveringOverSlider;                                                 // 0x25E1 (size: 0x1)

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
}; // Size: 0x25E2

#endif

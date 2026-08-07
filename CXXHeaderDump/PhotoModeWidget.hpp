#ifndef UE4SS_SDK_PhotoModeWidget_HPP
#define UE4SS_SDK_PhotoModeWidget_HPP

class UPhotoModeWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* OpenPhotoMode;                                            // 0x02C8 (size: 0x8)
    class UWidgetAnimation* NotificationScreenshotTaken;                              // 0x02D0 (size: 0x8)
    class USelectorSlider_C* Aperture;                                                // 0x02D8 (size: 0x8)
    class USizeBox* APM_SizeMenu;                                                     // 0x02E0 (size: 0x8)
    class UCommonCheckbox_C* Autofocus;                                               // 0x02E8 (size: 0x8)
    class UCommonSlider_C* BlueCorrection;                                            // 0x02F0 (size: 0x8)
    class UBorder* BorderScreenshotTaken;                                             // 0x02F8 (size: 0x8)
    class UCommonSlider_C* Brightness;                                                // 0x0300 (size: 0x8)
    class UCommonButtonTab_C* btn_CameraTab;                                          // 0x0308 (size: 0x8)
    class UCommonButtonTab_C* btn_ColorBalanceTab;                                    // 0x0310 (size: 0x8)
    class UCommonButtonTab_C* btn_DepthOfFieldTab;                                    // 0x0318 (size: 0x8)
    class UCommonButtonTab_C* btn_DisplayTab;                                         // 0x0320 (size: 0x8)
    class UCommonButtonTab_C* btn_FiltersTab;                                         // 0x0328 (size: 0x8)
    class UCommonButtonTab_C* btn_FramesTab;                                          // 0x0330 (size: 0x8)
    class UCommonButtonTab_C* btn_LogoTab;                                            // 0x0338 (size: 0x8)
    class UCommonButtonTab_C* btn_ScreenEffectsTab;                                   // 0x0340 (size: 0x8)
    class UButton* btnDown;                                                           // 0x0348 (size: 0x8)
    class UButton* btnLeft;                                                           // 0x0350 (size: 0x8)
    class UButton* btnRight;                                                          // 0x0358 (size: 0x8)
    class UButton* btnSwap;                                                           // 0x0360 (size: 0x8)
    class UButton* btnUp;                                                             // 0x0368 (size: 0x8)
    class UBorder* Buttons-Border;                                                    // 0x0370 (size: 0x8)
    class UCommonSlider_C* CameraDistance;                                            // 0x0378 (size: 0x8)
    class UCommonSlider_C* CameraRoll;                                                // 0x0380 (size: 0x8)
    class UCanvasPanel* CanvasScreenshotTaken;                                        // 0x0388 (size: 0x8)
    class UImage* CaptureMouseArea;                                                   // 0x0390 (size: 0x8)
    class UCommonCheckbox_C* CharacterRotation;                                       // 0x0398 (size: 0x8)
    class UCommonSlider_C* ChromaticAberrationIntensity;                              // 0x03A0 (size: 0x8)
    class UCommonSlider_C* ChromaticAberrationOffset;                                 // 0x03A8 (size: 0x8)
    class UCommonSlider_C* ColorBalanceBlue;                                          // 0x03B0 (size: 0x8)
    class UCommonSlider_C* ColorBalanceGreen;                                         // 0x03B8 (size: 0x8)
    class UCommonSlider_C* ColorBalanceIntensity;                                     // 0x03C0 (size: 0x8)
    class UCommonSlider_C* ColorBalanceRed;                                           // 0x03C8 (size: 0x8)
    class USelectorSlider_C* ColorBalanceSelector;                                    // 0x03D0 (size: 0x8)
    class UCommandButton_C* Command_Album;                                            // 0x03D8 (size: 0x8)
    class UCommandButton_C* Command_ChangeTab;                                        // 0x03E0 (size: 0x8)
    class UCommandButton_C* Command_Close;                                            // 0x03E8 (size: 0x8)
    class UCommandButton_C* Command_FreeCamera;                                       // 0x03F0 (size: 0x8)
    class UCommandButton_C* Command_HideUI;                                           // 0x03F8 (size: 0x8)
    class UCommandButton_C* Command_Mouse;                                            // 0x0400 (size: 0x8)
    class UCommandButton_C* Command_Reset;                                            // 0x0408 (size: 0x8)
    class UCommandButton_C* Command_TakePicture;                                      // 0x0410 (size: 0x8)
    class UImage* Divisor;                                                            // 0x0418 (size: 0x8)
    class UImage* Down;                                                               // 0x0420 (size: 0x8)
    class UCommonSlider_C* FieldOfView;                                               // 0x0428 (size: 0x8)
    class UCommonSlider_C* FilmGrain;                                                 // 0x0430 (size: 0x8)
    class USelectorSlider_C* Filter;                                                  // 0x0438 (size: 0x8)
    class UCommonSlider_C* FocusDistance;                                             // 0x0440 (size: 0x8)
    class UTextBlock* FocusDistanceTxt;                                               // 0x0448 (size: 0x8)
    class UCommonSlider_C* FrameBlue;                                                 // 0x0450 (size: 0x8)
    class UCommonSlider_C* FrameGreen;                                                // 0x0458 (size: 0x8)
    class UCommonSlider_C* FrameRed;                                                  // 0x0460 (size: 0x8)
    class USelectorSlider_C* FrameSelector;                                           // 0x0468 (size: 0x8)
    class UCommonSlider_C* FrameSize;                                                 // 0x0470 (size: 0x8)
    class UCommonCheckbox_C* Grid;                                                    // 0x0478 (size: 0x8)
    class UCanvasPanel* GRIDAutofocus;                                                // 0x0480 (size: 0x8)
    class UCanvasPanel* GRIDFocusDistanceText;                                        // 0x0488 (size: 0x8)
    class UCommonCheckbox_C* HideCharacter;                                           // 0x0490 (size: 0x8)
    class UImage* img_PhotoModeIsOn;                                                  // 0x0498 (size: 0x8)
    class UBorder* Info-Border;                                                       // 0x04A0 (size: 0x8)
    class UWidgetSwitcher* InfoSwitcher;                                              // 0x04A8 (size: 0x8)
    class UImage* Left;                                                               // 0x04B0 (size: 0x8)
    class UCommonSlider_C* LogoAngle;                                                 // 0x04B8 (size: 0x8)
    class UCommonSlider_C* LogoHorizontalAlignment;                                   // 0x04C0 (size: 0x8)
    class USelectorSlider_C* LogoSelector;                                            // 0x04C8 (size: 0x8)
    class UCommonSlider_C* LogoSize;                                                  // 0x04D0 (size: 0x8)
    class UCommonSlider_C* LogoVerticalAlignment;                                     // 0x04D8 (size: 0x8)
    class UCommonSlider_C* LUTIntensity;                                              // 0x04E0 (size: 0x8)
    class UCommonSlider_C* MotionBlur;                                                // 0x04E8 (size: 0x8)
    class UCanvasPanel* MoveCameraKeys-CanvasPanel;                                   // 0x04F0 (size: 0x8)
    class UOverlay* Overlay_1;                                                        // 0x04F8 (size: 0x8)
    class UOverlay* Overlay_2;                                                        // 0x0500 (size: 0x8)
    class UOverlay* Overlay_3;                                                        // 0x0508 (size: 0x8)
    class UOverlay* Overlay_4;                                                        // 0x0510 (size: 0x8)
    class UCanvasPanel* PhotoModeIsOnHidedMenu;                                       // 0x0518 (size: 0x8)
    class USelectorSlider_C* PhotoResolution;                                         // 0x0520 (size: 0x8)
    class UImage* PreviewPhoto;                                                       // 0x0528 (size: 0x8)
    class USelectorSlider_C* Rendering;                                               // 0x0530 (size: 0x8)
    class UImage* Right;                                                              // 0x0538 (size: 0x8)
    class UCommonSlider_C* SensorWidth;                                               // 0x0540 (size: 0x8)
    class UImage* Swap;                                                               // 0x0548 (size: 0x8)
    class UHorizontalBox* Swap-Gamepad-HorizontalBox;                                 // 0x0550 (size: 0x8)
    class UCommonSlider_C* Temperature;                                               // 0x0558 (size: 0x8)
    class UTextBlock* TextBlock_192;                                                  // 0x0560 (size: 0x8)
    class UTextBlock* TextPhotoModeIsOn;                                              // 0x0568 (size: 0x8)
    class UCommonSlider_C* Tint;                                                      // 0x0570 (size: 0x8)
    class UImage* Up;                                                                 // 0x0578 (size: 0x8)
    class UVerticalBox* VerticalBox_381;                                              // 0x0580 (size: 0x8)
    class UCommonSlider_C* Vignette;                                                  // 0x0588 (size: 0x8)
    class UWidgetSwitcher* WidgetSwitcherPM;                                          // 0x0590 (size: 0x8)
    TEnumAsByte<EnumIcons::Type> IconType;                                            // 0x0598 (size: 0x1)
    TEnumAsByte<EnumIcons::Type> GamepadIconStyle;                                    // 0x0599 (size: 0x1)
    class UWidget* CurrentGenericWidgetElement;                                       // 0x05A0 (size: 0x8)
    int32 CurrentTab;                                                                 // 0x05A8 (size: 0x4)
    int32 LastTabIndex;                                                               // 0x05AC (size: 0x4)
    bool UsingFreeCamera?;                                                            // 0x05B0 (size: 0x1)
    class APawn* FreeCameraPawn;                                                      // 0x05B8 (size: 0x8)
    class APhotoModeFreeCamera_C* FreeCameraReference;                                // 0x05C0 (size: 0x8)
    class UCameraComponent* FreeCameraComponent;                                      // 0x05C8 (size: 0x8)
    FPostProcessSettings EditedPostProcessSettings;                                   // 0x05D0 (size: 0x6E0)
    FPhotoModeWidget_CED_FreeCameraReference ED_FreeCameraReference;                  // 0x0CB0 (size: 0x10)
    void ED_FreeCameraReference(FPostProcessSettings PostProcessValues);
    FRotator EditedCameraRoll;                                                        // 0x0CC0 (size: 0x18)
    double EditedCameraFOV;                                                           // 0x0CD8 (size: 0x8)
    FPhotoModeWidget_CED_FreeCameraFOV ED_FreeCameraFOV;                              // 0x0CE0 (size: 0x10)
    void ED_FreeCameraFOV(double FOVValue);
    FPhotoModeWidget_CED_FreeCameraRotation ED_FreeCameraRotation;                    // 0x0CF0 (size: 0x10)
    void ED_FreeCameraRotation(FRotator RotationValue);
    bool ShowingMouseCursor;                                                          // 0x0D00 (size: 0x1)
    bool MenuHidden;                                                                  // 0x0D01 (size: 0x1)
    bool CanFocus?;                                                                   // 0x0D02 (size: 0x1)
    class UPhotoModeGalleryWidget_C* GalleryReference;                                // 0x0D08 (size: 0x8)
    class UPhotoModeComponent_C* PhotoModeComponentReference;                         // 0x0D10 (size: 0x8)
    bool RunPhysicsWithNoController?;                                                 // 0x0D18 (size: 0x1)
    bool ShowPhotoModeIsOnWarning?;                                                   // 0x0D19 (size: 0x1)
    bool TakingPhoto;                                                                 // 0x0D1A (size: 0x1)
    TArray<class UCommonSlider_C*> CommonSliders;                                     // 0x0D20 (size: 0x10)
    bool ResetSucess;                                                                 // 0x0D30 (size: 0x1)
    TArray<FText> FiltersOptions;                                                     // 0x0D38 (size: 0x10)
    TArray<class USelectorSlider_C*> SelectorSliders;                                 // 0x0D48 (size: 0x10)
    TArray<class UCommonCheckbox_C*> CommonCheckboxes;                                // 0x0D58 (size: 0x10)
    bool UsingAutofocus;                                                              // 0x0D68 (size: 0x1)
    TEnumAsByte<ECollisionResponse> CollisionChannel;                                 // 0x0D69 (size: 0x1)
    FWeightedBlendable HorizontalFrame;                                               // 0x0D70 (size: 0x10)
    FWeightedBlendable VerticalFrame;                                                 // 0x0D80 (size: 0x10)
    FWeightedBlendable FrameToAdd;                                                    // 0x0D90 (size: 0x10)
    bool CanTakePhoto;                                                                // 0x0DA0 (size: 0x1)
    class UMaterialInstanceDynamic* LogoDynamicMaterial;                              // 0x0DA8 (size: 0x8)
    FWeightedBlendable CurrentLogo;                                                   // 0x0DB0 (size: 0x10)
    TArray<FText> LogosNames;                                                         // 0x0DC0 (size: 0x10)
    FIntPoint ScreenshotResolution;                                                   // 0x0DD0 (size: 0x8)
    TArray<FText> PostProcessMaterialsEffects;                                        // 0x0DD8 (size: 0x10)
    FWeightedBlendable PostProcessMaterialToAdd;                                      // 0x0DE8 (size: 0x10)
    bool HighResFunction;                                                             // 0x0DF8 (size: 0x1)
    TArray<class UCommonButtonTab_C*> CommonButtons;                                  // 0x0E00 (size: 0x10)
    bool ShowGrid?;                                                                   // 0x0E10 (size: 0x1)
    bool WithUI;                                                                      // 0x0E11 (size: 0x1)
    FVector PlayerVelocity;                                                           // 0x0E18 (size: 0x18)
    TEnumAsByte<EMovementMode> PlayerMovementMode;                                    // 0x0E30 (size: 0x1)
    uint8 PlayerCustomMovementMode;                                                   // 0x0E31 (size: 0x1)
    FIntPoint DefaultUserScreenResolution;                                            // 0x0E34 (size: 0x8)
    bool KeyboardInput;                                                               // 0x0E3C (size: 0x1)
    bool GamepadInput;                                                                // 0x0E3D (size: 0x1)
    bool UpDownIsNowLeftRight;                                                        // 0x0E3E (size: 0x1)
    FRotator LastControlRotation;                                                     // 0x0E40 (size: 0x18)
    bool CanUpdateMouseWheel;                                                         // 0x0E58 (size: 0x1)
    FVector CameraLocation;                                                           // 0x0E60 (size: 0x18)
    double CameraDistanceTAL;                                                         // 0x0E78 (size: 0x8)
    double FOV;                                                                       // 0x0E80 (size: 0x8)
    FRotator Roll;                                                                    // 0x0E88 (size: 0x18)
    FKey LastKeyPressed;                                                              // 0x0EA0 (size: 0x18)
    double CameraMoveDelta;                                                           // 0x0EB8 (size: 0x8)
    FString LastScreenshotPath;                                                       // 0x0EC0 (size: 0x10)
    FString LastScreenshotName;                                                       // 0x0ED0 (size: 0x10)
    FString LastThumbnailPath;                                                        // 0x0EE0 (size: 0x10)
    FString LastThumbnailName;                                                        // 0x0EF0 (size: 0x10)
    bool TurnToCamera;                                                                // 0x0F00 (size: 0x1)
    FRotator PlayerRotationBackup;                                                    // 0x0F08 (size: 0x18)

    ESlateVisibility ShowHideCharacterRotation();
    FText getScreenshotTitle();
    double getAdjustedWorldDeltaSeconds();
    FEventReply OnKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    ESlateVisibility HideMenu();
    ESlateVisibility UpDownCameraGamepadVisibility();
    ESlateVisibility LeftRightCameraGamepadVisibility();
    ESlateVisibility SwapOptionGamepadVisibility();
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    ESlateVisibility FocusDistanceVisibility();
    FEventReply OnMouseWheel(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void RemoveMaterialFromCamera(const TArray<FWeightedBlendable>& TargetArray, const FWeightedBlendable& Item);
    ESlateVisibility GridVisibility();
    FText GetFocusDistance();
    ESlateVisibility ShowHidePhotoModeIsOnWarning();
    ESlateVisibility OpenGalleryButtonVisibility();
    ESlateVisibility TakePictureButtonVisibility();
    ESlateVisibility Up Down Left Right Buttons Visibility();
    ESlateVisibility MoveButtonVisibility();
    ESlateVisibility ShowHideMouseButtonVisibility();
    ESlateVisibility FreeCameraButtonVisibility();
    FText ShowHideMouseTxtButton();
    FText CameraModeTxt();
    void ActiveTab(int32 TabIndex, class UCommonButtonTab_C* ButtonToNotModify);
    void SetMenuIcons(FPhotoModeComponentMenuIcons Icons);
    void SendPhotoData(FPhotosData PhotoData);
    void PreConstruct(bool IsDesignTime);
    void Construct();
    void Initialization();
    void GetAnyKey(FKey Key);
    void ChangeTab(int32 TabIndex);
    void Focus(const class UWidget*& WidgetItem);
    void OnRemovedFromFocusPath(FFocusEvent InFocusEvent);
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void Dispatchers();
    void OnDestruct();
    void InterpolationUpdater(double DeltaTime);
    void ChangeFreeCameraRotation(FRotator RotationValue);
    void ChangeFreeCameraFOV(double FOVValue);
    void ChangeFreeCameraPP(FPostProcessSettings PostProcessValues);
    void BndEvt__Command_ChangeTab_K2Node_ComponentBoundEvent_0_OnPressed__DelegateSignature();
    void SetFocusBasedOnTab(int32 Index);
    void TabToOpen(int32 TabIndex, class UCommonButtonTab_C* CommonButtonPressed);
    void BndEvt__Command_Mouse_K2Node_ComponentBoundEvent_3_OnPressed__DelegateSignature();
    void ShowMouse();
    void ResetShowMouseCursor();
    void BndEvt__Command_HideUI_K2Node_ComponentBoundEvent_6_OnPressed__DelegateSignature();
    void BndEvt__Command_FreeCamera_K2Node_ComponentBoundEvent_4_OnPressed__DelegateSignature();
    void CloseFreeCamera();
    void UnblockInputMenu();
    void BlockInputMenu();
    void BndEvt__Command_Close_K2Node_ComponentBoundEvent_7_OnPressed__DelegateSignature();
    void BndEvt__Command_Reset_K2Node_ComponentBoundEvent_5_OnPressed__DelegateSignature();
    void BndEvt__ColorBalanceSelector_K2Node_ComponentBoundEvent_0_ResetDispatcher__DelegateSignature();
    void BndEvt__btnDown_K2Node_ComponentBoundEvent_15_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__btnUp_K2Node_ComponentBoundEvent_14_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__btnUp_1_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__btnDown_1_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void MakeScreenshot();
    void Register Photo(FString Save New Photo Title, FString Save New Photo Photo Full Path, FString Save New Photo Thumbnail Full Path);
    void BndEvt__CommandButton_K2Node_ComponentBoundEvent_0_OnPressed__DelegateSignature();
    void BlockScreenshot();
    void Screenshot();
    void BndEvt__Command_Album_K2Node_ComponentBoundEvent_1_OnPressed__DelegateSignature();
    void ResetKeyboardIcons();
    void BndEvt__FieldOfView_K2Node_ComponentBoundEvent_3_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__CameraRoll_K2Node_ComponentBoundEvent_4_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__CommonCheckbox_K2Node_ComponentBoundEvent_3_CheckboxDispatcher__DelegateSignature(bool Value);
    void BndEvt__Brightness_K2Node_ComponentBoundEvent_5_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__Temperature_K2Node_ComponentBoundEvent_6_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__Tint_K2Node_ComponentBoundEvent_7_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__Bloom_K2Node_ComponentBoundEvent_2_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__ChromaticAberrationIntensity_K2Node_ComponentBoundEvent_8_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__ChromaticAberrationOffset_K2Node_ComponentBoundEvent_9_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__FilmGrain_K2Node_ComponentBoundEvent_10_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__MotionBlur_K2Node_ComponentBoundEvent_11_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__Vignette_K2Node_ComponentBoundEvent_12_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__FocusDistance_K2Node_ComponentBoundEvent_13_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__PhotoAperture_K2Node_ComponentBoundEvent_0_Dispatcher__DelegateSignature(double Value);
    void BndEvt__SensorWidth_K2Node_ComponentBoundEvent_15_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__Autofocus_K2Node_ComponentBoundEvent_19_CheckboxDispatcher__DelegateSignature(bool Value);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void BndEvt__ColorBalanceSelector_K2Node_ComponentBoundEvent_21_Dispatcher__DelegateSignature(double Value);
    void BndEvt__ColorBalanceRed_K2Node_ComponentBoundEvent_22_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__ColorBalanceGreen_K2Node_ComponentBoundEvent_23_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__ColorBalanceBlue_K2Node_ComponentBoundEvent_24_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__ColorBalanceIntensity_K2Node_ComponentBoundEvent_25_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__FrameSize_K2Node_ComponentBoundEvent_4_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__FrameRed_K2Node_ComponentBoundEvent_3_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__FrameGreen_K2Node_ComponentBoundEvent_2_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__FrameBlue_K2Node_ComponentBoundEvent_1_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__FrameSelector_K2Node_ComponentBoundEvent_0_Dispatcher__DelegateSignature(double Value);
    void BndEvt__LogoSize_1_K2Node_ComponentBoundEvent_0_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__LogoSize_K2Node_ComponentBoundEvent_3_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__CommonSlider_1_K2Node_ComponentBoundEvent_1_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__CommonSlider_K2Node_ComponentBoundEvent_0_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__Logo_K2Node_ComponentBoundEvent_2_Dispatcher__DelegateSignature(double Value);
    void BndEvt__Filter_K2Node_ComponentBoundEvent_17_Dispatcher__DelegateSignature(double Value);
    void BndEvt__LUTIntensity_K2Node_ComponentBoundEvent_18_SliderDispatcher__DelegateSignature(double SliderValue);
    void BndEvt__SelectorSlider_K2Node_ComponentBoundEvent_0_Dispatcher__DelegateSignature(double Value);
    void CameraDistanceMouseWheel(double Delta);
    void BndEvt__CameraDistance_K2Node_ComponentBoundEvent_16_SliderDispatcher__DelegateSignature(double SliderValue);
    void CameraLeftRight(double Value);
    void CameraUpDown(double Value);
    void BndEvt__TurnPlayerToCamera_K2Node_ComponentBoundEvent_0_CheckboxDispatcher__DelegateSignature(bool Value);
    void BndEvt__HidePlayer_K2Node_ComponentBoundEvent_20_CheckboxDispatcher__DelegateSignature(bool Value);
    void BndEvt__PhotoResolution_K2Node_ComponentBoundEvent_0_Dispatcher__DelegateSignature(double Value);
    void ExecuteUbergraph_PhotoModeWidget(int32 EntryPoint);
    void ED_FreeCameraRotation__DelegateSignature(FRotator RotationValue);
    void ED_FreeCameraFOV__DelegateSignature(double FOVValue);
    void ED_FreeCameraReference__DelegateSignature(FPostProcessSettings PostProcessValues);
}; // Size: 0xF20

#endif

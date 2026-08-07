#ifndef UE4SS_SDK_PhotoModeGalleryWidget_HPP
#define UE4SS_SDK_PhotoModeGalleryWidget_HPP

class UPhotoModeGalleryWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* BackgroundNotification;                                   // 0x02C8 (size: 0x8)
    class UWidgetAnimation* OpeningNotification;                                      // 0x02D0 (size: 0x8)
    class UCanvasPanel* Album-CanvasPanel;                                            // 0x02D8 (size: 0x8)
    class UImage* Background_0;                                                       // 0x02E0 (size: 0x8)
    class UImage* Background_1;                                                       // 0x02E8 (size: 0x8)
    class UImage* Background_2;                                                       // 0x02F0 (size: 0x8)
    class UBackgroundBlur* BackgroundBlur;                                            // 0x02F8 (size: 0x8)
    class UBorder* BlackBackground;                                                   // 0x0300 (size: 0x8)
    class UButton* btnFullscreen;                                                     // 0x0308 (size: 0x8)
    class UButton* btnNextGalleryPage;                                                // 0x0310 (size: 0x8)
    class UButton* btnPrevGalleryPage;                                                // 0x0318 (size: 0x8)
    class UBorder* Buttons-Border_2;                                                  // 0x0320 (size: 0x8)
    class UBorder* Buttons-Bottom;                                                    // 0x0328 (size: 0x8)
    class UBorder* Buttons-Top;                                                       // 0x0330 (size: 0x8)
    class UCanvasPanel* ButtonsPanel;                                                 // 0x0338 (size: 0x8)
    class UCommandButton_C* Command_BackFullScreen;                                   // 0x0340 (size: 0x8)
    class UCommandButton_C* Command_Close;                                            // 0x0348 (size: 0x8)
    class UCommandButton_C* Command_Delete;                                           // 0x0350 (size: 0x8)
    class UCommandButton_C* Command_PhotoMode;                                        // 0x0358 (size: 0x8)
    class UCommandButton_C* Command_Select;                                           // 0x0360 (size: 0x8)
    class UCommandButton_C* Command_ShowHideMouseFullScreen;                          // 0x0368 (size: 0x8)
    class UCommandButton_C* Command_ShowMouse;                                        // 0x0370 (size: 0x8)
    class UCommandButton_C* CommandButton_OpenPhotoModeFullscreen;                    // 0x0378 (size: 0x8)
    class UImage* FadeImage;                                                          // 0x0380 (size: 0x8)
    class UCanvasPanel* Fullscreen-Panel;                                             // 0x0388 (size: 0x8)
    class UImage* FullscreenImage;                                                    // 0x0390 (size: 0x8)
    class UCanvasPanel* Gallery-Panel;                                                // 0x0398 (size: 0x8)
    class UImage* Image_19;                                                           // 0x03A0 (size: 0x8)
    class UWidgetSwitcher* MainPanelSwitcher;                                         // 0x03A8 (size: 0x8)
    class UImage* NextFullScreen;                                                     // 0x03B0 (size: 0x8)
    class UHorizontalBox* NextPage-HorizontalBox;                                     // 0x03B8 (size: 0x8)
    class UButton* NextPageButton;                                                    // 0x03C0 (size: 0x8)
    class UButton* NextPageFullscreen;                                                // 0x03C8 (size: 0x8)
    class UImage* NextPageIcon;                                                       // 0x03D0 (size: 0x8)
    class UHorizontalBox* Pages-HorizontalBox;                                        // 0x03D8 (size: 0x8)
    class UScrollBox* PagesScrollBox;                                                 // 0x03E0 (size: 0x8)
    class UImage* PreviousFullscreen;                                                 // 0x03E8 (size: 0x8)
    class UHorizontalBox* PreviousPage-HorizontalBox;                                 // 0x03F0 (size: 0x8)
    class UButton* PreviousPageButton;                                                // 0x03F8 (size: 0x8)
    class UButton* PrevPageFullscreen;                                                // 0x0400 (size: 0x8)
    class UImage* PrevPageIcon;                                                       // 0x0408 (size: 0x8)
    class USpacer* Spacer_11;                                                         // 0x0410 (size: 0x8)
    class USpacer* Spacer_13;                                                         // 0x0418 (size: 0x8)
    class USpacer* Spacer_14;                                                         // 0x0420 (size: 0x8)
    class UTextBlock* TextBlock_1;                                                    // 0x0428 (size: 0x8)
    class UVerticalBox* VerticalBox;                                                  // 0x0430 (size: 0x8)
    class UVerticalBox* VerticalBox_241;                                              // 0x0438 (size: 0x8)
    bool ShowingMouse?;                                                               // 0x0440 (size: 0x1)
    TEnumAsByte<EnumIcons::Type> IconsType;                                           // 0x0441 (size: 0x1)
    TEnumAsByte<EnumIcons::Type> GamepadIconStyle;                                    // 0x0442 (size: 0x1)
    bool ShowMouseCursorOption?;                                                      // 0x0443 (size: 0x1)
    bool inFullscreen?;                                                               // 0x0444 (size: 0x1)
    class UPhotoModeComponent_C* PhotoModeComponent;                                  // 0x0448 (size: 0x8)
    class UWidget* LastValidFocusedElement;                                           // 0x0450 (size: 0x8)
    TArray<FPhotosData> FullPhotosData;                                               // 0x0458 (size: 0x10)
    class UTexture2D* Fullscreen2DTexture;                                            // 0x0468 (size: 0x8)
    TArray<class UCommonPage_C*> Pages;                                               // 0x0470 (size: 0x10)
    TArray<class UCommonPhotoThumb_C*> PhotosThumbs;                                  // 0x0480 (size: 0x10)
    int32 PhotosPerPage;                                                              // 0x0490 (size: 0x4)
    int32 CurrentPage;                                                                // 0x0494 (size: 0x4)
    int32 PageOffsetToLoad;                                                           // 0x0498 (size: 0x4)
    int32 TotalPhotos;                                                                // 0x049C (size: 0x4)
    bool CanFreeMemory?;                                                              // 0x04A0 (size: 0x1)
    FPhotosData CurrentDataInFullscreen;                                              // 0x04A8 (size: 0x30)
    class UWidget* LastValidThumb;                                                    // 0x04D8 (size: 0x8)
    bool GamepadInput;                                                                // 0x04E0 (size: 0x1)
    bool KeyboardInput;                                                               // 0x04E1 (size: 0x1)
    bool CanDelete;                                                                   // 0x04E2 (size: 0x1)
    bool DeleteFunction;                                                              // 0x04E3 (size: 0x1)

    FEventReply OnKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    ESlateVisibility ShowHideDeleteOption();
    void DeletePhoto();
    ESlateVisibility SwapUpDownLeftRightVisibility();
    ESlateVisibility Previous Page Button Visibility();
    void CreatePages();
    FEventReply OnMouseWheel(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void UpdatePhotosData();
    void CloseFullscreen();
    ESlateVisibility ShowHideMouseVisibility();
    void SetIcons(FPhotoModeComponentGalleryIcons Icons);
    FText ShowHideMouseTxt();
    ESlateVisibility Next Page Button Visibility();
    void TabToOpen(int32 TabIndex, class UCommonButtonTab_C* CommonButtonPressed);
    void GalleryInitialization(bool EnableGallery?, class UPhotoModeComponent_C* PhotoModeComponentReference, bool ShowMouseCursorOption?, int32 DefaultGamepadIconsType);
    void Get Any Key(const FKey Key);
    void OnRemovedFromFocusPath(FFocusEvent InFocusEvent);
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void Construct();
    void PreConstruct(bool IsDesignTime);
    void Destruct();
    void SendPhotoData(FPhotosData PhotoData);
    void NextPage(bool NextFocusOnTheFirstPhoto);
    void PreviousPage(bool PrevFocusOnTheFirstPhoto);
    void LoadNextPrevPage(int32 NewPageIndex, bool PrevFocusOnTheFirstPhoto);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ResetKeyboardIcons();
    void BndEvt__NextPageFullscreen_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PrevGalleryPage_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PrevPageFullscreen_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__btnNextGalleryPage_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__Command_BackFullScreen_K2Node_ComponentBoundEvent_4_OnPressed__DelegateSignature();
    void BndEvt__Command_Close_K2Node_ComponentBoundEvent_1_OnPressed__DelegateSignature();
    void BndEvt__Command_ShowMouse_K2Node_ComponentBoundEvent_2_OnPressed__DelegateSignature();
    void ShowMouse();
    void ResetShowMouseCursor();
    void BndEvt__Command_Select_K2Node_ComponentBoundEvent_3_OnPressed__DelegateSignature();
    void ResetFocus();
    void BndEvt__CommandButton_OpenPhotoModeFullscreen_K2Node_ComponentBoundEvent_5_OnPressed__DelegateSignature();
    void BndEvt__Command_PhotoMode_K2Node_ComponentBoundEvent_0_OnPressed__DelegateSignature();
    void BndEvt__PreviousPageButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__NextPageButton_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_PhotoModeGalleryWidget(int32 EntryPoint);
}; // Size: 0x4E4

#endif

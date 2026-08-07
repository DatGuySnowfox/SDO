#ifndef UE4SS_SDK_UMG_RadialMenu_HPP
#define UE4SS_SDK_UMG_RadialMenu_HPP

class UUMG_RadialMenu_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* RadialBackground;                                                   // 0x02C8 (size: 0x8)
    class UImage* RadiusDebug;                                                        // 0x02D0 (size: 0x8)
    class UOverlay* RootOverlay;                                                      // 0x02D8 (size: 0x8)
    int32 Segments;                                                                   // 0x02E0 (size: 0x4)
    class UMaterialInterface* MaterialBase;                                           // 0x02E8 (size: 0x8)
    double IndexRaw;                                                                  // 0x02F0 (size: 0x8)
    int32 Index;                                                                      // 0x02F8 (size: 0x4)
    class UMaterialInstanceDynamic* dynaMat;                                          // 0x0300 (size: 0x8)
    TArray<class UUserWidget*> Children;                                              // 0x0308 (size: 0x10)
    bool AllowResizing;                                                               // 0x0318 (size: 0x1)
    double ItemOffsetRadius;                                                          // 0x0320 (size: 0x8)
    FUMG_RadialMenu_CSelectionChanged SelectionChanged;                               // 0x0328 (size: 0x10)
    void SelectionChanged(int32 NewSelection, int32 OldSelection);
    class APlayerController* InputController;                                         // 0x0338 (size: 0x8)
    TEnumAsByte<EJoystickTypes::Type> JoystickToUse;                                  // 0x0340 (size: 0x1)
    double MouseDeadzone;                                                             // 0x0348 (size: 0x8)
    bool bInDeadzone;                                                                 // 0x0350 (size: 0x1)
    FUMG_RadialMenu_CEnteredDeadzone EnteredDeadzone;                                 // 0x0358 (size: 0x10)
    void EnteredDeadzone();
    FUMG_RadialMenu_CExitedDeadzone ExitedDeadzone;                                   // 0x0368 (size: 0x10)
    void ExitedDeadzone();
    FVector2D CurrentInput;                                                           // 0x0378 (size: 0x10)
    double JoystickDeadzone;                                                          // 0x0388 (size: 0x8)
    bool DefaultToMouseIfNoJoystick;                                                  // 0x0390 (size: 0x1)
    int32 DefaultPlayerIndex;                                                         // 0x0394 (size: 0x4)
    bool UseDeadzoneEvents;                                                           // 0x0398 (size: 0x1)
    class USoundBase* SelectionChangedSound;                                          // 0x03A0 (size: 0x8)
    bool AutoCenterMouse;                                                             // 0x03A8 (size: 0x1)
    bool AutoRotateForUp;                                                             // 0x03A9 (size: 0x1)
    bool bUseCustomInput;                                                             // 0x03AA (size: 0x1)
    FVector2D CustomInput;                                                            // 0x03B0 (size: 0x10)
    bool UseCustomTexture;                                                            // 0x03C0 (size: 0x1)
    class UTexture* RadialTexture;                                                    // 0x03C8 (size: 0x8)
    bool UseCustomColors;                                                             // 0x03D0 (size: 0x1)
    FLinearColor InnterRingColor;                                                     // 0x03D4 (size: 0x10)
    FLinearColor OuterRingColor;                                                      // 0x03E4 (size: 0x10)
    FLinearColor RingMainColor;                                                       // 0x03F4 (size: 0x10)
    FLinearColor UnhighlightColor;                                                    // 0x0404 (size: 0x10)

    void UpdateMaterialVisuals();
    void GetCustomInput(FVector2D& Input, bool& Valid);
    void SetCustomInput(FVector2D CustomInput);
    void UpdateDirectionWithCustomInput();
    void RemoveChildIndexFromRadialMenu(int32 IndexToRemove);
    void RemoveChildWidgetFromRadialMenu(const class UUserWidget*& ItemToFind);
    FText DebugIndex();
    FVector2D FixInputRotation(FVector2D Input);
    void FixMainRotation();
    void AutoRegisterToInput();
    void UpdateInput();
    void RegisterPlayerInput(class APlayerController* Controller, bool& Success);
    void UpdateDirectionWithJoystick();
    void GetCurrentIndex(int32& Index, bool& Valid);
    void GetChild(int32 Index, class UUserWidget*& Output, bool& Success);
    void SetIndex(int32 Index);
    void UpdateAllChildrenPositions();
    void UpdateChildPosition(int32 Index);
    void ClearChildren();
    void GetSelectedWidget(class UUserWidget*& Output);
    void AddChildToRadialMenu(class UUserWidget* Content, bool& Success);
    void UpdateDirectionWithMouseCursor();
    double GetSectionDegreeSize();
    void SetInputDirection(FVector2D Direction);
    void Construct();
    void UpdateSegments(int32 Segments);
    void InDeadzone(bool InDeadzone);
    void UpdateMaterials();
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_UMG_RadialMenu(int32 EntryPoint);
    void ExitedDeadzone__DelegateSignature();
    void EnteredDeadzone__DelegateSignature();
    void SelectionChanged__DelegateSignature(int32 NewSelection, int32 OldSelection);
}; // Size: 0x414

#endif

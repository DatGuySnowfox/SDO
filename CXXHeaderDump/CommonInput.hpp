#ifndef UE4SS_SDK_CommonInput_HPP
#define UE4SS_SDK_CommonInput_HPP

#include "CommonInput_enums.hpp"

struct FCommonInputKeyBrushConfiguration
{
    FKey Key;                                                                         // 0x0000 (size: 0x18)
    FSlateBrush KeyBrush;                                                             // 0x0020 (size: 0xD0)

}; // Size: 0xF0

struct FCommonInputKeySetBrushConfiguration
{
    TArray<FKey> Keys;                                                                // 0x0000 (size: 0x10)
    FSlateBrush KeyBrush;                                                             // 0x0010 (size: 0xD0)

}; // Size: 0xE0

struct FCommonInputPlatformBaseData
{
    ECommonInputType DefaultInputType;                                                // 0x0008 (size: 0x1)
    bool bSupportsMouseAndKeyboard;                                                   // 0x0009 (size: 0x1)
    bool bSupportsGamepad;                                                            // 0x000A (size: 0x1)
    FName DefaultGamepadName;                                                         // 0x000C (size: 0x8)
    bool bCanChangeGamepadType;                                                       // 0x0014 (size: 0x1)
    bool bSupportsTouch;                                                              // 0x0015 (size: 0x1)
    TArray<TSoftClassPtr<UCommonInputBaseControllerData>> ControllerData;             // 0x0018 (size: 0x10)
    TArray<class TSubclassOf<UCommonInputBaseControllerData>> ControllerDataClasses;  // 0x0028 (size: 0x10)

}; // Size: 0x38

struct FInputDeviceIdentifierPair
{
    FName InputDeviceName;                                                            // 0x0000 (size: 0x8)
    FString HardwareDeviceIdentifier;                                                 // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FInputHoldData
{
    float HoldTime;                                                                   // 0x0000 (size: 0x4)
    float HoldRollbackTime;                                                           // 0x0004 (size: 0x4)

}; // Size: 0x8

class UCommonInputActionDomain : public UDataAsset
{
    ECommonInputEventFlowBehavior Behavior;                                           // 0x0030 (size: 0x4)
    ECommonInputEventFlowBehavior InnerBehavior;                                      // 0x0034 (size: 0x4)
    bool bUseActionDomainDesiredInputConfig;                                          // 0x0038 (size: 0x1)
    ECommonInputMode InputMode;                                                       // 0x0039 (size: 0x1)
    EMouseCaptureMode MouseCaptureMode;                                               // 0x003A (size: 0x1)

}; // Size: 0x40

class UCommonInputActionDomainTable : public UDataAsset
{
    TArray<class UCommonInputActionDomain*> ActionDomains;                            // 0x0030 (size: 0x10)
    ECommonInputMode InputMode;                                                       // 0x0040 (size: 0x1)
    EMouseCaptureMode MouseCaptureMode;                                               // 0x0041 (size: 0x1)

}; // Size: 0x48

class UCommonInputBaseControllerData : public UObject
{
    ECommonInputType InputType;                                                       // 0x0028 (size: 0x1)
    FName GamepadName;                                                                // 0x002C (size: 0x8)
    FText GamepadDisplayName;                                                         // 0x0038 (size: 0x18)
    FText GamepadCategory;                                                            // 0x0050 (size: 0x18)
    FText GamepadPlatformName;                                                        // 0x0068 (size: 0x18)
    TArray<FInputDeviceIdentifierPair> GamepadHardwareIdMapping;                      // 0x0080 (size: 0x10)
    TSoftObjectPtr<UTexture2D> ControllerTexture;                                     // 0x0090 (size: 0x28)
    TSoftObjectPtr<UTexture2D> ControllerButtonMaskTexture;                           // 0x00B8 (size: 0x28)
    TArray<FCommonInputKeyBrushConfiguration> InputBrushDataMap;                      // 0x00E0 (size: 0x10)
    TArray<FCommonInputKeySetBrushConfiguration> InputBrushKeySets;                   // 0x00F0 (size: 0x10)

    TArray<FName> GetRegisteredGamepads();
}; // Size: 0x100

class UCommonInputPlatformSettings : public UPlatformSettings
{
    ECommonInputType DefaultInputType;                                                // 0x0040 (size: 0x1)
    bool bSupportsMouseAndKeyboard;                                                   // 0x0041 (size: 0x1)
    bool bSupportsTouch;                                                              // 0x0042 (size: 0x1)
    bool bSupportsGamepad;                                                            // 0x0043 (size: 0x1)
    FName DefaultGamepadName;                                                         // 0x0044 (size: 0x8)
    bool bCanChangeGamepadType;                                                       // 0x004C (size: 0x1)
    TArray<TSoftClassPtr<UCommonInputBaseControllerData>> ControllerData;             // 0x0050 (size: 0x10)
    TArray<class TSubclassOf<UCommonInputBaseControllerData>> ControllerDataClasses;  // 0x0060 (size: 0x10)

}; // Size: 0x70

class UCommonInputSettings : public UDeveloperSettings
{
    TSoftClassPtr<UCommonUIInputData> InputData;                                      // 0x0038 (size: 0x28)
    FPerPlatformSettings PlatformInput;                                               // 0x0060 (size: 0x10)
    TMap<class FName, class FCommonInputPlatformBaseData> CommonInputPlatformData;    // 0x0070 (size: 0x50)
    bool bEnableInputMethodThrashingProtection;                                       // 0x00C0 (size: 0x1)
    int32 InputMethodThrashingLimit;                                                  // 0x00C4 (size: 0x4)
    double InputMethodThrashingWindowInSeconds;                                       // 0x00C8 (size: 0x8)
    double InputMethodThrashingCooldownInSeconds;                                     // 0x00D0 (size: 0x8)
    bool bAllowOutOfFocusDeviceInput;                                                 // 0x00D8 (size: 0x1)
    bool bEnableDefaultInputConfig;                                                   // 0x00D9 (size: 0x1)
    bool bEnableEnhancedInputSupport;                                                 // 0x00DA (size: 0x1)
    TSoftObjectPtr<UCommonInputActionDomainTable> ActionDomainTable;                  // 0x00E0 (size: 0x28)
    TSubclassOf<class UCommonUIInputData> InputDataClass;                             // 0x0110 (size: 0x8)
    class UCommonInputActionDomainTable* ActionDomainTablePtr;                        // 0x0118 (size: 0x8)

    bool IsEnhancedInputSupportEnabled();
}; // Size: 0x120

class UCommonInputSubsystem : public ULocalPlayerSubsystem
{
    FCommonInputSubsystemOnInputMethodChanged OnInputMethodChanged;                   // 0x0068 (size: 0x10)
    void InputMethodChangedDelegate(ECommonInputType bNewInputType);
    int32 NumberOfInputMethodChangesRecently;                                         // 0x0078 (size: 0x4)
    double LastInputMethodChangeTime;                                                 // 0x0080 (size: 0x8)
    double LastTimeInputMethodThrashingBegan;                                         // 0x0088 (size: 0x8)
    ECommonInputType LastInputType;                                                   // 0x0090 (size: 0x1)
    ECommonInputType CurrentInputType;                                                // 0x0091 (size: 0x1)
    FName GamepadInputType;                                                           // 0x0094 (size: 0x8)
    TMap<class FName, class ECommonInputType> CurrentInputLocks;                      // 0x00A0 (size: 0x50)
    class UCommonInputActionDomainTable* ActionDomainTable;                           // 0x00F8 (size: 0x8)
    bool bIsGamepadSimulatedClick;                                                    // 0x0100 (size: 0x1)

    bool ShouldShowInputKeys();
    void SetGamepadInputType(const FName InGamepadInputType);
    void SetCurrentInputType(ECommonInputType NewInputType);
    bool IsUsingPointerInput();
    bool IsInputMethodActive(ECommonInputType InputMethod);
    ECommonInputType GetDefaultInputType();
    ECommonInputType GetCurrentInputType();
    FName GetCurrentGamepadName();
    void BroadcastInputMethodChanged();
}; // Size: 0x118

class UCommonUIHoldData : public UObject
{
    FInputHoldData KeyboardAndMouse;                                                  // 0x0028 (size: 0x8)
    FInputHoldData Gamepad;                                                           // 0x0030 (size: 0x8)
    FInputHoldData Touch;                                                             // 0x0038 (size: 0x8)

}; // Size: 0x40

class UCommonUIInputData : public UObject
{
    FDataTableRowHandle DefaultClickAction;                                           // 0x0028 (size: 0x10)
    FDataTableRowHandle DefaultBackAction;                                            // 0x0038 (size: 0x10)
    TSoftClassPtr<UCommonUIHoldData> DefaultHoldData;                                 // 0x0048 (size: 0x28)
    class UInputAction* EnhancedInputClickAction;                                     // 0x0070 (size: 0x8)
    class UInputAction* EnhancedInputBackAction;                                      // 0x0078 (size: 0x8)

}; // Size: 0x80

#endif

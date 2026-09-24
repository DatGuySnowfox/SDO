#ifndef UE4SS_SDK_EnhancedInput_HPP
#define UE4SS_SDK_EnhancedInput_HPP

#include "EnhancedInput_enums.hpp"

struct FAppliedInputContextData
{
    int32 Priority;                                                                   // 0x0000 (size: 0x4)
    int32 RegistrationCount;                                                          // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FBlueprintEnhancedInputActionBinding
{
    class UInputAction* InputAction;                                                  // 0x0000 (size: 0x8)
    ETriggerEvent TriggerEvent;                                                       // 0x0008 (size: 0x1)
    FName FunctionNameToBind;                                                         // 0x000C (size: 0x8)

}; // Size: 0x18

struct FBlueprintInputDebugKeyDelegateBinding
{
    FInputChord InputChord;                                                           // 0x0000 (size: 0x20)
    TEnumAsByte<EInputEvent> InputKeyEvent;                                           // 0x0020 (size: 0x1)
    FName FunctionNameToBind;                                                         // 0x0024 (size: 0x8)
    bool bExecuteWhenPaused;                                                          // 0x002C (size: 0x1)

}; // Size: 0x30

struct FDefaultContextSetting
{
    TSoftObjectPtr<class UInputMappingContext> InputMappingContext;                   // 0x0000 (size: 0x28)
    int32 Priority;                                                                   // 0x0028 (size: 0x4)
    bool bAddImmediately;                                                             // 0x002C (size: 0x1)
    bool bRegisterWithUserSettings;                                                   // 0x002D (size: 0x1)

}; // Size: 0x30

struct FEnhancedActionKeyMapping
{
    TArray<UInputTrigger*> Triggers;                                                  // 0x0000 (size: 0x10)
    TArray<UInputModifier*> Modifiers;                                                // 0x0010 (size: 0x10)
    class UInputAction* Action;                                                       // 0x0020 (size: 0x8)
    FKey Key;                                                                         // 0x0028 (size: 0x18)
    uint8 bShouldBeIgnored;                                                           // 0x0040 (size: 0x1)
    uint8 bHasAlwaysTickTrigger;                                                      // 0x0040 (size: 0x1)
    EPlayerMappableKeySettingBehaviors SettingBehavior;                               // 0x0041 (size: 0x1)
    class UPlayerMappableKeySettings* PlayerMappableKeySettings;                      // 0x0048 (size: 0x8)

}; // Size: 0x50

struct FInjectedInput
{
    TArray<UInputTrigger*> Triggers;                                                  // 0x0020 (size: 0x10)
    TArray<UInputModifier*> Modifiers;                                                // 0x0030 (size: 0x10)

}; // Size: 0x40

struct FInjectedInputArray
{
    TArray<FInjectedInput> Injected;                                                  // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FInputActionInstance
{
    class UInputAction* SourceAction;                                                 // 0x0000 (size: 0x8)
    ETriggerEvent TriggerEvent;                                                       // 0x0013 (size: 0x1)
    float LastTriggeredWorldTime;                                                     // 0x0014 (size: 0x4)
    TArray<UInputTrigger*> Triggers;                                                  // 0x0018 (size: 0x10)
    TArray<UInputModifier*> Modifiers;                                                // 0x0028 (size: 0x10)
    float ElapsedProcessedTime;                                                       // 0x0058 (size: 0x4)
    float ElapsedTriggeredTime;                                                       // 0x005C (size: 0x4)

}; // Size: 0x60

struct FInputActionValue
{
}; // Size: 0x20

struct FInputCancelAction
{
    class UInputAction* CancelAction;                                                 // 0x0000 (size: 0x8)
    uint8 CancellationStates;                                                         // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FInputComboStepData
{
    class UInputAction* ComboStepAction;                                              // 0x0000 (size: 0x8)
    uint8 ComboStepCompletionStates;                                                  // 0x0008 (size: 0x1)
    float TimeToPressKey;                                                             // 0x000C (size: 0x4)

}; // Size: 0x10

struct FKeyConsumptionOptions
{
}; // Size: 0x18

struct FKeyMappingRow
{
    TSet<FPlayerKeyMapping> Mappings;                                                 // 0x0000 (size: 0x50)

}; // Size: 0x50

struct FMapPlayerKeyArgs
{
    FName MappingName;                                                                // 0x0000 (size: 0x8)
    EPlayerMappableKeySlot Slot;                                                      // 0x0008 (size: 0x1)
    FKey NewKey;                                                                      // 0x0010 (size: 0x18)
    FName HardwareDeviceId;                                                           // 0x0028 (size: 0x8)
    FGameplayTag ProfileId;                                                           // 0x0030 (size: 0x8)
    FString ProfileIdString;                                                          // 0x0038 (size: 0x10)
    uint8 bCreateMatchingSlotIfNeeded;                                                // 0x0048 (size: 0x1)
    uint8 bDeferOnSettingsChangedBroadcast;                                           // 0x0048 (size: 0x1)

}; // Size: 0x50

struct FMappingQueryIssue
{
    EMappingQueryIssue Issue;                                                         // 0x0000 (size: 0x1)
    class UInputMappingContext* BlockingContext;                                      // 0x0008 (size: 0x8)
    class UInputAction* BlockingAction;                                               // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FModifyContextOptions
{
    uint8 bIgnoreAllPressedKeysUntilRelease;                                          // 0x0000 (size: 0x1)
    uint8 bForceImmediately;                                                          // 0x0000 (size: 0x1)
    uint8 bNotifyUserSettings;                                                        // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FPlayerKeyMapping
{
    FName MappingName;                                                                // 0x0000 (size: 0x8)
    FText DisplayName;                                                                // 0x0008 (size: 0x10)
    FText DisplayCategory;                                                            // 0x0018 (size: 0x10)
    EPlayerMappableKeySlot Slot;                                                      // 0x0028 (size: 0x1)
    uint8 bIsDirty;                                                                   // 0x0029 (size: 0x1)
    FKey DefaultKey;                                                                  // 0x0030 (size: 0x18)
    FKey CurrentKey;                                                                  // 0x0048 (size: 0x18)
    FHardwareDeviceIdentifier HardwareDeviceId;                                       // 0x0060 (size: 0x18)
    class UInputAction* AssociatedInputAction;                                        // 0x0078 (size: 0x8)
    TSoftObjectPtr<class UInputAction> AssociatedInputActionSoft;                     // 0x0080 (size: 0x28)

}; // Size: 0xA8

struct FPlayerMappableKeyProfileCreationArgs
{
    TSubclassOf<class UEnhancedPlayerMappableKeyProfile> ProfileType;                 // 0x0000 (size: 0x8)
    FGameplayTag ProfileIdentifier;                                                   // 0x0008 (size: 0x8)
    FString ProfileStringIdentifier;                                                  // 0x0010 (size: 0x10)
    FPlatformUserId UserId;                                                           // 0x0020 (size: 0x4)
    FText DisplayName;                                                                // 0x0028 (size: 0x10)
    uint8 bSetAsCurrentProfile;                                                       // 0x0038 (size: 0x1)

}; // Size: 0x40

struct FPlayerMappableKeyQueryOptions
{
    FName MappingName;                                                                // 0x0000 (size: 0x8)
    FKey KeyToMatch;                                                                  // 0x0008 (size: 0x18)
    EPlayerMappableKeySlot SlotToMatch;                                               // 0x0020 (size: 0x1)
    uint8 bMatchBasicKeyTypes;                                                        // 0x0021 (size: 0x1)
    uint8 bMatchKeyAxisType;                                                          // 0x0021 (size: 0x1)
    EHardwareDevicePrimaryType RequiredDeviceType;                                    // 0x0022 (size: 0x1)
    int32 RequiredDeviceFlags;                                                        // 0x0024 (size: 0x4)

}; // Size: 0x28

class IEnhancedInputSubsystemInterface : public IInterface
{

    void UpdateValueOfContinuousInputInjectionForPlayerMapping(const FName MappingName, FInputActionValue RawValue);
    void UpdateValueOfContinuousInputInjectionForAction(const class UInputAction* Action, FInputActionValue RawValue);
    void StopContinuousInputInjectionForPlayerMapping(const FName MappingName);
    void StopContinuousInputInjectionForAction(const class UInputAction* Action);
    void StartContinuousInputInjectionForPlayerMapping(const FName MappingName, FInputActionValue RawValue, const TArray<UInputModifier*>& Modifiers, const TArray<UInputTrigger*>& Triggers);
    void StartContinuousInputInjectionForAction(const class UInputAction* Action, FInputActionValue RawValue, const TArray<UInputModifier*>& Modifiers, const TArray<UInputTrigger*>& Triggers);
    void SetInputMode(const FGameplayTagContainer& NewMode, const FModifyContextOptions& Options);
    void RequestRebuildControlMappings(const FModifyContextOptions& Options, EInputMappingRebuildType RebuildType);
    void RemoveTagsFromInputMode(const FGameplayTagContainer& TagsToRemove, const FModifyContextOptions& Options);
    void RemoveTagFromInputMode(const FGameplayTag& TagToRemove, const FModifyContextOptions& Options);
    void RemoveMappingContext(const class UInputMappingContext* MappingContext, const FModifyContextOptions& Options);
    EMappingQueryResult QueryMapKeyInContextSet(const TArray<UInputMappingContext*>& PrioritizedActiveContexts, const class UInputMappingContext* InputContext, const class UInputAction* Action, FKey Key, TArray<FMappingQueryIssue>& OutIssues, EMappingQueryIssue BlockingIssues);
    EMappingQueryResult QueryMapKeyInActiveContextSet(const class UInputMappingContext* InputContext, const class UInputAction* Action, FKey Key, TArray<FMappingQueryIssue>& OutIssues, EMappingQueryIssue BlockingIssues);
    TArray<FKey> QueryKeysMappedToAction(const class UInputAction* Action);
    void OnUserSettingsChanged(class UEnhancedInputUserSettings* Settings);
    void OnUserKeyProfileChanged(const class UEnhancedPlayerMappableKeyProfile* InNewProfile);
    void InjectInputVectorForPlayerMapping(const FName MappingName, FVector Value, const TArray<UInputModifier*>& Modifiers, const TArray<UInputTrigger*>& Triggers);
    void InjectInputVectorForAction(const class UInputAction* Action, FVector Value, const TArray<UInputModifier*>& Modifiers, const TArray<UInputTrigger*>& Triggers);
    void InjectInputForPlayerMapping(const FName MappingName, FInputActionValue RawValue, const TArray<UInputModifier*>& Modifiers, const TArray<UInputTrigger*>& Triggers);
    void InjectInputForAction(const class UInputAction* Action, FInputActionValue RawValue, const TArray<UInputModifier*>& Modifiers, const TArray<UInputTrigger*>& Triggers);
    bool HasMappingContext(const class UInputMappingContext* MappingContext, int32& OutFoundPriority);
    class UEnhancedInputUserSettings* GetUserSettings();
    FGameplayTagContainer GetInputMode();
    TArray<FEnhancedActionKeyMapping> GetAllPlayerMappableActionKeyMappings();
    void ClearAllMappings();
    void AppendTagsToInputMode(const FGameplayTagContainer& TagsToAdd, const FModifyContextOptions& Options);
    void AddTagToInputMode(const FGameplayTag& TagToAdd, const FModifyContextOptions& Options);
    void AddMappingContext(const class UInputMappingContext* MappingContext, int32 Priority, const FModifyContextOptions& Options);
}; // Size: 0x28

class UEnhancedInputActionDelegateBinding : public UInputDelegateBinding
{
    TArray<FBlueprintEnhancedInputActionBinding> InputActionDelegateBindings;         // 0x0028 (size: 0x10)

}; // Size: 0x38

class UEnhancedInputActionValueBinding : public UInputDelegateBinding
{
    TArray<FBlueprintEnhancedInputActionBinding> InputActionValueBindings;            // 0x0028 (size: 0x10)

}; // Size: 0x38

class UEnhancedInputComponent : public UInputComponent
{

    FInputActionValue GetBoundActionValue(const class UInputAction* Action);
}; // Size: 0x178

class UEnhancedInputDeveloperSettings : public UDeveloperSettingsBackedByCVars
{
    TArray<FDefaultContextSetting> DefaultMappingContexts;                            // 0x0038 (size: 0x10)
    TArray<FDefaultContextSetting> DefaultWorldSubsystemMappingContexts;              // 0x0048 (size: 0x10)
    FPerPlatformSettings PlatformSettings;                                            // 0x0058 (size: 0x10)
    TSoftClassPtr<UEnhancedInputUserSettings> UserSettingsClass;                      // 0x0068 (size: 0x28)
    TSoftClassPtr<UEnhancedPlayerMappableKeyProfile> DefaultPlayerMappableKeyProfileClass; // 0x0090 (size: 0x28)
    FString InputSettingsSaveSlotName;                                                // 0x00B8 (size: 0x10)
    TSoftClassPtr<UEnhancedPlayerInput> DefaultWorldInputClass;                       // 0x00C8 (size: 0x28)
    uint8 bSendTriggeredEventsWhenInputIsFlushed;                                     // 0x00F0 (size: 0x1)
    uint8 bEnableUserSettings;                                                        // 0x00F0 (size: 0x1)
    uint8 bEnableDefaultMappingContexts;                                              // 0x00F0 (size: 0x1)
    uint8 bShouldOnlyTriggerLastActionInChord;                                        // 0x00F0 (size: 0x1)
    uint8 bEnableInputModeFiltering;                                                  // 0x00F0 (size: 0x1)
    uint8 bEnableWorldSubsystem;                                                      // 0x00F0 (size: 0x1)
    FGameplayTagQuery DefaultMappingContextInputModeQuery;                            // 0x00F8 (size: 0x48)
    FGameplayTagContainer DefaultInputMode;                                           // 0x0140 (size: 0x20)

}; // Size: 0x160

class UEnhancedInputLibrary : public UBlueprintFunctionLibrary
{

    void RequestRebuildControlMappingsUsingContext(const class UInputMappingContext* Context, bool bForceImmediately);
    FInputActionValue MakeInputActionValueOfType(double X, double Y, double Z, const EInputActionValueType ValueType);
    bool IsActionKeyMappingPlayerMappable(const FEnhancedActionKeyMapping& ActionKeyMapping);
    class UPlayerMappableKeySettings* GetPlayerMappableKeySettings(const FEnhancedActionKeyMapping& ActionKeyMapping);
    FName GetMappingName(const FEnhancedActionKeyMapping& ActionKeyMapping);
    FInputActionValue GetBoundActionValue(class AActor* Actor, const class UInputAction* Action);
    void FlushPlayerInput(class APlayerController* PlayerController);
    FString Conv_TriggerEventValueToString(const ETriggerEvent TriggerEvent);
    FString Conv_InputActionValueToString(FInputActionValue ActionValue);
    bool Conv_InputActionValueToBool(FInputActionValue InValue);
    FVector Conv_InputActionValueToAxis3D(FInputActionValue ActionValue);
    FVector2D Conv_InputActionValueToAxis2D(FInputActionValue InValue);
    double Conv_InputActionValueToAxis1D(FInputActionValue InValue);
    void BreakInputActionValue(FInputActionValue InActionValue, double& X, double& Y, double& Z, EInputActionValueType& Type);
}; // Size: 0x28

class UEnhancedInputLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
    FEnhancedInputLocalPlayerSubsystemControlMappingsRebuiltDelegate ControlMappingsRebuiltDelegate; // 0x0180 (size: 0x10)
    void OnControlMappingsRebuilt();
    FEnhancedInputLocalPlayerSubsystemOnMappingContextAdded OnMappingContextAdded;    // 0x0190 (size: 0x10)
    void OnMappingContextAdded(const class UInputMappingContext* MappingContext);
    FEnhancedInputLocalPlayerSubsystemOnMappingContextRemoved OnMappingContextRemoved; // 0x01A0 (size: 0x10)
    void OnMappingContextRemoved(const class UInputMappingContext* MappingContext);
    class UEnhancedInputUserSettings* UserSettings;                                   // 0x01B0 (size: 0x8)
    TMap<UInputAction*, FInjectedInput> ContinuouslyInjectedInputs;                   // 0x01B8 (size: 0x50)

    void OnMappingContextRemoved__DelegateSignature(const class UInputMappingContext* MappingContext);
    void OnMappingContextAdded__DelegateSignature(const class UInputMappingContext* MappingContext);
    void OnControlMappingsRebuilt__DelegateSignature();
}; // Size: 0x208

class UEnhancedInputPlatformData : public UObject
{
    TMap<UInputMappingContext*, UInputMappingContext*> MappingContextRedirects;       // 0x0028 (size: 0x50)

    class UInputMappingContext* GetContextRedirect(class UInputMappingContext* InContext);
}; // Size: 0x78

class UEnhancedInputPlatformSettings : public UPlatformSettings
{
    TArray<TSoftClassPtr<UEnhancedInputPlatformData>> InputData;                      // 0x0040 (size: 0x10)
    TArray<TSubclassOf<class UEnhancedInputPlatformData>> InputDataClasses;           // 0x0050 (size: 0x10)
    bool bShouldLogMappingContextRedirects;                                           // 0x0060 (size: 0x1)

}; // Size: 0x68

class UEnhancedInputUserSettings : public USaveGame
{
    FEnhancedInputUserSettingsOnSettingsChanged OnSettingsChanged;                    // 0x0030 (size: 0x10)
    void EnhancedInputUserSettingsChanged(class UEnhancedInputUserSettings* Settings);
    FEnhancedInputUserSettingsOnSettingsApplied OnSettingsApplied;                    // 0x0040 (size: 0x10)
    void EnhancedInputUserSettingsApplied();
    FGameplayTag CurrentProfileIdentifier;                                            // 0x0070 (size: 0x8)
    FString CurrentProfileIdentifierString;                                           // 0x0078 (size: 0x10)
    TMap<FGameplayTag, UEnhancedPlayerMappableKeyProfile*> SavedKeyProfiles;          // 0x0088 (size: 0x50)
    TMap<FString, UEnhancedPlayerMappableKeyProfile*> SavedKeyProfilesMap;            // 0x00D8 (size: 0x50)
    TWeakObjectPtr<class ULocalPlayer> OwningLocalPlayer;                             // 0x0128 (size: 0x8)
    TSet<UInputMappingContext*> RegisteredMappingContexts;                            // 0x0130 (size: 0x50)

    bool UnregisterInputMappingContexts(const TSet<UInputMappingContext*>& MappingContexts);
    bool UnregisterInputMappingContext(const class UInputMappingContext* IMC);
    void UnMapPlayerKey(const FMapPlayerKeyArgs& InArgs, FGameplayTagContainer& FailureReason);
    bool SetActiveKeyProfile(FString InProfileId);
    void SaveSettings();
    void ResetKeyProfileToDefault(const FGameplayTag& ProfileId, FGameplayTagContainer& FailureReason);
    void ResetKeyProfileIdToDefault(FString ProfileId, FGameplayTagContainer& FailureReason);
    void ResetAllPlayerKeysInRow(const FMapPlayerKeyArgs& InArgs, FGameplayTagContainer& FailureReason);
    bool RegisterInputMappingContexts(const TSet<UInputMappingContext*>& MappingContexts);
    bool RegisterInputMappingContext(const class UInputMappingContext* IMC);
    void MapPlayerKey(const FMapPlayerKeyArgs& InArgs, FGameplayTagContainer& FailureReason);
    void MappingContextRegisteredWithSettings__DelegateSignature(const class UInputMappingContext* IMC);
    void MappableKeyProfileChanged__DelegateSignature(const class UEnhancedPlayerMappableKeyProfile* NewProfile);
    bool IsMappingContextRegistered(const class UInputMappingContext* IMC);
    class UEnhancedPlayerMappableKeyProfile* GetKeyProfileWithId(FString ProfileId);
    FString GetActiveKeyProfileId();
    class UEnhancedPlayerMappableKeyProfile* GetActiveKeyProfile();
    TSet<FPlayerKeyMapping> FindMappingsInRow(const FName MappingName);
    void EnhancedInputUserSettingsChanged__DelegateSignature(class UEnhancedInputUserSettings* Settings);
    void EnhancedInputUserSettingsApplied__DelegateSignature();
    class UEnhancedPlayerMappableKeyProfile* CreateNewKeyProfile(const FPlayerMappableKeyProfileCreationArgs& InArgs);
    void AsyncSaveSettings();
    void ApplySettings();
}; // Size: 0x180

class UEnhancedInputWorldSubsystem : public UWorldSubsystem
{
    class UEnhancedPlayerInput* PlayerInput;                                          // 0x0180 (size: 0x8)
    TArray<TWeakObjectPtr<class UInputComponent>> CurrentInputStack;                  // 0x0198 (size: 0x10)
    TMap<UInputAction*, FInjectedInput> ContinuouslyInjectedInputs;                   // 0x01A8 (size: 0x50)

    bool RemoveActorInputComponent(class AActor* Actor);
    void AddActorInputComponent(class AActor* Actor);
}; // Size: 0x1F8

class UEnhancedPlayerInput : public UPlayerInput
{
    TMap<UInputAction*, FKeyConsumptionOptions> KeyConsumptionData;                   // 0x0498 (size: 0x50)
    TMap<UInputAction*, FInputActionInstance> ActionInstanceData;                     // 0x04E8 (size: 0x50)
    TMap<UInputMappingContext*, FAppliedInputContextData> AppliedInputContextData;    // 0x0538 (size: 0x50)
    TMap<UInputMappingContext*, int32> AppliedInputContexts;                          // 0x0588 (size: 0x50)
    TArray<FEnhancedActionKeyMapping> EnhancedActionMappings;                         // 0x05D8 (size: 0x10)
    FGameplayTagContainer CurrentInputMode;                                           // 0x05E8 (size: 0x20)
    TMap<FKey, FVector> KeysPressedThisTick;                                          // 0x0748 (size: 0x50)
    TMap<UInputAction*, FInjectedInputArray> InputsInjectedThisTick;                  // 0x0798 (size: 0x50)
    TSet<UInputAction*> LastInjectedActions;                                          // 0x07E8 (size: 0x50)

}; // Size: 0x8B0

class UEnhancedPlayerMappableKeyProfile : public UObject
{
    FGameplayTag ProfileIdentifier;                                                   // 0x0028 (size: 0x8)
    FString ProfileIdentifierString;                                                  // 0x0030 (size: 0x10)
    FPlatformUserId OwningUserId;                                                     // 0x0040 (size: 0x4)
    FText DisplayName;                                                                // 0x0048 (size: 0x10)
    TMap<FName, FKeyMappingRow> PlayerMappedKeys;                                     // 0x0058 (size: 0x50)

    FString ToString();
    void SetDisplayName(const FText& NewDisplayName);
    void ResetToDefault();
    void ResetMappingToDefault(const FName InMappingName);
    int32 QueryPlayerMappedKeys(const FPlayerMappableKeyQueryOptions& Options, TArray<FKey>& OutKeys);
    void K2_FindKeyMapping(FPlayerKeyMapping& OutKeyMapping, const FMapPlayerKeyArgs& InArgs);
    FString GetProfileIdString();
    FText GetProfileDisplayName();
    TMap<FName, FKeyMappingRow> GetPlayerMappingRows();
    int32 GetMappingNamesForKey(const FKey& InKey, TArray<FName>& OutMappingNames);
    int32 GetMappedKeysInRow(const FName MappingName, TArray<FKey>& OutKeys);
    void DumpProfileToLog();
    bool DoesMappingPassQueryOptions(const FPlayerKeyMapping& PlayerMapping, const FPlayerMappableKeyQueryOptions& Options);
}; // Size: 0xA8

class UInputAction : public UDataAsset
{
    FText ActionDescription;                                                          // 0x0030 (size: 0x10)
    bool bTriggerWhenPaused;                                                          // 0x0040 (size: 0x1)
    bool bConsumeInput;                                                               // 0x0041 (size: 0x1)
    bool bConsumesActionAndAxisMappings;                                              // 0x0042 (size: 0x1)
    bool bReserveAllMappings;                                                         // 0x0043 (size: 0x1)
    int32 TriggerEventsThatConsumeLegacyKeys;                                         // 0x0044 (size: 0x4)
    EInputActionValueType ValueType;                                                  // 0x0048 (size: 0x1)
    EInputActionAccumulationBehavior AccumulationBehavior;                            // 0x0049 (size: 0x1)
    TArray<UInputTrigger*> Triggers;                                                  // 0x0050 (size: 0x10)
    TArray<UInputModifier*> Modifiers;                                                // 0x0060 (size: 0x10)
    class UPlayerMappableKeySettings* PlayerMappableKeySettings;                      // 0x0070 (size: 0x8)

}; // Size: 0x78

class UInputDebugKeyDelegateBinding : public UInputDelegateBinding
{
    TArray<FBlueprintInputDebugKeyDelegateBinding> InputDebugKeyDelegateBindings;     // 0x0028 (size: 0x10)

}; // Size: 0x38

class UInputMappingContext : public UDataAsset
{
    TArray<FEnhancedActionKeyMapping> Mappings;                                       // 0x0030 (size: 0x10)
    EMappingContextInputModeFilterOptions InputModeFilterOptions;                     // 0x0040 (size: 0x1)
    FGameplayTagQuery InputModeQueryOverride;                                         // 0x0048 (size: 0x48)
    EMappingContextRegistrationTrackingMode RegistrationTrackingMode;                 // 0x0090 (size: 0x1)
    FText ContextDescription;                                                         // 0x0098 (size: 0x10)

    void UnmapKey(const class UInputAction* Action, FKey Key);
    void UnmapAllKeysFromAction(const class UInputAction* Action);
    void UnmapAll();
    bool ShouldShowInputModeQuery();
    FEnhancedActionKeyMapping MapKey(const class UInputAction* Action, FKey ToKey);
}; // Size: 0xA8

class UInputModifier : public UObject
{

    FInputActionValue ModifyRaw(const class UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime);
    FLinearColor GetVisualizationColor(FInputActionValue SampleValue, FInputActionValue FinalValue);
}; // Size: 0x28

class UInputModifierDeadZone : public UInputModifier
{
    float LowerThreshold;                                                             // 0x0028 (size: 0x4)
    float UpperThreshold;                                                             // 0x002C (size: 0x4)
    EDeadZoneType Type;                                                               // 0x0030 (size: 0x1)

}; // Size: 0x38

class UInputModifierFOVScaling : public UInputModifier
{
    float FOVScale;                                                                   // 0x0028 (size: 0x4)
    EFOVScalingType FOVScalingType;                                                   // 0x002C (size: 0x1)

}; // Size: 0x30

class UInputModifierNegate : public UInputModifier
{
    bool bX;                                                                          // 0x0028 (size: 0x1)
    bool bY;                                                                          // 0x0029 (size: 0x1)
    bool bZ;                                                                          // 0x002A (size: 0x1)

}; // Size: 0x30

class UInputModifierResponseCurveExponential : public UInputModifier
{
    FVector CurveExponent;                                                            // 0x0028 (size: 0x18)

}; // Size: 0x40

class UInputModifierResponseCurveUser : public UInputModifier
{
    class UCurveFloat* ResponseX;                                                     // 0x0028 (size: 0x8)
    class UCurveFloat* ResponseY;                                                     // 0x0030 (size: 0x8)
    class UCurveFloat* ResponseZ;                                                     // 0x0038 (size: 0x8)

}; // Size: 0x40

class UInputModifierScalar : public UInputModifier
{
    FVector Scalar;                                                                   // 0x0028 (size: 0x18)

}; // Size: 0x40

class UInputModifierScaleByDeltaTime : public UInputModifier
{
}; // Size: 0x28

class UInputModifierSmooth : public UInputModifier
{
}; // Size: 0x58

class UInputModifierSmoothDelta : public UInputModifier
{
    ENormalizeInputSmoothingType SmoothingMethod;                                     // 0x0028 (size: 0x1)
    float Speed;                                                                      // 0x002C (size: 0x4)
    float EasingExponent;                                                             // 0x0030 (size: 0x4)

}; // Size: 0x68

class UInputModifierSwizzleAxis : public UInputModifier
{
    EInputAxisSwizzle Order;                                                          // 0x0028 (size: 0x1)

}; // Size: 0x30

class UInputModifierToWorldSpace : public UInputModifier
{
}; // Size: 0x28

class UInputTrigger : public UObject
{
    float ActuationThreshold;                                                         // 0x0028 (size: 0x4)
    bool bShouldAlwaysTick;                                                           // 0x002C (size: 0x1)
    FInputActionValue LastValue;                                                      // 0x0030 (size: 0x20)

    ETriggerState UpdateState(const class UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime);
    bool IsActuated(const FInputActionValue& ForValue);
    ETriggerType GetTriggerType();
}; // Size: 0x50

class UInputTriggerChordAction : public UInputTrigger
{
    class UInputAction* ChordAction;                                                  // 0x0050 (size: 0x8)

}; // Size: 0x58

class UInputTriggerChordBlocker : public UInputTriggerChordAction
{
}; // Size: 0x58

class UInputTriggerCombo : public UInputTrigger
{
    int32 CurrentComboStepIndex;                                                      // 0x0050 (size: 0x4)
    float CurrentTimeBetweenComboSteps;                                               // 0x0054 (size: 0x4)
    TArray<FInputComboStepData> ComboActions;                                         // 0x0058 (size: 0x10)
    TArray<FInputCancelAction> InputCancelActions;                                    // 0x0068 (size: 0x10)

}; // Size: 0x78

class UInputTriggerDown : public UInputTrigger
{
}; // Size: 0x50

class UInputTriggerHold : public UInputTriggerTimedBase
{
    float HoldTimeThreshold;                                                          // 0x005C (size: 0x4)
    bool bIsOneShot;                                                                  // 0x0060 (size: 0x1)

}; // Size: 0x68

class UInputTriggerHoldAndRelease : public UInputTriggerTimedBase
{
    float HoldTimeThreshold;                                                          // 0x0058 (size: 0x4)

}; // Size: 0x60

class UInputTriggerPressed : public UInputTrigger
{
}; // Size: 0x50

class UInputTriggerPulse : public UInputTriggerTimedBase
{
    bool bTriggerOnStart;                                                             // 0x005C (size: 0x1)
    float Interval;                                                                   // 0x0060 (size: 0x4)
    int32 TriggerLimit;                                                               // 0x0064 (size: 0x4)

}; // Size: 0x68

class UInputTriggerReleased : public UInputTrigger
{
}; // Size: 0x50

class UInputTriggerRepeatedTap : public UInputTriggerTimedBase
{
    double RepeatDelay;                                                               // 0x0058 (size: 0x8)
    double RepeatTime;                                                                // 0x0060 (size: 0x8)
    int32 NumberOfTapsWhichTriggerRepeat;                                             // 0x0068 (size: 0x4)
    float TapReleaseTimeThreshold;                                                    // 0x006C (size: 0x4)
    int32 NumberOfTapsSinceLastTrigger;                                               // 0x0070 (size: 0x4)

}; // Size: 0x78

class UInputTriggerTap : public UInputTriggerTimedBase
{
    float TapReleaseTimeThreshold;                                                    // 0x0058 (size: 0x4)

}; // Size: 0x60

class UInputTriggerTimedBase : public UInputTrigger
{
    float HeldDuration;                                                               // 0x0050 (size: 0x4)
    bool bAffectedByTimeDilation;                                                     // 0x0054 (size: 0x1)

}; // Size: 0x58

class UPlayerMappableInputConfig : public UPrimaryDataAsset
{
    FName ConfigName;                                                                 // 0x0030 (size: 0x8)
    FText ConfigDisplayName;                                                          // 0x0038 (size: 0x10)
    bool bIsDeprecated;                                                               // 0x0048 (size: 0x1)
    class UObject* MetaData;                                                          // 0x0050 (size: 0x8)
    TMap<UInputMappingContext*, int32> Contexts;                                      // 0x0058 (size: 0x50)

    void ResetToDefault();
    bool IsDeprecated();
    TArray<FEnhancedActionKeyMapping> GetPlayerMappableKeys();
    class UObject* GetMetadata();
    TMap<UInputMappingContext*, int32> GetMappingContexts();
    FEnhancedActionKeyMapping GetMappingByName(const FName MappingName);
    TArray<FEnhancedActionKeyMapping> GetKeysBoundToAction(const class UInputAction* InAction);
    FText GetDisplayName();
    FName GetConfigName();
}; // Size: 0xA8

class UPlayerMappableKeySettings : public UObject
{
    class UObject* MetaData;                                                          // 0x0028 (size: 0x8)
    FName Name;                                                                       // 0x0030 (size: 0x8)
    FText DisplayName;                                                                // 0x0038 (size: 0x10)
    FText DisplayCategory;                                                            // 0x0048 (size: 0x10)
    FGameplayTagContainer SupportedKeyProfiles;                                       // 0x0058 (size: 0x20)
    TArray<FString> SupportedKeyProfileIds;                                           // 0x0078 (size: 0x10)

}; // Size: 0x88

#endif

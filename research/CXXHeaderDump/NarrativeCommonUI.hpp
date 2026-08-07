#ifndef UE4SS_SDK_NarrativeCommonUI_HPP
#define UE4SS_SDK_NarrativeCommonUI_HPP

struct FInputActionBindingHandle
{
}; // Size: 0x4

class UNarrativeActivatableWidget : public UCommonActivatableWidget
{

    void UnregisterBinding(FInputActionBindingHandle BindingHandle);
    void UnregisterAllBindings();
    void SetBindingDisplayName(FInputActionBindingHandle BindingHandle, FText NewDisplayName);
    void RegisterBinding(FDataTableRowHandle InputAction, const FRegisterBindingCallback& Callback, FInputActionBindingHandle& BindingHandle, FText OverrideDisplayName, const bool bShouldDisplayInActionBar);
}; // Size: 0x420

class UNarrativeCommonHUD : public UCommonUserWidget
{

    void ShowNotification(const FText& NotificationText, const float Duration);
}; // Size: 0x2E8

class UNarrativeCommonUIFunctionLibrary : public UBlueprintFunctionLibrary
{

    void PushHUDNotification(const class UObject* WorldContextObject, FText Message, const float Duration);
}; // Size: 0x28

class UNarrativeCommonUISubsystem : public UGameInstanceSubsystem
{
    class UNarrativeCommonHUD* CommonHUD;                                             // 0x0030 (size: 0x8)

    void ShowNotification(const FText& NotificationText, const float Duration);
}; // Size: 0x38

#endif

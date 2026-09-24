#ifndef UE4SS_SDK_WBP_NarrativeActivatableWidget_HPP
#define UE4SS_SDK_WBP_NarrativeActivatableWidget_HPP

class UWBP_NarrativeActivatableWidget_C : public UNarrativeActivatableWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x04B8 (size: 0x8)
    bool DeactivateOnBack;                                                            // 0x04C0 (size: 0x1)
    bool AutoFocusDesiredTargetOnActivate;                                            // 0x04C1 (size: 0x1)

    void RegisterActions();
    void HandleFocus();
    bool BP_OnHandleBackAction();
    void BP_OnActivated();
    void BP_OnDeactivated();
    void Construct();
    void ExecuteUbergraph_WBP_NarrativeActivatableWidget(int32 EntryPoint);
}; // Size: 0x4C2

#endif

#ifndef UE4SS_SDK_WBP_NarrativeMenu_HPP
#define UE4SS_SDK_WBP_NarrativeMenu_HPP

class UWBP_NarrativeMenu_C : public UWBP_NarrativeActivatableWidget_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x04C8 (size: 0x8)
    bool ShowCursorAndFocusUI;                                                        // 0x04D0 (size: 0x1)
    class UWBP_NarrativeHUD_C* OwningHUD;                                             // 0x04D8 (size: 0x8)
    bool PauseGame?;                                                                  // 0x04E0 (size: 0x1)
    float CustomMenuDilation;                                                         // 0x04E4 (size: 0x4)

    void Remove Pause Game();
    void Handle Pause Game();
    void Remove Time Dilation();
    void Handle Time Dilation();
    void Construct();
    void Destruct();
    void ExecuteUbergraph_WBP_NarrativeMenu(int32 EntryPoint);
}; // Size: 0x4E8

#endif

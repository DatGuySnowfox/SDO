#ifndef UE4SS_SDK_WBP_NarrativeMenu_HPP
#define UE4SS_SDK_WBP_NarrativeMenu_HPP

class UWBP_NarrativeMenu_C : public UWBP_NarrativeActivatableWidget_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    bool ShowCursorAndFocusUI;                                                        // 0x0438 (size: 0x1)
    class UWBP_NarrativeHUD_C* OwningHUD;                                             // 0x0440 (size: 0x8)

    void Construct();
    void Destruct();
    void ExecuteUbergraph_WBP_NarrativeMenu(int32 EntryPoint);
}; // Size: 0x448

#endif

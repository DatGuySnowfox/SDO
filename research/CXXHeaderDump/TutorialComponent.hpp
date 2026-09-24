#ifndef UE4SS_SDK_TutorialComponent_HPP
#define UE4SS_SDK_TutorialComponent_HPP

class UTutorialComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00D0 (size: 0x8)
    class UTutorialPopupWidget_C* TutorialUI;                                         // 0x00D8 (size: 0x8)
    FTutorialComponent_CNewTutorialMessage NewTutorialMessage;                        // 0x00E0 (size: 0x10)
    void NewTutorialMessage(FText Text, double Duration);
    FTutorialComponent_CRemoveCurrentTutorialMessage RemoveCurrentTutorialMessage;    // 0x00F0 (size: 0x10)
    void RemoveCurrentTutorialMessage();

    void On_RemoveCurrentTutorialMessage();
    void On_NewTutorialMessage(FText Text, double Duration);
    void LoadComponent();
    void ExecuteUbergraph_TutorialComponent(int32 EntryPoint);
    void RemoveCurrentTutorialMessage__DelegateSignature();
    void NewTutorialMessage__DelegateSignature(FText Text, double Duration);
}; // Size: 0x100

#endif

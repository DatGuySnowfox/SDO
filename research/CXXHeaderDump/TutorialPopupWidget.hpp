#ifndef UE4SS_SDK_TutorialPopupWidget_HPP
#define UE4SS_SDK_TutorialPopupWidget_HPP

class UTutorialPopupWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* TextBlock;                                                      // 0x0348 (size: 0x8)
    class UImage* Image_73;                                                           // 0x0350 (size: 0x8)
    class UWidgetAnimation* Anim;                                                     // 0x0358 (size: 0x8)

    void RemoveTutorialMessage();
    void ReplaceInputKey(FText Text, FText& ToText);
    void NewTutorialText(FText Text, float Duration);
    void UpdateCurrentTutorialText(FText Text);
    void RemoveCurrentTutorialText();
    void ExecuteUbergraph_TutorialPopupWidget(int32 EntryPoint);
}; // Size: 0x360

#endif

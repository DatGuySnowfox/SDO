#ifndef UE4SS_SDK_DaysSurvivedWidget_HPP
#define UE4SS_SDK_DaysSurvivedWidget_HPP

class UDaysSurvivedWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Name;                                                           // 0x0348 (size: 0x8)
    class UWidgetAnimation* FadeOut;                                                  // 0x0350 (size: 0x8)
    FString FullText;                                                                 // 0x0358 (size: 0x10)
    FString DisplayText;                                                              // 0x0368 (size: 0x10)

    void Event_Survived();
    void FadeOutFinished();
    void ExecuteUbergraph_DaysSurvivedWidget(int32 EntryPoint);
}; // Size: 0x378

#endif

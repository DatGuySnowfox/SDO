#ifndef UE4SS_SDK_DaysSurvivedWidget_HPP
#define UE4SS_SDK_DaysSurvivedWidget_HPP

class UDaysSurvivedWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* FadeOut;                                                  // 0x02C8 (size: 0x8)
    class UTextBlock* Name;                                                           // 0x02D0 (size: 0x8)
    FString FullText;                                                                 // 0x02D8 (size: 0x10)
    FString DisplayText;                                                              // 0x02E8 (size: 0x10)

    void Event_Survived();
    void FadeOutFinished();
    void ExecuteUbergraph_DaysSurvivedWidget(int32 EntryPoint);
}; // Size: 0x2F8

#endif

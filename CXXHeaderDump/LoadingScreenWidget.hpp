#ifndef UE4SS_SDK_LoadingScreenWidget_HPP
#define UE4SS_SDK_LoadingScreenWidget_HPP

class ULoadingScreenWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* FadeOutAnimation;                                         // 0x02C8 (size: 0x8)
    class UWidgetAnimation* FadeInAnimation;                                          // 0x02D0 (size: 0x8)
    class UImage* Background;                                                         // 0x02D8 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x02E0 (size: 0x8)
    class UThrobber* Throbber;                                                        // 0x02E8 (size: 0x8)
    FLoadingScreenWidget_CFadeInFinished FadeInFinished;                              // 0x02F0 (size: 0x10)
    void FadeInFinished();
    FLoadingScreenWidget_CFadeOutFinished FadeOutFinished;                            // 0x0300 (size: 0x10)
    void FadeOutFinished();
    TArray<class UTexture2D*> Images;                                                 // 0x0310 (size: 0x10)
    TArray<FText> Tips;                                                               // 0x0320 (size: 0x10)

    void Random Image / Tip();
    void FadeOut(const FFadeOutFadeFinished& FadeFinished);
    void FadeIn(const FFadeInFadeFinished& FadeFinished);
    void OnInitialized();
    void CustomEvent_0();
    void CustomEvent_1();
    void Construct();
    void ExecuteUbergraph_LoadingScreenWidget(int32 EntryPoint);
    void FadeOutFinished__DelegateSignature();
    void FadeInFinished__DelegateSignature();
}; // Size: 0x330

#endif

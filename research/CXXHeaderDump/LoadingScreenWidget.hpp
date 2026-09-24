#ifndef UE4SS_SDK_LoadingScreenWidget_HPP
#define UE4SS_SDK_LoadingScreenWidget_HPP

class ULoadingScreenWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UThrobber* Throbber;                                                        // 0x0348 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x0350 (size: 0x8)
    class UImage* Background;                                                         // 0x0358 (size: 0x8)
    class UWidgetAnimation* FadeInAnimation;                                          // 0x0360 (size: 0x8)
    class UWidgetAnimation* FadeOutAnimation;                                         // 0x0368 (size: 0x8)
    FLoadingScreenWidget_CFadeInFinished FadeInFinished;                              // 0x0370 (size: 0x10)
    void FadeInFinished();
    FLoadingScreenWidget_CFadeOutFinished FadeOutFinished;                            // 0x0380 (size: 0x10)
    void FadeOutFinished();
    TArray<UTexture2D*> Images;                                                       // 0x0390 (size: 0x10)
    TArray<FText> Tips;                                                               // 0x03A0 (size: 0x10)

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
}; // Size: 0x3B0

#endif

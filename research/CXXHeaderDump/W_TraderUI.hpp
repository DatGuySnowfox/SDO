#ifndef UE4SS_SDK_W_TraderUI_HPP
#define UE4SS_SDK_W_TraderUI_HPP

class UW_TraderUI_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* MainContainer;                                             // 0x0388 (size: 0x8)
    class UTextBlock* CurrencyText;                                                   // 0x0390 (size: 0x8)

    FText GetText();
    void PreInitSpecialContainer();
    void Construct();
    void ExecuteUbergraph_W_TraderUI(int32 EntryPoint);
}; // Size: 0x398

#endif

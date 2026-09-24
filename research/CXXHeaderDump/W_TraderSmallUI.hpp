#ifndef UE4SS_SDK_W_TraderSmallUI_HPP
#define UE4SS_SDK_W_TraderSmallUI_HPP

class UW_TraderSmallUI_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* MainContainer;                                             // 0x0388 (size: 0x8)
    class UTextBlock* CurrencyText;                                                   // 0x0390 (size: 0x8)

    FText GetText();
    void PreInitSpecialContainer();
    void Construct();
    void ExecuteUbergraph_W_TraderSmallUI(int32 EntryPoint);
}; // Size: 0x398

#endif

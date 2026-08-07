#ifndef UE4SS_SDK_AIHealthBarUI_HPP
#define UE4SS_SDK_AIHealthBarUI_HPP

class UAIHealthBarUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* AIBox;                                                             // 0x02C8 (size: 0x8)
    class UImage* Boss;                                                               // 0x02D0 (size: 0x8)
    class UProgressBarLinear_C* HealthBar;                                            // 0x02D8 (size: 0x8)
    class UTextBlock* HealthText;                                                     // 0x02E0 (size: 0x8)
    class UTextBlock* Name;                                                           // 0x02E8 (size: 0x8)
    FAIHealthBarUI_CVisible Visible;                                                  // 0x02F0 (size: 0x10)
    void Visible();
    FAIHealthBarUI_CHidden Hidden;                                                    // 0x0300 (size: 0x10)
    void Hidden();

    void UpdateHealth(double Health, double MaxHealth);
    void ExecuteUbergraph_AIHealthBarUI(int32 EntryPoint);
    void Hidden__DelegateSignature();
    void Visible__DelegateSignature();
}; // Size: 0x310

#endif

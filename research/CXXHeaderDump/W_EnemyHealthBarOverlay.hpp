#ifndef UE4SS_SDK_W_EnemyHealthBarOverlay_HPP
#define UE4SS_SDK_W_EnemyHealthBarOverlay_HPP

class UW_EnemyHealthBarOverlay_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Name;                                                           // 0x0348 (size: 0x8)
    class UTextBlock* HealthText;                                                     // 0x0350 (size: 0x8)
    class UProgressBarLinear_C* HealthBar;                                            // 0x0358 (size: 0x8)
    class UImage* Boss;                                                               // 0x0360 (size: 0x8)
    class UBorder* AIBox;                                                             // 0x0368 (size: 0x8)
    FW_EnemyHealthBarOverlay_CVisible Visible;                                        // 0x0370 (size: 0x10)
    void Visible();
    FW_EnemyHealthBarOverlay_CHidden Hidden;                                          // 0x0380 (size: 0x10)
    void Hidden();

    void UpdateHealth(double Health, double MaxHealth);
    void ExecuteUbergraph_W_EnemyHealthBarOverlay(int32 EntryPoint);
    void Hidden__DelegateSignature();
    void Visible__DelegateSignature();
}; // Size: 0x390

#endif

#ifndef UE4SS_SDK_BuildableObjectHealth_HPP
#define UE4SS_SDK_BuildableObjectHealth_HPP

class UBuildableObjectHealth_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* HealthText;                                                     // 0x0348 (size: 0x8)
    class UProgressBarLinear_C* HealthBar;                                            // 0x0350 (size: 0x8)
    FBuildableObjectHealth_CVisible Visible;                                          // 0x0358 (size: 0x10)
    void Visible();
    FBuildableObjectHealth_CHidden Hidden;                                            // 0x0368 (size: 0x10)
    void Hidden();

    void UpdateHealth(int32 Health, int32 MaxHealth);
    void ExecuteUbergraph_BuildableObjectHealth(int32 EntryPoint);
    void Hidden__DelegateSignature();
    void Visible__DelegateSignature();
}; // Size: 0x378

#endif

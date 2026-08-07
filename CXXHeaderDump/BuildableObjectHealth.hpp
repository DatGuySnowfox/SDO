#ifndef UE4SS_SDK_BuildableObjectHealth_HPP
#define UE4SS_SDK_BuildableObjectHealth_HPP

class UBuildableObjectHealth_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UProgressBarLinear_C* HealthBar;                                            // 0x02C8 (size: 0x8)
    class UTextBlock* HealthText;                                                     // 0x02D0 (size: 0x8)
    FBuildableObjectHealth_CVisible Visible;                                          // 0x02D8 (size: 0x10)
    void Visible();
    FBuildableObjectHealth_CHidden Hidden;                                            // 0x02E8 (size: 0x10)
    void Hidden();

    void UpdateHealth(int32 Health, int32 MaxHealth);
    void ExecuteUbergraph_BuildableObjectHealth(int32 EntryPoint);
    void Hidden__DelegateSignature();
    void Visible__DelegateSignature();
}; // Size: 0x2F8

#endif

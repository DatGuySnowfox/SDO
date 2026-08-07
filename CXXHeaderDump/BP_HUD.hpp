#ifndef UE4SS_SDK_BP_HUD_HPP
#define UE4SS_SDK_BP_HUD_HPP

class UBP_HUD_C : public UUserWidget
{
    class UProgressBar* HealthBar;                                                    // 0x02C0 (size: 0x8)
    class UImage* Image_0;                                                            // 0x02C8 (size: 0x8)
    class ABP_ExampleCharacter_C* Character;                                          // 0x02D0 (size: 0x8)

    float Set Health Bar();
}; // Size: 0x2D8

#endif

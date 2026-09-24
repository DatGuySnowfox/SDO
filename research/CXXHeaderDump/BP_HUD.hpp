#ifndef UE4SS_SDK_BP_HUD_HPP
#define UE4SS_SDK_BP_HUD_HPP

class UBP_HUD_C : public UUserWidget
{
    class UImage* Image_0;                                                            // 0x0340 (size: 0x8)
    class UProgressBar* HealthBar;                                                    // 0x0348 (size: 0x8)
    class ABP_ExampleCharacter_C* Character;                                          // 0x0350 (size: 0x8)

    float Set Health Bar();
}; // Size: 0x358

#endif

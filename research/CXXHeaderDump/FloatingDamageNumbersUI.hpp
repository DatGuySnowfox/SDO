#ifndef UE4SS_SDK_FloatingDamageNumbersUI_HPP
#define UE4SS_SDK_FloatingDamageNumbersUI_HPP

class UFloatingDamageNumbersUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* DmgText;                                                        // 0x0348 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x0350 (size: 0x8)
    double Damage;                                                                    // 0x0358 (size: 0x8)
    FLinearColor Colour;                                                              // 0x0360 (size: 0x10)
    FVector2D InitialPosition;                                                        // 0x0370 (size: 0x10)
    FVector2D FinalPosition;                                                          // 0x0380 (size: 0x10)

    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void Construct();
    void ExecuteUbergraph_FloatingDamageNumbersUI(int32 EntryPoint);
}; // Size: 0x390

#endif

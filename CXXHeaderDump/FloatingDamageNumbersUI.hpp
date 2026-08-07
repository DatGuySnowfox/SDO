#ifndef UE4SS_SDK_FloatingDamageNumbersUI_HPP
#define UE4SS_SDK_FloatingDamageNumbersUI_HPP

class UFloatingDamageNumbersUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x02C8 (size: 0x8)
    class UCanvasPanel* CanvasPanel;                                                  // 0x02D0 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x02D8 (size: 0x8)
    FVector2D Position;                                                               // 0x02E0 (size: 0x10)
    FVector2D FinalPosition;                                                          // 0x02F0 (size: 0x10)
    double Damage;                                                                    // 0x0300 (size: 0x8)
    FLinearColor Colour;                                                              // 0x0308 (size: 0x10)

    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_FloatingDamageNumbersUI(int32 EntryPoint);
}; // Size: 0x318

#endif

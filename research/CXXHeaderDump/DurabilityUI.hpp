#ifndef UE4SS_SDK_DurabilityUI_HPP
#define UE4SS_SDK_DurabilityUI_HPP

class UDurabilityUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* DurabilityIcon;                                                     // 0x0348 (size: 0x8)
    class UProgressBarCircular_C* DurabilityBar;                                      // 0x0350 (size: 0x8)
    class UTexture2D* Texture;                                                        // 0x0358 (size: 0x8)
    FLinearColor Fill Color Current;                                                  // 0x0360 (size: 0x10)

    void Construct();
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_DurabilityUI(int32 EntryPoint);
}; // Size: 0x370

#endif

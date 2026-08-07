#ifndef UE4SS_SDK_DurabilityUI_HPP
#define UE4SS_SDK_DurabilityUI_HPP

class UDurabilityUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UProgressBarCircular_C* DurabilityBar;                                      // 0x02C8 (size: 0x8)
    class UImage* DurabilityIcon;                                                     // 0x02D0 (size: 0x8)
    class UTexture2D* Texture;                                                        // 0x02D8 (size: 0x8)
    FLinearColor Fill Color Current;                                                  // 0x02E0 (size: 0x10)

    void Construct();
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_DurabilityUI(int32 EntryPoint);
}; // Size: 0x2F0

#endif

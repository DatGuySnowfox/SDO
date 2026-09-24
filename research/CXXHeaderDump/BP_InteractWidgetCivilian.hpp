#ifndef UE4SS_SDK_BP_InteractWidgetCivilian_HPP
#define UE4SS_SDK_BP_InteractWidgetCivilian_HPP

class UBP_InteractWidgetCivilian_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* txt;                                                            // 0x0348 (size: 0x8)
    FLinearColor DefColor;                                                            // 0x0350 (size: 0x10)

    void SetName(FText ActionName, FText ItemName);
    void ExecuteUbergraph_BP_InteractWidgetCivilian(int32 EntryPoint);
}; // Size: 0x360

#endif

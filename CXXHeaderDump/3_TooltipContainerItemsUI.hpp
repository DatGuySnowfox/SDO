#ifndef UE4SS_SDK_3_TooltipContainerItemsUI_HPP
#define UE4SS_SDK_3_TooltipContainerItemsUI_HPP

class U3_TooltipContainerItemsUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UVerticalBox* VerticalBox;                                                  // 0x02C8 (size: 0x8)
    F3_TooltipContainerItemsUI_CClicked Clicked;                                      // 0x02D0 (size: 0x10)
    void Clicked();
    TArray<class UWidget*> ItemsW;                                                    // 0x02E0 (size: 0x10)
    TArray<FText> ItemNames;                                                          // 0x02F0 (size: 0x10)
    TArray<int32> ItemCount;                                                          // 0x0300 (size: 0x10)

    void Construct();
    void CreateButton(FText Name, int32 Count);
    void ExecuteUbergraph_3_TooltipContainerItemsUI(int32 EntryPoint);
    void Clicked__DelegateSignature();
}; // Size: 0x310

#endif

#ifndef UE4SS_SDK_3_TooltipContainerItemsUI_HPP
#define UE4SS_SDK_3_TooltipContainerItemsUI_HPP

class U3_TooltipContainerItemsUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UVerticalBox* VerticalBox;                                                  // 0x0348 (size: 0x8)
    F3_TooltipContainerItemsUI_CClicked Clicked;                                      // 0x0350 (size: 0x10)
    void Clicked();
    TArray<UWidget*> ItemsW;                                                          // 0x0360 (size: 0x10)
    TArray<FText> ItemNames;                                                          // 0x0370 (size: 0x10)
    TArray<int32> ItemCount;                                                          // 0x0380 (size: 0x10)

    void Construct();
    void CreateButton(FText Name, int32 Count);
    void ExecuteUbergraph_3_TooltipContainerItemsUI(int32 EntryPoint);
    void Clicked__DelegateSignature();
}; // Size: 0x390

#endif

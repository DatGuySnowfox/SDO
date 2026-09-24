#ifndef UE4SS_SDK_4_ContainerItemsTitle_HPP
#define UE4SS_SDK_4_ContainerItemsTitle_HPP

class U4_ContainerItemsTitle_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* ItemName;                                                       // 0x0348 (size: 0x8)
    F4_ContainerItemsTitle_CClicked Clicked;                                          // 0x0350 (size: 0x10)
    void Clicked();
    FText Text;                                                                       // 0x0360 (size: 0x10)
    int32 Count;                                                                      // 0x0370 (size: 0x4)

    void Construct();
    void ExecuteUbergraph_4_ContainerItemsTitle(int32 EntryPoint);
    void Clicked__DelegateSignature();
}; // Size: 0x374

#endif

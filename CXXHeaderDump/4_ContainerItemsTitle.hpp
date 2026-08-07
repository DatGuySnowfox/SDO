#ifndef UE4SS_SDK_4_ContainerItemsTitle_HPP
#define UE4SS_SDK_4_ContainerItemsTitle_HPP

class U4_ContainerItemsTitle_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* ItemName;                                                       // 0x02C8 (size: 0x8)
    F4_ContainerItemsTitle_CClicked Clicked;                                          // 0x02D0 (size: 0x10)
    void Clicked();
    FText Text;                                                                       // 0x02E0 (size: 0x18)
    int32 Count;                                                                      // 0x02F8 (size: 0x4)

    void Construct();
    void ExecuteUbergraph_4_ContainerItemsTitle(int32 EntryPoint);
    void Clicked__DelegateSignature();
}; // Size: 0x2FC

#endif

#ifndef UE4SS_SDK_1_ContainersListUI_HPP
#define UE4SS_SDK_1_ContainersListUI_HPP

class U1_ContainersListUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UVerticalBox* VerticalBox;                                                  // 0x0348 (size: 0x8)
    F1_ContainersListUI_CClicked Clicked;                                             // 0x0350 (size: 0x10)
    void Clicked();

    void Construct();
    void CustomEvent();
    void ExecuteUbergraph_1_ContainersListUI(int32 EntryPoint);
    void Clicked__DelegateSignature();
}; // Size: 0x360

#endif

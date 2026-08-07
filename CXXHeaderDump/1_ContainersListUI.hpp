#ifndef UE4SS_SDK_1_ContainersListUI_HPP
#define UE4SS_SDK_1_ContainersListUI_HPP

class U1_ContainersListUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UVerticalBox* VerticalBox;                                                  // 0x02C8 (size: 0x8)
    F1_ContainersListUI_CClicked Clicked;                                             // 0x02D0 (size: 0x10)
    void Clicked();

    void Construct();
    void CustomEvent();
    void ExecuteUbergraph_1_ContainersListUI(int32 EntryPoint);
    void Clicked__DelegateSignature();
}; // Size: 0x2E0

#endif

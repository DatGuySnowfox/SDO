#ifndef UE4SS_SDK_RadioTextUI_HPP
#define UE4SS_SDK_RadioTextUI_HPP

class URadioTextUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x02C8 (size: 0x8)
    FStruct_Radio RadioStruct;                                                        // 0x02D0 (size: 0x20)
    int32 Row;                                                                        // 0x02F0 (size: 0x4)
    class ABP_Radio_Text_C* RadioRef;                                                 // 0x02F8 (size: 0x8)

    void Construct();
    void ScrollText();
    void ExecuteUbergraph_RadioTextUI(int32 EntryPoint);
}; // Size: 0x300

#endif

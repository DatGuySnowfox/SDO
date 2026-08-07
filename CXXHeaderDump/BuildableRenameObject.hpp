#ifndef UE4SS_SDK_BuildableRenameObject_HPP
#define UE4SS_SDK_BuildableRenameObject_HPP

class UBuildableRenameObject_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButtonWidget_C* CancelButton;                                              // 0x02C8 (size: 0x8)
    class UButtonWidget_C* ConfirmButton;                                             // 0x02D0 (size: 0x8)
    class UEditableText* EditableText;                                                // 0x02D8 (size: 0x8)
    class AActor* ActorRef;                                                           // 0x02E0 (size: 0x8)

    void Cancel();
    void Confirm();
    void Construct();
    void ExecuteUbergraph_BuildableRenameObject(int32 EntryPoint);
}; // Size: 0x2E8

#endif

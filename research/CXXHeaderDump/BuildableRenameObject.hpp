#ifndef UE4SS_SDK_BuildableRenameObject_HPP
#define UE4SS_SDK_BuildableRenameObject_HPP

class UBuildableRenameObject_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UEditableText* EditableText;                                                // 0x0348 (size: 0x8)
    class UButtonWidget_C* ConfirmButton;                                             // 0x0350 (size: 0x8)
    class UButtonWidget_C* CancelButton;                                              // 0x0358 (size: 0x8)
    class AActor* ActorRef;                                                           // 0x0360 (size: 0x8)

    void Cancel();
    void Confirm();
    void Construct();
    void ExecuteUbergraph_BuildableRenameObject(int32 EntryPoint);
}; // Size: 0x368

#endif

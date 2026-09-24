#ifndef UE4SS_SDK_VehicleButton_HPP
#define UE4SS_SDK_VehicleButton_HPP

class UVehicleButton_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* TextBlock_31;                                                   // 0x0348 (size: 0x8)
    class UButton* Button_41;                                                         // 0x0350 (size: 0x8)
    TSubclassOf<class AActor> Item;                                                   // 0x0358 (size: 0x8)
    FText VehicleName;                                                                // 0x0360 (size: 0x10)

    void BndEvt__ItemButton_Button_41_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_VehicleButton(int32 EntryPoint);
}; // Size: 0x370

#endif

#ifndef UE4SS_SDK_VehicleButton_HPP
#define UE4SS_SDK_VehicleButton_HPP

class UVehicleButton_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_41;                                                         // 0x02C8 (size: 0x8)
    class UTextBlock* TextBlock_31;                                                   // 0x02D0 (size: 0x8)
    TSubclassOf<class AActor> Item;                                                   // 0x02D8 (size: 0x8)
    FText VehicleName;                                                                // 0x02E0 (size: 0x18)

    void BndEvt__ItemButton_Button_41_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_VehicleButton(int32 EntryPoint);
}; // Size: 0x2F8

#endif

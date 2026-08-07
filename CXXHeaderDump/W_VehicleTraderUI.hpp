#ifndef UE4SS_SDK_W_VehicleTraderUI_HPP
#define UE4SS_SDK_W_VehicleTraderUI_HPP

class UW_VehicleTraderUI_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UJSIContainer_C* MainContainer;                                             // 0x0308 (size: 0x8)
    class UScrollBox* ScrollBox_Vehicle;                                              // 0x0310 (size: 0x8)

    void GetDataTable_Vehicles(class UPanelWidget* ScrollBox);
    void Construct();
    void PreInitSpecialContainer();
    void ExecuteUbergraph_W_VehicleTraderUI(int32 EntryPoint);
}; // Size: 0x318

#endif

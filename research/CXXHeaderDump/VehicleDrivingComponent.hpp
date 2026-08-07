#ifndef UE4SS_SDK_VehicleDrivingComponent_HPP
#define UE4SS_SDK_VehicleDrivingComponent_HPP

class UVehicleDrivingComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    class ABP_VehicleMaster_C* Vehicle;                                               // 0x00C0 (size: 0x8)
    FVehicleDrivingComponent_CDeathInVehicle DeathInVehicle;                          // 0x00C8 (size: 0x10)
    void DeathInVehicle();

    void GetName(FString& Name);
    void InpActEvt_IA_VehicleHorn_K2Node_EnhancedInputActionEvent_13(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleHandbrake_K2Node_EnhancedInputActionEvent_12(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleHandbrake_K2Node_EnhancedInputActionEvent_11(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleFlip_K2Node_EnhancedInputActionEvent_10(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_Flashlight_K2Node_EnhancedInputActionEvent_9(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleRadio_K2Node_EnhancedInputActionEvent_8(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleForward_K2Node_EnhancedInputActionEvent_7(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleForward_K2Node_EnhancedInputActionEvent_6(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleLeft_K2Node_EnhancedInputActionEvent_5(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleLeft_K2Node_EnhancedInputActionEvent_4(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleRight_K2Node_EnhancedInputActionEvent_3(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleRight_K2Node_EnhancedInputActionEvent_2(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleBackwards_K2Node_EnhancedInputActionEvent_1(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void InpActEvt_IA_VehicleBackwards_K2Node_EnhancedInputActionEvent_0(FInputActionValue ActionValue, float ElapsedTime, float TriggeredTime, const class UInputAction* SourceAction);
    void LoadComponent();
    void Death();
    void ExecuteUbergraph_VehicleDrivingComponent(int32 EntryPoint);
    void DeathInVehicle__DelegateSignature();
}; // Size: 0xD8

#endif

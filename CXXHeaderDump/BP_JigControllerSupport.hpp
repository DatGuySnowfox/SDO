#ifndef UE4SS_SDK_BP_JigControllerSupport_HPP
#define UE4SS_SDK_BP_JigControllerSupport_HPP

class UBP_JigControllerSupport_C : public UActorComponent
{
    bool ControllerEnabled;                                                           // 0x00A0 (size: 0x1)

    void IsControllerEnabled(bool& ControllerEnabled);
}; // Size: 0xA1

#endif

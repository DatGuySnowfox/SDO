#ifndef UE4SS_SDK_Save_General_HPP
#define UE4SS_SDK_Save_General_HPP

class USave_General_C : public UEMSCustomSaveGame
{
    FUDS_and_UDW_State WeatherSystem;                                                 // 0x0050 (size: 0x200)

    void Save_General();
}; // Size: 0x250

#endif

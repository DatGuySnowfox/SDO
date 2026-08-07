#ifndef UE4SS_SDK_BPI_UI_HPP
#define UE4SS_SDK_BPI_UI_HPP

class IBPI_UI_C : public IInterface
{

    void ShowNightVisionDurability(bool Show?);
    void GetNightVisionDurability(double Dur);
    void ShowFlashlightDurability(bool Show?);
    void GetFlashlightDurability(double Dur);
    void ShowRespiratorDurability(bool Show?);
    void GetRespiratorDurability(double Dur);
}; // Size: 0x28

#endif

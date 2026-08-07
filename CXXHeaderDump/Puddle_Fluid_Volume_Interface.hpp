#ifndef UE4SS_SDK_Puddle_Fluid_Volume_Interface_HPP
#define UE4SS_SDK_Puddle_Fluid_Volume_Interface_HPP

class IPuddle_Fluid_Volume_Interface_C : public IInterface
{

    void Interact with Puddle(FVector Location, double Diameter, double Speed, FVector Last Trace Location, double Ground Height, double& Effective Depth);
}; // Size: 0x28

#endif

#ifndef UE4SS_SDK_Temperature_Volume_Interface_HPP
#define UE4SS_SDK_Temperature_Volume_Interface_HPP

class ITemperature_Volume_Interface_C : public IInterface
{

    void Query Temperature Volume(FVector Sample Location, double& Offset, double& Interior Temp, double& Interior Alpha);
}; // Size: 0x28

#endif

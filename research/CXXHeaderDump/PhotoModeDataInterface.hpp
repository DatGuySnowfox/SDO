#ifndef UE4SS_SDK_PhotoModeDataInterface_HPP
#define UE4SS_SDK_PhotoModeDataInterface_HPP

class IPhotoModeDataInterface_C : public IInterface
{

    void TabToOpen(int32 TabIndex, class UCommonButtonTab_C* CommonButtonPressed);
    void SendPhotoData(FPhotosData PhotoData);
}; // Size: 0x28

#endif

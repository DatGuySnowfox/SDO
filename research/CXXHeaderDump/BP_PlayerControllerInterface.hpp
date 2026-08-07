#ifndef UE4SS_SDK_BP_PlayerControllerInterface_HPP
#define UE4SS_SDK_BP_PlayerControllerInterface_HPP

class IBP_PlayerControllerInterface_C : public IInterface
{

    void CameraShake(TSubclassOf<class ULegacyCameraShake> CameraShake, bool Force?);
}; // Size: 0x28

#endif

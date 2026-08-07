#ifndef UE4SS_SDK_BP_Zombie_Dog_HPP
#define UE4SS_SDK_BP_Zombie_Dog_HPP

class ABP_Zombie_Dog_C : public ABP_MasterZombie_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x08B0 (size: 0x8)

    void GetMarker(class UTexture2D*& Texture, FLinearColor& Color, double& MarkerOffset);
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_Zombie_Dog(int32 EntryPoint);
}; // Size: 0x8B8

#endif

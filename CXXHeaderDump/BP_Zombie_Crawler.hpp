#ifndef UE4SS_SDK_BP_Zombie_Crawler_HPP
#define UE4SS_SDK_BP_Zombie_Crawler_HPP

class ABP_Zombie_Crawler_C : public ABP_MasterZombie_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x08B0 (size: 0x8)

    void GetMarker(class UTexture2D*& Texture, FLinearColor& Color, double& MarkerOffset);
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_Zombie_Crawler(int32 EntryPoint);
}; // Size: 0x8B8

#endif

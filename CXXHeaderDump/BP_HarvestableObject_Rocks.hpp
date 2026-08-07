#ifndef UE4SS_SDK_BP_HarvestableObject_Rocks_HPP
#define UE4SS_SDK_BP_HarvestableObject_Rocks_HPP

class ABP_HarvestableObject_Rocks_C : public ABP_HarvestableObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D8 (size: 0x8)

    void SetMaxHits();
    void UserConstructionScript();
    void HitHarvestableObject(FVector HitLocation, int32 Hits);
    void MC_PlaySound(FVector Location);
    void Svr_PlaySound(FVector Location);
    void Svr_SetMaxHits();
    void ExecuteUbergraph_BP_HarvestableObject_Rocks(int32 EntryPoint);
}; // Size: 0x2E0

#endif

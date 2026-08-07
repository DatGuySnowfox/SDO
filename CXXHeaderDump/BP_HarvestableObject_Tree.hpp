#ifndef UE4SS_SDK_BP_HarvestableObject_Tree_HPP
#define UE4SS_SDK_BP_HarvestableObject_Tree_HPP

class ABP_HarvestableObject_Tree_C : public ABP_HarvestableObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D8 (size: 0x8)
    float FallOverTimeline_FallOverAlpha_8E6A479F46FC05F3F36446A1B2EB299C;            // 0x02E0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> FallOverTimeline__Direction_8E6A479F46FC05F3F36446A1B2EB299C; // 0x02E4 (size: 0x1)
    class UTimelineComponent* FallOverTimeline;                                       // 0x02E8 (size: 0x8)

    void SetMaxHits();
    void UserConstructionScript();
    void FallOverTimeline__FinishedFunc();
    void FallOverTimeline__UpdateFunc();
    void HitHarvestableObject(FVector HitLocation, int32 Hits);
    void MC_FallOverEffect(FVector CharacterLocation);
    void MC_PlaySound(FVector Location);
    void Svr_PlaySound(FVector Location);
    void Svr_FallOverEffect(FVector CharacterLocation);
    void Svr_SetMaxHits();
    void ExecuteUbergraph_BP_HarvestableObject_Tree(int32 EntryPoint);
}; // Size: 0x2F0

#endif

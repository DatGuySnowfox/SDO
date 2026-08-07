#ifndef UE4SS_SDK_RandomContainerItem_HPP
#define UE4SS_SDK_RandomContainerItem_HPP

struct FRandomContainerItem
{
    class UJigsawItem_DataAsset_C* ItemID_41_8C5E69274AB445604263E4A2B3091BAD;        // 0x0000 (size: 0x8)
    double SpawnChance_9_0A1811814580181DD09AE78500C25A28;                            // 0x0008 (size: 0x8)
    TEnumAsByte<Enum_LootType::Type> LootType_44_CFD868CE4BEBE931C8DD4693D2E52CF4;    // 0x0010 (size: 0x1)
    int32 RandomCountMin_17_2CF035DB43DEE23801AD29A29560BDA9;                         // 0x0014 (size: 0x4)
    int32 RandomCountMax_18_C37B88104F52B02AA216B5912518580D;                         // 0x0018 (size: 0x4)
    int32 Index_37_B46F3993454026F67AFD6D936119D0F6;                                  // 0x001C (size: 0x4)
    bool PreferRotated?_39_71AEF07C444B177A20CAF4A74260B9CD;                          // 0x0020 (size: 0x1)
    double PriceOverride_29_306CC4B441051ADD17999E8D00D06A7F;                         // 0x0028 (size: 0x8)
    FS_RandomStatsConfig RandomStats_34_54BB3BF64CE6FC45D1F1C1AA417937A4;             // 0x0030 (size: 0x18)
    TArray<FDefaultAttachments> InContainerItem_26_8B73BDAE473217274FE336B3D2EB2407;  // 0x0048 (size: 0x10)

}; // Size: 0x58

#endif

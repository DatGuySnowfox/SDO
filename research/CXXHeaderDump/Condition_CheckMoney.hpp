#ifndef UE4SS_SDK_Condition_CheckMoney_HPP
#define UE4SS_SDK_Condition_CheckMoney_HPP

class UCondition_CheckMoney_C : public UNarrativeCondition
{
    int32 Price;                                                                      // 0x0030 (size: 0x4)
    class UJigsawItem_DataAsset_C* Currency;                                          // 0x0038 (size: 0x8)

    bool CheckCondition(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x40

#endif

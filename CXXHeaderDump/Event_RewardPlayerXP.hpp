#ifndef UE4SS_SDK_Event_RewardPlayerXP_HPP
#define UE4SS_SDK_Event_RewardPlayerXP_HPP

class UEvent_RewardPlayerXP_C : public UNarrativeEvent
{
    double Amount;                                                                    // 0x0030 (size: 0x8)

    void ExecuteEvent(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x38

#endif

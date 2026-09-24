#ifndef UE4SS_SDK_Quest_Event_RewardPlayerXP_HPP
#define UE4SS_SDK_Quest_Event_RewardPlayerXP_HPP

class UQuest_Event_RewardPlayerXP_C : public UNarrativeEvent
{
    double Amount;                                                                    // 0x0040 (size: 0x8)

    void ExecuteEvent(class APawn* Target, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x48

#endif

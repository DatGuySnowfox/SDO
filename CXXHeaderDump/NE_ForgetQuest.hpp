#ifndef UE4SS_SDK_NE_ForgetQuest_HPP
#define UE4SS_SDK_NE_ForgetQuest_HPP

class UNE_ForgetQuest_C : public UNarrativeEvent
{
    TSubclassOf<class UQuest> Quest;                                                  // 0x0030 (size: 0x8)

    void ExecuteEvent(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
    FString GetGraphDisplayText();
}; // Size: 0x38

#endif

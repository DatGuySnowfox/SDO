#ifndef UE4SS_SDK_NC_IsQuestAtState_HPP
#define UE4SS_SDK_NC_IsQuestAtState_HPP

class UNC_IsQuestAtState_C : public UNarrativeCondition
{
    TSubclassOf<class UQuest> Quest;                                                  // 0x0030 (size: 0x8)
    FName State;                                                                      // 0x0038 (size: 0x8)

    bool CheckCondition(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
    FString GetGraphDisplayText();
}; // Size: 0x40

#endif

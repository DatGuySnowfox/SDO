#ifndef UE4SS_SDK_BP_PlayerCharacter_Tutorial_HPP
#define UE4SS_SDK_BP_PlayerCharacter_Tutorial_HPP

class ABP_PlayerCharacter_Tutorial_C : public ABP_PlayerCharacter_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x1E40 (size: 0x8)
    class UTutorialComponent_C* TutorialComponent;                                    // 0x1E48 (size: 0x8)

    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_PlayerCharacter_Tutorial(int32 EntryPoint);
}; // Size: 0x1E50

#endif

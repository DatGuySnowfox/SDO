#ifndef UE4SS_SDK_PersistentLevel_HPP
#define UE4SS_SDK_PersistentLevel_HPP

class APersistentLevel_C : public ALevelScriptActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class ULoadingScreenWidget_C* LoadingScreen;                                      // 0x02A8 (size: 0x8)
    FName CurrentLevel;                                                               // 0x02B0 (size: 0x8)
    FPersistentLevel_CRemoveLoadingScreen RemoveLoadingScreen;                        // 0x02B8 (size: 0x10)
    void RemoveLoadingScreen();
    FPersistentLevel_CGetLevelName GetLevelName;                                      // 0x02C8 (size: 0x10)
    void GetLevelName(FName Name);

    void ReceiveBeginPlay();
    void Event_FadeFinished();
    void Event_LoadLevel(FName LevelName);
    void Event_RemoveLoadingScreen();
    void ExecuteUbergraph_PersistentLevel(int32 EntryPoint);
    void GetLevelName__DelegateSignature(FName Name);
    void RemoveLoadingScreen__DelegateSignature();
}; // Size: 0x2D8

#endif

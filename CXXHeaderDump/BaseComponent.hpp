#ifndef UE4SS_SDK_BaseComponent_HPP
#define UE4SS_SDK_BaseComponent_HPP

class UBaseComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    class ACharacter* Player;                                                         // 0x00A8 (size: 0x8)
    class AController* Controller;                                                    // 0x00B0 (size: 0x8)

    void Svr_Initialise();
    void LoadComponent();
    void ExecuteUbergraph_BaseComponent(int32 EntryPoint);
}; // Size: 0xB8

#endif

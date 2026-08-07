#ifndef UE4SS_SDK_BP_QuestMarker_HPP
#define UE4SS_SDK_BP_QuestMarker_HPP

class ABP_QuestMarker_C : public ABP_MasterQuestObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTexture2D* Marker Texture;                                                 // 0x02C8 (size: 0x8)
    FLinearColor Color;                                                               // 0x02D0 (size: 0x10)
    FString Text;                                                                     // 0x02E0 (size: 0x10)
    TArray<class UW_QuestMarker_C*> Markers;                                          // 0x02F0 (size: 0x10)
    class ABP_CompassMarker_C* CompassMarker;                                         // 0x0300 (size: 0x8)
    bool Underground?;                                                                // 0x0308 (size: 0x1)

    void ReceiveBeginPlay();
    void AddMarker();
    void ReceiveDestroyed();
    void ExecuteUbergraph_BP_QuestMarker(int32 EntryPoint);
}; // Size: 0x309

#endif

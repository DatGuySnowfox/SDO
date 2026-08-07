#ifndef UE4SS_SDK_Container_AnimalMeat_HPP
#define UE4SS_SDK_Container_AnimalMeat_HPP

class AContainer_AnimalMeat_C : public ABP_LootContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)
    bool SmallAnimal?;                                                                // 0x0448 (size: 0x1)

    void TraceForFloor();
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_Container_AnimalMeat(int32 EntryPoint);
}; // Size: 0x449

#endif

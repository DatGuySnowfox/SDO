#ifndef UE4SS_SDK_Buildable_ModularMaster_HPP
#define UE4SS_SDK_Buildable_ModularMaster_HPP

class ABuildable_ModularMaster_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)

    void GetSnappedObjects(TArray<class ABuildable_ModularMaster_C*>& OverlappingObjects);
    void RemoveModularBuilds();
    void ExecuteUbergraph_Buildable_ModularMaster(int32 EntryPoint);
}; // Size: 0x438

#endif

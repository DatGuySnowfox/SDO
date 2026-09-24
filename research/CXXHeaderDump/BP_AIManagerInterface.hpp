#ifndef UE4SS_SDK_BP_AIManagerInterface_HPP
#define UE4SS_SDK_BP_AIManagerInterface_HPP

class IBP_AIManagerInterface_C : public IInterface
{

    void Return AI Manager(class APrefab_Quest_CoyotesCheckpoint3_C*& AI Manager);
    void Return AI Manager Settings(FS_AIOptimization& AI Settings);
}; // Size: 0x28

#endif

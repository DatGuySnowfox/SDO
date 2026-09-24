#ifndef UE4SS_SDK_JigContextMenuCanvas_HPP
#define UE4SS_SDK_JigContextMenuCanvas_HPP

class UJigContextMenuCanvas_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UJigContextMenuW_C* JigContextMenuW;                                        // 0x0348 (size: 0x8)

    void Construct();
    void ExecuteUbergraph_JigContextMenuCanvas(int32 EntryPoint);
}; // Size: 0x350

#endif

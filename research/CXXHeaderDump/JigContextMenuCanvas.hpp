#ifndef UE4SS_SDK_JigContextMenuCanvas_HPP
#define UE4SS_SDK_JigContextMenuCanvas_HPP

class UJigContextMenuCanvas_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UJigContextMenuW_C* JigContextMenuW;                                        // 0x02C8 (size: 0x8)

    void Construct();
    void ExecuteUbergraph_JigContextMenuCanvas(int32 EntryPoint);
}; // Size: 0x2D0

#endif

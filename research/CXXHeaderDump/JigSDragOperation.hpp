#ifndef UE4SS_SDK_JigSDragOperation_HPP
#define UE4SS_SDK_JigSDragOperation_HPP

class UJigSDragOperation_C : public UDragDropOperation
{
    class UJSI_Slot_C* SlotRef;                                                       // 0x0090 (size: 0x8)
    FVector2D MouseDownPos;                                                           // 0x0098 (size: 0x10)
    class UJSI_Slot_C* FixedSlotRef;                                                  // 0x00A8 (size: 0x8)

}; // Size: 0xB0

#endif

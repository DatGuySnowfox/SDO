#ifndef UE4SS_SDK_DragWidget_HPP
#define UE4SS_SDK_DragWidget_HPP

class UDragWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* SImage;                                                             // 0x0348 (size: 0x8)
    class UBorder* SBorder;                                                           // 0x0350 (size: 0x8)
    class UImage* FrameImg;                                                           // 0x0358 (size: 0x8)
    FVector2D JigItemSize;                                                            // 0x0360 (size: 0x10)
    class UTexture* Image;                                                            // 0x0370 (size: 0x8)
    class UJSI_Slot_C* ParentSlot;                                                    // 0x0378 (size: 0x8)
    TArray<UJSIContainer_C*> ContainersDragOver;                                      // 0x0380 (size: 0x10)
    FKey RotateKey;                                                                   // 0x0390 (size: 0x18)
    bool IsKeyDown;                                                                   // 0x03A8 (size: 0x1)
    bool RotationChanged?;                                                            // 0x03A9 (size: 0x1)
    class UJSI_Slot_C* OnSlot;                                                        // 0x03B0 (size: 0x8)
    class UDragDropOperation* Operation;                                              // 0x03B8 (size: 0x8)
    bool IsSlotRotated?;                                                              // 0x03C0 (size: 0x1)
    double InitImageRot;                                                              // 0x03C8 (size: 0x8)
    bool AdjustLocToMouse;                                                            // 0x03D0 (size: 0x1)
    bool isSplit?;                                                                    // 0x03D1 (size: 0x1)
    TArray<UJSIContainer_C*> HighlightedEContainers;                                  // 0x03D8 (size: 0x10)
    TArray<UJSI_Slot_C*> HighlightedItems;                                            // 0x03E8 (size: 0x10)

    void HighlightItemOverItemValidContainers();
    void ClearVisualHighlight();
    void HighlightAllValidContainers();
    void HandleRotation();
    FEventReply OnKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    void AddContainer(const class UJSIContainer_C*& NewContainer);
    void Construct();
    void Destruct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void RotateKeyDown();
    void ResetOpacity();
    void ExecuteUbergraph_DragWidget(int32 EntryPoint);
}; // Size: 0x3F8

#endif

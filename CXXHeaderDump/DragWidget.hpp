#ifndef UE4SS_SDK_DragWidget_HPP
#define UE4SS_SDK_DragWidget_HPP

class UDragWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* FrameImg;                                                           // 0x02C8 (size: 0x8)
    class UBorder* SBorder;                                                           // 0x02D0 (size: 0x8)
    class UImage* SImage;                                                             // 0x02D8 (size: 0x8)
    FVector2D JigItemSize;                                                            // 0x02E0 (size: 0x10)
    class UTexture* Image;                                                            // 0x02F0 (size: 0x8)
    class UJSI_Slot_C* ParentSlot;                                                    // 0x02F8 (size: 0x8)
    TArray<class UJSIContainer_C*> ContainersDragOver;                                // 0x0300 (size: 0x10)
    FKey RotateKey;                                                                   // 0x0310 (size: 0x18)
    bool IsKeyDown;                                                                   // 0x0328 (size: 0x1)
    bool RotationChanged?;                                                            // 0x0329 (size: 0x1)
    class UJSI_Slot_C* OnSlot;                                                        // 0x0330 (size: 0x8)
    class UDragDropOperation* Operation;                                              // 0x0338 (size: 0x8)
    bool IsSlotRotated?;                                                              // 0x0340 (size: 0x1)
    double InitImageRot;                                                              // 0x0348 (size: 0x8)
    bool AdjustLocToMouse;                                                            // 0x0350 (size: 0x1)
    bool isSplit?;                                                                    // 0x0351 (size: 0x1)
    TArray<class UJSIContainer_C*> HighlightedEContainers;                            // 0x0358 (size: 0x10)
    TArray<class UJSI_Slot_C*> HighlightedItems;                                      // 0x0368 (size: 0x10)

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
}; // Size: 0x378

#endif

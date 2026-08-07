#ifndef UE4SS_SDK_2_ContainerListTitle_HPP
#define UE4SS_SDK_2_ContainerListTitle_HPP

class U2_ContainerListTitle_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* Border_0;                                                          // 0x02C8 (size: 0x8)
    class UImage* Image;                                                              // 0x02D0 (size: 0x8)
    class UTextBlock* ItemName;                                                       // 0x02D8 (size: 0x8)
    FGuid UID;                                                                        // 0x02E0 (size: 0x10)
    class UJigsawItem_DataAsset_C* ItemInfo;                                          // 0x02F0 (size: 0x8)
    F2_ContainerListTitle_CClicked Clicked;                                           // 0x02F8 (size: 0x10)
    void Clicked();
    class U3_TooltipContainerItemsUI_C* Tooltip;                                      // 0x0308 (size: 0x8)

    class UWidget* GetToolTipWidget();
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void Construct();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_2_ContainerListTitle(int32 EntryPoint);
    void Clicked__DelegateSignature();
}; // Size: 0x310

#endif

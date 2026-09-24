#ifndef UE4SS_SDK_BP_EquipToContent_HPP
#define UE4SS_SDK_BP_EquipToContent_HPP

class UBP_EquipToContent_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Weight;                                                         // 0x0348 (size: 0x8)
    class UImage* Image_115;                                                          // 0x0350 (size: 0x8)
    class UHorizontalBox* HorizontalBox_1;                                            // 0x0358 (size: 0x8)
    class UBorder* DropdownBorder;                                                    // 0x0360 (size: 0x8)
    class UTextBlock* ContentName;                                                    // 0x0368 (size: 0x8)
    class UBorder* ContentBorder;                                                     // 0x0370 (size: 0x8)
    class UImage* ArrowUpDown;                                                        // 0x0378 (size: 0x8)
    class UJSIContainer_C* EquipToRef;                                                // 0x0380 (size: 0x8)
    bool ContentVisible;                                                              // 0x0388 (size: 0x1)
    class UWidget* ContentRef;                                                        // 0x0390 (size: 0x8)
    FBP_EquipToContent_COnShown OnShown;                                              // 0x0398 (size: 0x10)
    void OnShown();
    FBP_EquipToContent_COnCollapsed OnCollapsed;                                      // 0x03A8 (size: 0x10)
    void OnCollapsed();
    double TotalWeight;                                                               // 0x03B8 (size: 0x8)
    TArray<UJSIContainer_C*> AllContainers;                                           // 0x03C0 (size: 0x10)

    void GetItemName(FText ContentName, FText& Name);
    void SetWeightText();
    FEventReply On_ArrowUpDown_MouseButtonDown_0(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void SetupContent(class UWidget* WidgetRef, FText ContentName, bool ShowDropdownOption?);
    void OnWeightUpdated_Event_0(double NewWeight);
    void ClearContent();
    void ExecuteUbergraph_BP_EquipToContent(int32 EntryPoint);
    void OnCollapsed__DelegateSignature();
    void OnShown__DelegateSignature();
}; // Size: 0x3D0

#endif

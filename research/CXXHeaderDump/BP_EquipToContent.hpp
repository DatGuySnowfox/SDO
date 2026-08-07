#ifndef UE4SS_SDK_BP_EquipToContent_HPP
#define UE4SS_SDK_BP_EquipToContent_HPP

class UBP_EquipToContent_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* ArrowUpDown;                                                        // 0x02C8 (size: 0x8)
    class UBorder* ContentBorder;                                                     // 0x02D0 (size: 0x8)
    class UTextBlock* ContentName;                                                    // 0x02D8 (size: 0x8)
    class UBorder* DropdownBorder;                                                    // 0x02E0 (size: 0x8)
    class UHorizontalBox* HorizontalBox_1;                                            // 0x02E8 (size: 0x8)
    class UImage* Image_115;                                                          // 0x02F0 (size: 0x8)
    class UTextBlock* Weight;                                                         // 0x02F8 (size: 0x8)
    class UJSIContainer_C* EquipToRef;                                                // 0x0300 (size: 0x8)
    bool ContentVisible;                                                              // 0x0308 (size: 0x1)
    class UWidget* ContentRef;                                                        // 0x0310 (size: 0x8)
    FBP_EquipToContent_COnShown OnShown;                                              // 0x0318 (size: 0x10)
    void OnShown();
    FBP_EquipToContent_COnCollapsed OnCollapsed;                                      // 0x0328 (size: 0x10)
    void OnCollapsed();
    double TotalWeight;                                                               // 0x0338 (size: 0x8)
    TArray<class UJSIContainer_C*> AllContainers;                                     // 0x0340 (size: 0x10)

    void GetItemName(FText ContentName, FText& Name);
    void SetWeightText();
    FEventReply On_ArrowUpDown_MouseButtonDown_0(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void SetupContent(class UWidget* WidgetRef, FText ContentName, bool ShowDropdownOption?);
    void OnWeightUpdated_Event_0(double NewWeight);
    void ClearContent();
    void ExecuteUbergraph_BP_EquipToContent(int32 EntryPoint);
    void OnCollapsed__DelegateSignature();
    void OnShown__DelegateSignature();
}; // Size: 0x350

#endif

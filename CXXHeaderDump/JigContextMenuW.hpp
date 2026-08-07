#ifndef UE4SS_SDK_JigContextMenuW_HPP
#define UE4SS_SDK_JigContextMenuW_HPP

class UJigContextMenuW_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* ConsumeBtn;                                                        // 0x02C8 (size: 0x8)
    class UButton* ConsumeCannedBtn;                                                  // 0x02D0 (size: 0x8)
    class UButton* DestroyBtn;                                                        // 0x02D8 (size: 0x8)
    class UButton* DrinkBtn;                                                          // 0x02E0 (size: 0x8)
    class UButton* DropBtn;                                                           // 0x02E8 (size: 0x8)
    class UButton* EquipBtn;                                                          // 0x02F0 (size: 0x8)
    class UButton* FillBtn;                                                           // 0x02F8 (size: 0x8)
    class UButton* InspectBtn;                                                        // 0x0300 (size: 0x8)
    class UButton* OpenBtn;                                                           // 0x0308 (size: 0x8)
    class UButton* OpenLootBoxBtn;                                                    // 0x0310 (size: 0x8)
    class UVerticalBox* OptionsVB;                                                    // 0x0318 (size: 0x8)
    class UButton* PlaceBtn;                                                          // 0x0320 (size: 0x8)
    class UButton* PlayMusicBtn;                                                      // 0x0328 (size: 0x8)
    class UButton* ReadBtn;                                                           // 0x0330 (size: 0x8)
    class UButton* RepairArmorBtn;                                                    // 0x0338 (size: 0x8)
    class UButton* RepairAttachmentBtn;                                               // 0x0340 (size: 0x8)
    class UButton* RepairWeaponBtn;                                                   // 0x0348 (size: 0x8)
    class UButton* RipClothingBtn;                                                    // 0x0350 (size: 0x8)
    class UButton* ScanLaptopBtn;                                                     // 0x0358 (size: 0x8)
    class UButton* ShowAttachmentsBtn;                                                // 0x0360 (size: 0x8)
    class UButton* ShowTacticalAttachmentsBtn;                                        // 0x0368 (size: 0x8)
    class UButton* SmokeBtn;                                                          // 0x0370 (size: 0x8)
    class UButton* UnequipBtn;                                                        // 0x0378 (size: 0x8)
    class UButton* UnloadBtn;                                                         // 0x0380 (size: 0x8)
    class UButton* UseBtn;                                                            // 0x0388 (size: 0x8)
    class UUserWidget* ParentW;                                                       // 0x0390 (size: 0x8)
    class UJigContextMenuComp_C* JigContextComp;                                      // 0x0398 (size: 0x8)

    void GetWidgetByMenuOption(FGameplayTag Option, class UButton*& WidgetRef);
    void SetEnabledOptions(FGameplayTagContainer MenuOptions, bool& Result?);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void SerVisNextFrame(double InOpacity);
    void BndEvt__JigContextMenuW_InspectBtn_K2Node_ComponentBoundEvent_8_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_OpenBtn_K2Node_ComponentBoundEvent_9_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_UseBtn_K2Node_ComponentBoundEvent_10_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_PlaceBtn_K2Node_ComponentBoundEvent_11_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_EquipBtn_K2Node_ComponentBoundEvent_12_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_UnequipBtn_K2Node_ComponentBoundEvent_13_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_UnloadBtn_K2Node_ComponentBoundEvent_14_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_ConsumeBtn_K2Node_ComponentBoundEvent_15_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_ConsumeCannedBtn_K2Node_ComponentBoundEvent_16_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_DrinkBtn_K2Node_ComponentBoundEvent_17_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_FillBtn_K2Node_ComponentBoundEvent_18_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_ReadBtn_K2Node_ComponentBoundEvent_19_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_RipClothingBtn_K2Node_ComponentBoundEvent_20_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_ScanLaptopBtn_K2Node_ComponentBoundEvent_21_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_RepairBtn_K2Node_ComponentBoundEvent_22_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_DropBtn_K2Node_ComponentBoundEvent_23_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_DestroyBtn_K2Node_ComponentBoundEvent_24_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_ShowAttachmentsBtn_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_RepairArmorBtn_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_PlayMusicBtn_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_OpenLootBoxBtn_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_ShowTacticalAttachmentsBtn_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_SmokeBtn_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JigContextMenuW_RepairAttachmentBtn_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_JigContextMenuW(int32 EntryPoint);
}; // Size: 0x3A0

#endif

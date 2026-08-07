#ifndef UE4SS_SDK_W_DeadPlayerLootUI_HPP
#define UE4SS_SDK_W_DeadPlayerLootUI_HPP

class UW_DeadPlayerLootUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* B_Backpack;                                                        // 0x02C8 (size: 0x8)
    class UBorder* B_Bag;                                                             // 0x02D0 (size: 0x8)
    class UBorder* B_BodyArmor;                                                       // 0x02D8 (size: 0x8)
    class UBorder* B_Legs;                                                            // 0x02E0 (size: 0x8)
    class UBorder* B_Other;                                                           // 0x02E8 (size: 0x8)
    class UBorder* B_Torso;                                                           // 0x02F0 (size: 0x8)
    class UJSIContainer_C* CAccessories;                                              // 0x02F8 (size: 0x8)
    class UJSIContainer_C* CArmor;                                                    // 0x0300 (size: 0x8)
    class UJSIContainer_C* CBackpack;                                                 // 0x0308 (size: 0x8)
    class UJSIContainer_C* CBinoculars;                                               // 0x0310 (size: 0x8)
    class UJSIContainer_C* CCompass;                                                  // 0x0318 (size: 0x8)
    class UJSIContainer_C* CContainer;                                                // 0x0320 (size: 0x8)
    class UJSIContainer_C* CEyewear;                                                  // 0x0328 (size: 0x8)
    class UJSIContainer_C* CFaceWear;                                                 // 0x0330 (size: 0x8)
    class UJSIContainer_C* CFeet;                                                     // 0x0338 (size: 0x8)
    class UJSIContainer_C* CFishingRod;                                               // 0x0340 (size: 0x8)
    class UJSIContainer_C* CFlashlight;                                               // 0x0348 (size: 0x8)
    class UJSIContainer_C* CGloves;                                                   // 0x0350 (size: 0x8)
    class UJSIContainer_C* CGPS;                                                      // 0x0358 (size: 0x8)
    class UJSIContainer_C* CHeadWear;                                                 // 0x0360 (size: 0x8)
    class UJSIContainer_C* CLegs;                                                     // 0x0368 (size: 0x8)
    class UJSIContainer_C* CMelee;                                                    // 0x0370 (size: 0x8)
    class UJSIContainer_C* CPrimary;                                                  // 0x0378 (size: 0x8)
    class UJSIContainer_C* CSecondary;                                                // 0x0380 (size: 0x8)
    class UJSIContainer_C* CSidearm;                                                  // 0x0388 (size: 0x8)
    class UJSIContainer_C* CThrowable;                                                // 0x0390 (size: 0x8)
    class UJSIContainer_C* CTorso;                                                    // 0x0398 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle_162;                              // 0x03A0 (size: 0x8)
    class UVerticalBox* InventoryVB;                                                  // 0x03A8 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Pockets;                                      // 0x03B0 (size: 0x8)
    class UVerticalBox* OtherContent;                                                 // 0x03B8 (size: 0x8)
    TArray<class UJSIContainer_C*> AllContainers;                                     // 0x03C0 (size: 0x10)

    void GetLootContent(class UUserWidget*& Widget);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void GetAllAttachments(TArray<FName>& Attachments);
    void GetListOfNonAddContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetListOfContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void AddContentToPanel(class UUserWidget* Content, class UJSIContainer_C* ToContainer);
    void SetItemReference(class UJSI_Slot_C* ItemRef);
    void SetActorOwner(class AActor* ActorRef);
    void JSICheckStatus();
    void JSIOnWeightUpdated(double NewWeight);
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void ForceInitSpecialcontainer();
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void OnCreatedFromUtility();
    void OnInitialized();
    void DropInfo_OnItemEquippedChange_Event(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void ExecuteUbergraph_W_DeadPlayerLootUI(int32 EntryPoint);
}; // Size: 0x3D0

#endif

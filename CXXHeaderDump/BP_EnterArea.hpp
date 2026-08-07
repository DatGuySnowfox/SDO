#ifndef UE4SS_SDK_BP_EnterArea_HPP
#define UE4SS_SDK_BP_EnterArea_HPP

class ABP_EnterArea_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x02A8 (size: 0x8)
    FText Name;                                                                       // 0x02B0 (size: 0x18)
    FTimerHandle OverlapTimer;                                                        // 0x02C8 (size: 0x8)
    bool AdditionalNotification?;                                                     // 0x02D0 (size: 0x1)
    FText Notification Details Message;                                               // 0x02D8 (size: 0x18)
    class UTexture2D* Notification Details Icon;                                      // 0x02F0 (size: 0x8)
    FLinearColor Notification Details Icon Colour;                                    // 0x02F8 (size: 0x10)

    void BndEvt__BP_RadiationArea_Sphere_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_RadiationArea_Sphere_K2Node_ComponentBoundEvent_1_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void Event_EnteredArea(class ABP_PlayerCharacter_C* Actor);
    void Event_ExitedArea(class ABP_PlayerCharacter_C* Actor);
    void ReceiveBeginPlay();
    void Event_Overlap();
    void Event_Respawn();
    void ExecuteUbergraph_BP_EnterArea(int32 EntryPoint);
}; // Size: 0x308

#endif

#ifndef UE4SS_SDK_Buildable_GeneratorMaster_HPP
#define UE4SS_SDK_Buildable_GeneratorMaster_HPP

class ABuildable_GeneratorMaster_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x0438 (size: 0x8)
    class UStaticMeshComponent* Sphere;                                               // 0x0440 (size: 0x8)
    class USphereComponent* Collision;                                                // 0x0448 (size: 0x8)
    double CurrentFuel;                                                               // 0x0450 (size: 0x8)
    double MaxFuel;                                                                   // 0x0458 (size: 0x8)
    bool TurnedOn?;                                                                   // 0x0460 (size: 0x1)
    FTimerHandle ShakeTimer;                                                          // 0x0468 (size: 0x8)
    FTimerHandle FuelTimer;                                                           // 0x0470 (size: 0x8)
    FTimerHandle FindObjectsTimer;                                                    // 0x0478 (size: 0x8)
    class ABP_PlayerCharacter_C* Player;                                              // 0x0480 (size: 0x8)
    bool PlayerClicked?;                                                              // 0x0488 (size: 0x1)
    class UWidget* InteractWidget;                                                    // 0x0490 (size: 0x8)

    void GetObjects_ForceOff();
    void OnRep_CurrentFuel();
    void FuelCost();
    void Shake();
    void OnRep_TurnedOn?();
    void GetObjects();
    void ActorLoaded();
    void ReceiveBeginPlay();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void Svr_UpdateFuel(double Fuel);
    void Client_Notification(FText Message, class UTexture2D* Icon, FLinearColor Icon Colour, double Delay);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_GeneratorMaster(int32 EntryPoint);
}; // Size: 0x498

#endif

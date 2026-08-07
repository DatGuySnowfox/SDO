#ifndef UE4SS_SDK_Buildable_WaterWell_HPP
#define UE4SS_SDK_Buildable_WaterWell_HPP

class ABuildable_WaterWell_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UActor_Weather_Status_C* Actor_Weather_Status;                              // 0x0438 (size: 0x8)
    class USceneComponent* TraceLocation;                                             // 0x0440 (size: 0x8)
    FTimerHandle WellTimer;                                                           // 0x0448 (size: 0x8)
    double CurrentWater;                                                              // 0x0450 (size: 0x8)
    double MaxWater;                                                                  // 0x0458 (size: 0x8)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x0460 (size: 0x8)
    bool OnLandscape?;                                                                // 0x0468 (size: 0x1)

    void SurfaceCheck();
    void WaterWellTrace();
    void DrinkWater();
    void Event_WellTimer();
    void OnInteractBuildable(class AActor* Actor);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void ReceiveBeginPlay();
    void Event_UpdateWater(double Value, bool Decrease?);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_WaterWell(int32 EntryPoint);
}; // Size: 0x469

#endif

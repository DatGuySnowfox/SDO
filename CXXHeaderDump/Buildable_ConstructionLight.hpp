#ifndef UE4SS_SDK_Buildable_ConstructionLight_HPP
#define UE4SS_SDK_Buildable_ConstructionLight_HPP

class ABuildable_ConstructionLight_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UStaticMeshComponent* Plane1;                                               // 0x0438 (size: 0x8)
    class UStaticMeshComponent* Plane;                                                // 0x0440 (size: 0x8)
    class USpotLightComponent* SpotLight1;                                            // 0x0448 (size: 0x8)
    class USpotLightComponent* SpotLight;                                             // 0x0450 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x0458 (size: 0x8)
    bool LightOn;                                                                     // 0x0460 (size: 0x1)

    void ReceiveBeginPlay();
    void EventPoweredOn();
    void EventPoweredOff();
    void OnInteractBuildable(class AActor* Actor);
    void LightCallback(bool NewValue);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_ConstructionLight(int32 EntryPoint);
}; // Size: 0x461

#endif

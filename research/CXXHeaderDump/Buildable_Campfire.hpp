#ifndef UE4SS_SDK_Buildable_Campfire_HPP
#define UE4SS_SDK_Buildable_Campfire_HPP

class ABuildable_Campfire_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UParticleSystemComponent* Particles;                                        // 0x0438 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x0440 (size: 0x8)
    class UAudioComponent* Sound;                                                     // 0x0448 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x0450 (size: 0x8)

    void JigMP_OnRequestDropItem(FRepItemInfo ItemInfo, FName ContainerName, class AActor*& ActorRef);
    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_Campfire(int32 EntryPoint);
}; // Size: 0x458

#endif

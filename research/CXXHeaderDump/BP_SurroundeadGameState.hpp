#ifndef UE4SS_SDK_BP_SurroundeadGameState_HPP
#define UE4SS_SDK_BP_SurroundeadGameState_HPP

class ABP_SurroundeadGameState_C : public AGameStateBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02F0 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02F8 (size: 0x8)
    class ABP_ItemSnapCaptor_C* SnapActorRef;                                         // 0x0300 (size: 0x8)
    bool FirstCaptureDone;                                                            // 0x0308 (size: 0x1)
    TArray<class UJSI_Slot_C*> ItemsQueue;                                            // 0x0310 (size: 0x10)
    double SnapDelay;                                                                 // 0x0320 (size: 0x8)
    TArray<FName> AllInspectedIDs;                                                    // 0x0328 (size: 0x10)
    TArray<int32> AllUIDs;                                                            // 0x0338 (size: 0x10)

    void InspectorGetPrimitiveComponent(class AActor* Actor, class UPrimitiveComponent*& Comp);
    void GetLocalAttachmentByID(class UJigsawItem_DataAsset_C* ItemId, bool& Found, FName& Socket, FGameplayTag& Type, TSubclassOf<class AActor>& Class);
    void SpawnSnapshotCaptor(class UJSI_Slot_C* ItemRef, class ABP_ItemInspector_C*& InspectorRef);
    void UpdateSnapCustom(FTransform Transform, double FOV, class UJSI_Slot_C* JigItem, bool& Result);
    void AddItemToQueue(class UJSI_Slot_C* Item, bool& Result);
    void UpdateAttachments(class UJSI_Slot_C* ItemRef, class UTextureRenderTarget2D* RenderT, TArray<FName>& Attachments, class UTextureRenderTarget*& RenderTarget);
    void UpdateSnap(class UJSI_Slot_C* ItemRef, class UTextureRenderTarget2D* RenderT, class UTextureRenderTarget*& RenderTarget);
    void GetSnap(class UJSI_Slot_C* ItemRef, class UTextureRenderTarget2D*& Texture);
    bool CanItemBeCaptured(class UJSI_Slot_C* ItemRef, bool& Shared);
    void HandleQueue();
    void HandleSnapTaken();
    void OnSnapTaken();
    void HandleNext();
    void ExecuteUbergraph_BP_SurroundeadGameState(int32 EntryPoint);
}; // Size: 0x348

#endif

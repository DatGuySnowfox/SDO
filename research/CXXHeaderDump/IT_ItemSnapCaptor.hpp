#ifndef UE4SS_SDK_IT_ItemSnapCaptor_HPP
#define UE4SS_SDK_IT_ItemSnapCaptor_HPP

class IIT_ItemSnapCaptor_C : public IInterface
{

    void InspectorGetPrimitiveComponent(class AActor* Actor, class UPrimitiveComponent*& Comp);
    void GetLocalAttachmentByID(class UJigsawItem_DataAsset_C* ItemId, bool& Found, FName& Socket, FGameplayTag& Type, TSubclassOf<class AActor>& Class);
    void SpawnSnapshotCaptor(class UJSI_Slot_C* ItemRef, class ABP_ItemInspector_C*& InspectorRef);
    void UpdateSnapCustom(FTransform Transform, double FOV, class UJSI_Slot_C* JigItem, bool& Result);
    void AddItemToQueue(class UJSI_Slot_C* Item, bool& Result);
    void UpdateAttachments(class UJSI_Slot_C* ItemRef, class UTextureRenderTarget2D* RenderT, TArray<FName>& Attachments, class UTextureRenderTarget*& RenderTarget);
    void UpdateSnap(class UJSI_Slot_C* ItemRef, class UTextureRenderTarget2D* RenderT, class UTextureRenderTarget*& RenderTarget);
    void GetSnap(class UJSI_Slot_C* ItemRef, class UTextureRenderTarget2D*& Texture);
}; // Size: 0x28

#endif

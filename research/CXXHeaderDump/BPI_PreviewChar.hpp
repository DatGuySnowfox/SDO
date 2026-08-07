#ifndef UE4SS_SDK_BPI_PreviewChar_HPP
#define UE4SS_SDK_BPI_PreviewChar_HPP

class IBPI_PreviewChar_C : public IInterface
{

    void Preview Set Equipped Primitive By Slot(FGameplayTag Tag, class UObject* Item, FName Socket, bool ProcessAttachments?);
    void PreviewAttachmentUpdated(FGameplayTag Tag, class UJSI_Slot_C* Item);
    void PreviewOnWeaponEquipped(FGameplayTag Tag, class UJSI_Slot_C* Item);
    void SetPreviewRenderTarget(class UTextureRenderTarget2D* RT);
    void SetPreviewZoomInOut(bool Value);
    void SetPreviewMeshRotation(FVector2D Rot);
    void SetPreviewPawnOwner(class AActor* Owner);
    void SetCaptureActivate(bool Value);
}; // Size: 0x28

#endif

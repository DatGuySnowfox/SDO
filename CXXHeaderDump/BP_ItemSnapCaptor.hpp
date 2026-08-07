#ifndef UE4SS_SDK_BP_ItemSnapCaptor_HPP
#define UE4SS_SDK_BP_ItemSnapCaptor_HPP

class ABP_ItemSnapCaptor_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USpringArmComponent* SpringArm;                                             // 0x02A0 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02A8 (size: 0x8)
    class USceneComponent* Main;                                                      // 0x02B0 (size: 0x8)
    class USpotLightComponent* SpotLight;                                             // 0x02B8 (size: 0x8)
    class USceneCaptureComponent2D* SceneCaptureComponent2D;                          // 0x02C0 (size: 0x8)
    double InitialFOV;                                                                // 0x02C8 (size: 0x8)
    class UJSI_Slot_C* ItemToInspect;                                                 // 0x02D0 (size: 0x8)
    TMap<class FName, class UTextureRenderTarget2D*> CreatedRenderTargets;            // 0x02D8 (size: 0x50)
    bool IsLocked?;                                                                   // 0x0328 (size: 0x1)
    bool Streaming?;                                                                  // 0x0329 (size: 0x1)
    TArray<class ABP_AMainLocalAttachment_C*> CurrentAttachments;                     // 0x0330 (size: 0x10)
    FBP_ItemSnapCaptor_COnSnapTaken OnSnapTaken;                                      // 0x0340 (size: 0x10)
    void OnSnapTaken();
    class USceneComponent* ItemMeshComp;                                              // 0x0350 (size: 0x8)

    void CaptureFromTargetCustom(class UTextureRenderTarget2D* TextureTarget, bool& GoNext?);
    void RemoveIronsights(bool Hide?, class ABP_AMainLocalAttachment_C* Attachment, class USceneComponent* SceneComp);
    void GetAttachSocketByInContainerIndex(class UJigsawItem_DataAsset_C* Info, int32 InContainerIndex, FName& Socket);
    void GetActiveComponent(class USceneComponent*& Comp);
    void SetJigitem_Custom(class UJSI_Slot_C* ItemRef, FTransform Transform, double FOV, bool& Found, bool& ShareRenderTargetWithSimilarItems);
    void SetLocked(bool IsLocked?);
    void IsLocked(bool& IsLocked?);
    void CaptureFromTarget(class UTextureRenderTarget2D* TextureTarget, bool& GoNext?);
    void GetSnapNoCapture(class UJSI_Slot_C* Item, bool ShareRTarget, class UTextureRenderTarget2D*& Output_Get, bool& FromShared?);
    void GetDefaultDim(class UJSI_Slot_C* Target, FVector2D& SlotDimension);
    FTransform GetInitValues(double& FOVAngle);
    void IsActionbar(class UJSI_Slot_C* ItemRef, bool& Result);
    void ClearAttachments();
    void InstallAttachments(class UJSI_Slot_C* JigItem);
    void UpdateSnap(class UJSI_Slot_C* ItemRef, class UTextureRenderTarget2D* TextureTarget, class UTextureRenderTarget2D*& Output_Get);
    void GetSnap(bool ShareRTarget, class UTextureRenderTarget2D*& Output_Get, bool& FromShared?);
    void SetJigitem(class UJSI_Slot_C* ItemRef, bool& Found, bool& ShareRenderTargetWithSimilarItems);
    void UserConstructionScript();
    void DelayedCaptureSnap();
    void ReceiveBeginPlay();
    void CaptureSnap();
    void ExecuteUbergraph_BP_ItemSnapCaptor(int32 EntryPoint);
    void OnSnapTaken__DelegateSignature();
}; // Size: 0x358

#endif

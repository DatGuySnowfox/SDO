#ifndef UE4SS_SDK_BP_ItemInspector_HPP
#define UE4SS_SDK_BP_ItemInspector_HPP

class ABP_ItemInspector_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x02A0 (size: 0x8)
    class USpotLightComponent* SpotLight2;                                            // 0x02A8 (size: 0x8)
    class USpotLightComponent* SpotLight1;                                            // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02B8 (size: 0x8)
    class USpringArmComponent* SpringArm;                                             // 0x02C0 (size: 0x8)
    class USceneComponent* Root;                                                      // 0x02C8 (size: 0x8)
    class USceneComponent* Main;                                                      // 0x02D0 (size: 0x8)
    class USpotLightComponent* SpotLight;                                             // 0x02D8 (size: 0x8)
    class USceneCaptureComponent2D* SceneCaptureComponent2D;                          // 0x02E0 (size: 0x8)
    double InitialFOV;                                                                // 0x02E8 (size: 0x8)
    class UJSI_Slot_C* ItemToInspect;                                                 // 0x02F0 (size: 0x8)
    double MaxZoomInFOV;                                                              // 0x02F8 (size: 0x8)
    double MaxZoomOutFOV;                                                             // 0x0300 (size: 0x8)
    class UTextureRenderTarget2D* RenderT;                                            // 0x0308 (size: 0x8)
    TArray<class ABP_AMainLocalAttachment_C*> CurrentAttachments;                     // 0x0310 (size: 0x10)
    FRotator NewVar;                                                                  // 0x0320 (size: 0x18)

    void SetMeshPosition(FVector2D XYRot);
    void RemoveIronsights(bool Hide?, class ABP_AMainLocalAttachment_C* Attachment);
    void Capture One Frame();
    void FindLocalAttachmentByUID(FGuid UID, class ABP_AMainLocalAttachment_C*& Attachment);
    void InstallAttachments(class UJSI_Slot_C* JigItem);
    void ClearAttachments();
    void GetActiveComponent(class USceneComponent*& Comp);
    void GetAttachSocketByInContainerIndex(class UJigsawItem_DataAsset_C* Info, int32 InContainerIndex, FName& Socket);
    void SetCaptorActiveState(bool IsActive);
    FTransform GetInitValues(double& FOVAngle);
    void UpdateLocAndRot(double FOVAngle, FTransform NewTransform);
    void GetRenderTarget(class UTextureRenderTarget2D*& RenderT);
    void OnZoomIn();
    void OnZoomOut();
    void SetMeshRotation(FVector2D XYRot);
    void SetItemToInspect(class UJSI_Slot_C* ItemRef);
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void ExecuteUbergraph_BP_ItemInspector(int32 EntryPoint);
}; // Size: 0x338

#endif

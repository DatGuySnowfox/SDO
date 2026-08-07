#ifndef UE4SS_SDK_CommonPhotoThumb_HPP
#define UE4SS_SDK_CommonPhotoThumb_HPP

class UCommonPhotoThumb_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* NotifySelection;                                          // 0x02C8 (size: 0x8)
    class UButton* ButtonPhoto;                                                       // 0x02D0 (size: 0x8)
    class UCanvasPanel* Edges;                                                        // 0x02D8 (size: 0x8)
    class UImage* PhotoFrame;                                                         // 0x02E0 (size: 0x8)
    FPhotosData PhotoData;                                                            // 0x02E8 (size: 0x30)
    FLinearColor CurrentColor;                                                        // 0x0318 (size: 0x10)
    class UTexture2D* Texture;                                                        // 0x0328 (size: 0x8)
    class UTexture2D* Frame;                                                          // 0x0330 (size: 0x8)
    FVector2D BaseSize;                                                               // 0x0338 (size: 0x10)
    double SizeMultiplier;                                                            // 0x0348 (size: 0x8)
    class UPhotoModeGalleryWidget_C* GalleryReference;                                // 0x0350 (size: 0x8)
    FCommonPhotoThumb_CSendPhoto SendPhoto;                                           // 0x0358 (size: 0x10)
    void SendPhoto(FPhotosData PhotoData);
    class UCommonPage_C* Page;                                                        // 0x0368 (size: 0x8)
    FLinearColor FrameFocusedColor;                                                   // 0x0370 (size: 0x10)
    FLinearColor FrameUnfocusedColor;                                                 // 0x0380 (size: 0x10)
    bool Focused;                                                                     // 0x0390 (size: 0x1)

    ESlateVisibility Edges Visibility();
    FLinearColor FrameFocusedAndUnfocusedColor();
    void PreConstruct(bool IsDesignTime);
    void SetPhotoData(FPhotosData PhotoInfo);
    void Initialize(class UPhotoModeGalleryWidget_C* GalleryReference, class UCommonPage_C* OwnPage);
    void Destruct();
    void BndEvt__ButtonPhoto_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void Animation();
    void ResetThumb();
    void UpdatePhoto();
    void DeleteData();
    void ResetThumbAnimation();
    void ExecuteUbergraph_CommonPhotoThumb(int32 EntryPoint);
    void SendPhoto__DelegateSignature(FPhotosData PhotoData);
}; // Size: 0x391

#endif

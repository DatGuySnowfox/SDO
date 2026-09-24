#ifndef UE4SS_SDK_CommonPhotoThumb_HPP
#define UE4SS_SDK_CommonPhotoThumb_HPP

class UCommonPhotoThumb_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* PhotoFrame;                                                         // 0x0348 (size: 0x8)
    class UCanvasPanel* Edges;                                                        // 0x0350 (size: 0x8)
    class UButton* ButtonPhoto;                                                       // 0x0358 (size: 0x8)
    class UWidgetAnimation* NotifySelection;                                          // 0x0360 (size: 0x8)
    FPhotosData PhotoData;                                                            // 0x0368 (size: 0x30)
    FLinearColor CurrentColor;                                                        // 0x0398 (size: 0x10)
    class UTexture2D* Texture;                                                        // 0x03A8 (size: 0x8)
    class UTexture2D* Frame;                                                          // 0x03B0 (size: 0x8)
    FVector2D BaseSize;                                                               // 0x03B8 (size: 0x10)
    double SizeMultiplier;                                                            // 0x03C8 (size: 0x8)
    class UPhotoModeGalleryWidget_C* GalleryReference;                                // 0x03D0 (size: 0x8)
    FCommonPhotoThumb_CSendPhoto SendPhoto;                                           // 0x03D8 (size: 0x10)
    void SendPhoto(FPhotosData PhotoData);
    class UCommonPage_C* Page;                                                        // 0x03E8 (size: 0x8)
    FLinearColor FrameFocusedColor;                                                   // 0x03F0 (size: 0x10)
    FLinearColor FrameUnfocusedColor;                                                 // 0x0400 (size: 0x10)
    bool Focused;                                                                     // 0x0410 (size: 0x1)

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
}; // Size: 0x411

#endif

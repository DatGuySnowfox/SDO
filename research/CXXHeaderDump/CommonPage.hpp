#ifndef UE4SS_SDK_CommonPage_HPP
#define UE4SS_SDK_CommonPage_HPP

class UCommonPage_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UUniformGridPanel* Page;                                                    // 0x0348 (size: 0x8)
    int32 PhotosPerColumn;                                                            // 0x0350 (size: 0x4)
    int32 PhotosPerRow;                                                               // 0x0354 (size: 0x4)
    TArray<UCommonPhotoThumb_C*> PhotosArray;                                         // 0x0358 (size: 0x10)
    int32 PageIndex;                                                                  // 0x0368 (size: 0x4)
    class UPhotoModeGalleryWidget_C* GalleryReference;                                // 0x0370 (size: 0x8)

    void PreConstruct(bool IsDesignTime);
    void Construct();
    void Initialize(class UPhotoModeGalleryWidget_C* GalleryReference, int32 PageIndex);
    void ResetPage();
    void UpdatePage();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_CommonPage(int32 EntryPoint);
}; // Size: 0x378

#endif

#ifndef UE4SS_SDK_CommonPage_HPP
#define UE4SS_SDK_CommonPage_HPP

class UCommonPage_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UUniformGridPanel* Page;                                                    // 0x02C8 (size: 0x8)
    int32 PhotosPerColumn;                                                            // 0x02D0 (size: 0x4)
    int32 PhotosPerRow;                                                               // 0x02D4 (size: 0x4)
    TArray<class UCommonPhotoThumb_C*> PhotosArray;                                   // 0x02D8 (size: 0x10)
    int32 PageIndex;                                                                  // 0x02E8 (size: 0x4)
    class UPhotoModeGalleryWidget_C* GalleryReference;                                // 0x02F0 (size: 0x8)

    void PreConstruct(bool IsDesignTime);
    void Construct();
    void Initialize(class UPhotoModeGalleryWidget_C* GalleryReference, int32 PageIndex);
    void ResetPage();
    void UpdatePage();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_CommonPage(int32 EntryPoint);
}; // Size: 0x2F8

#endif

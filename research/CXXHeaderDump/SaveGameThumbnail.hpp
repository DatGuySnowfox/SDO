#ifndef UE4SS_SDK_SaveGameThumbnail_HPP
#define UE4SS_SDK_SaveGameThumbnail_HPP

class USaveGameThumbnail_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* Thumbnail;                                                          // 0x0348 (size: 0x8)

    void SetThumbnail(class UTexture2D* Texture);
    void ClearThumbnail();
    void ExecuteUbergraph_SaveGameThumbnail(int32 EntryPoint);
}; // Size: 0x350

#endif

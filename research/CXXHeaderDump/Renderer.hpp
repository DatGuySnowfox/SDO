#ifndef UE4SS_SDK_Renderer_HPP
#define UE4SS_SDK_Renderer_HPP

#include "Renderer_enums.hpp"

class ASparseVolumeTextureViewer : public AInfo
{
    class USparseVolumeTextureViewerComponent* SparseVolumeTextureViewerComponent;    // 0x0298 (size: 0x8)

}; // Size: 0x2A0

class USparseVolumeTextureViewerComponent : public UPrimitiveComponent
{
    class USparseVolumeTexture* SparseVolumeTexturePreview;                           // 0x0570 (size: 0x8)
    uint8 bAnimate;                                                                   // 0x0578 (size: 0x1)
    uint8 bReversePlayback;                                                           // 0x0578 (size: 0x1)
    uint8 bBlockingStreamingRequests;                                                 // 0x0578 (size: 0x1)
    float AnimationFrame;                                                             // 0x057C (size: 0x4)
    float FrameRate;                                                                  // 0x0580 (size: 0x4)
    float AnimationTime;                                                              // 0x0584 (size: 0x4)
    TEnumAsByte<ESparseVolumeTexturePreviewAttribute> PreviewAttribute;               // 0x0588 (size: 0x1)
    int32 MipLevel;                                                                   // 0x058C (size: 0x4)
    float Extinction;                                                                 // 0x0590 (size: 0x4)

}; // Size: 0x5B0

#endif

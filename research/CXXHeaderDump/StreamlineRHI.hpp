#ifndef UE4SS_SDK_StreamlineRHI_HPP
#define UE4SS_SDK_StreamlineRHI_HPP

#include "StreamlineRHI_enums.hpp"

class UStreamlineOverrideSettings : public UObject
{
    EStreamlineSettingOverride LoadDebugOverlayOverride;                              // 0x0028 (size: 0x1)
    EStreamlineSettingOverride AllowOTAUpdateOverride;                                // 0x0029 (size: 0x1)
    EStreamlineSettingOverride EnableDLSSFGInPlayInEditorViewportsOverride;           // 0x002A (size: 0x1)
    EStreamlineSettingOverride UseSlSetTagOverride;                                   // 0x002B (size: 0x1)

}; // Size: 0x30

class UStreamlineSettings : public UObject
{
    bool bLoadDebugOverlay;                                                           // 0x0028 (size: 0x1)
    bool bAllowOTAUpdate;                                                             // 0x0029 (size: 0x1)
    int32 NVIDIANGXApplicationId;                                                     // 0x002C (size: 0x4)
    bool bEnableStreamlineD3D12;                                                      // 0x0030 (size: 0x1)
    bool bEnableStreamlineD3D11;                                                      // 0x0031 (size: 0x1)
    bool bEnableDLSSFGInPlayInEditorViewports;                                        // 0x0032 (size: 0x1)
    bool bUseSlSetTag;                                                                // 0x0033 (size: 0x1)

}; // Size: 0x38

#endif

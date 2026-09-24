#ifndef UE4SS_SDK_DLSS_HPP
#define UE4SS_SDK_DLSS_HPP

#include "DLSS_enums.hpp"

struct FDLSSUpscalerModularFeatureSettings
{
    EDLSSUpscalerModularFeatureQuality Quality;                                       // 0x0000 (size: 0x1)

}; // Size: 0x1

class UDLSSOverrideSettings : public UObject
{
    bool bShowDLSSIncompatiblePluginsToolsWarnings;                                   // 0x0028 (size: 0x1)
    EDLSSSettingOverride ShowDLSSSDebugOnScreenMessages;                              // 0x0029 (size: 0x1)
    EDLSSSettingOverride EnableDLSSInEditorViewportsOverride;                         // 0x002A (size: 0x1)
    EDLSSSettingOverride EnableDLSSInPlayInEditorViewportsOverride;                   // 0x002B (size: 0x1)

}; // Size: 0x30

class UDLSSSettings : public UObject
{
    bool bAllowOTAUpdate;                                                             // 0x0028 (size: 0x1)
    uint32 NVIDIANGXApplicationId;                                                    // 0x002C (size: 0x4)
    uint8 BiasCurrentColorStencilValue;                                               // 0x0030 (size: 0x1)
    bool bEnableDLSSD3D12;                                                            // 0x0031 (size: 0x1)
    bool bEnableDLSSD3D11;                                                            // 0x0032 (size: 0x1)
    bool bEnableDLSSVulkan;                                                           // 0x0033 (size: 0x1)
    bool bShowDLSSIncompatiblePluginsToolsWarnings;                                   // 0x0034 (size: 0x1)
    bool bEnableDLSSInEditorViewports;                                                // 0x0035 (size: 0x1)
    bool bEnableDLSSInPlayInEditorViewports;                                          // 0x0036 (size: 0x1)
    bool bShowDLSSSDebugOnScreenMessages;                                             // 0x0037 (size: 0x1)
    FString GenericDLSSSRBinaryPath;                                                  // 0x0038 (size: 0x10)
    bool bGenericDLSSSRBinaryExists;                                                  // 0x0048 (size: 0x1)
    FString CustomDLSSSRBinaryPath;                                                   // 0x0050 (size: 0x10)
    bool bCustomDLSSSRBinaryExists;                                                   // 0x0060 (size: 0x1)
    EDLSSPreset DLAAPreset;                                                           // 0x0061 (size: 0x1)
    EDLSSPreset DLSSQualityPreset;                                                    // 0x0063 (size: 0x1)
    EDLSSPreset DLSSBalancedPreset;                                                   // 0x0064 (size: 0x1)
    EDLSSPreset DLSSPerformancePreset;                                                // 0x0065 (size: 0x1)
    EDLSSPreset DLSSUltraPerformancePreset;                                           // 0x0066 (size: 0x1)
    FString GenericDLSSRRBinaryPath;                                                  // 0x0068 (size: 0x10)
    bool bGenericDLSSRRBinaryExists;                                                  // 0x0078 (size: 0x1)
    FString CustomDLSSRRBinaryPath;                                                   // 0x0080 (size: 0x10)
    bool bCustomDLSSRRBinaryExists;                                                   // 0x0090 (size: 0x1)
    EDLSSRRPreset DLAARRPreset;                                                       // 0x0091 (size: 0x1)
    EDLSSRRPreset DLSSRRQualityPreset;                                                // 0x0093 (size: 0x1)
    EDLSSRRPreset DLSSRRBalancedPreset;                                               // 0x0094 (size: 0x1)
    EDLSSRRPreset DLSSRRPerformancePreset;                                            // 0x0095 (size: 0x1)
    EDLSSRRPreset DLSSRRUltraPerformancePreset;                                       // 0x0096 (size: 0x1)

}; // Size: 0x98

#endif

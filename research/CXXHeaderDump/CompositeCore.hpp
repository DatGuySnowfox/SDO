#ifndef UE4SS_SDK_CompositeCore_HPP
#define UE4SS_SDK_CompositeCore_HPP

#include "CompositeCore_enums.hpp"

class UCompositeCorePluginSettings : public UDeveloperSettings
{
    uint8 bApplyPreExposure;                                                          // 0x0038 (size: 0x1)
    uint8 bApplyFXAA;                                                                 // 0x0038 (size: 0x1)
    TArray<FSoftClassPath> DisabledPrimitiveClasses;                                  // 0x0040 (size: 0x10)
    TArray<FSoftClassPath> AllowedComponentClasses;                                   // 0x0050 (size: 0x10)
    int32 SceneViewExtensionPriority;                                                 // 0x0060 (size: 0x4)

}; // Size: 0xC0

class UCompositeCoreSubsystem : public UTickableWorldSubsystem
{

    void UnregisterPrimitive(class UPrimitiveComponent* InPrimitiveComponent);
    void RegisterPrimitive(class UPrimitiveComponent* InPrimitiveComponent);
}; // Size: 0x50

class UHoldoutCompositeComponent : public USceneComponent
{
    bool bIsEnabled;                                                                  // 0x0240 (size: 0x1)

    void SetEnabled(bool bInEnabled);
    bool IsEnabled();
}; // Size: 0x250

#endif

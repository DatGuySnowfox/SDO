#ifndef UE4SS_SDK_AudioModulation_HPP
#define UE4SS_SDK_AudioModulation_HPP

#include "AudioModulation_enums.hpp"

struct FEnvelopeFollowerGeneratorParams
{
    bool bBypass;                                                                     // 0x0000 (size: 0x1)
    bool bInvert;                                                                     // 0x0001 (size: 0x1)
    class UAudioBus* AudioBus;                                                        // 0x0008 (size: 0x8)
    float Gain;                                                                       // 0x0010 (size: 0x4)
    float AttackTime;                                                                 // 0x0014 (size: 0x4)
    float ReleaseTime;                                                                // 0x0018 (size: 0x4)

}; // Size: 0x20

struct FSoundControlBusMixStage
{
    class USoundControlBus* Bus;                                                      // 0x0000 (size: 0x8)
    FSoundModulationMixValue Value;                                                   // 0x0008 (size: 0x20)

}; // Size: 0x28

struct FSoundControlModulationInput
{
    uint8 bSampleAndHold;                                                             // 0x0000 (size: 0x1)
    FSoundModulationTransform Transform;                                              // 0x0008 (size: 0xB8)
    class USoundControlBus* Bus;                                                      // 0x00C0 (size: 0x8)

}; // Size: 0xC8

struct FSoundControlModulationPatch
{
    bool bBypass;                                                                     // 0x0000 (size: 0x1)
    class USoundModulationParameter* OutputParameter;                                 // 0x0008 (size: 0x8)
    TArray<FSoundControlModulationInput> Inputs;                                      // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FSoundModulationADEnvelopeParams
{
    float AttackTime;                                                                 // 0x0000 (size: 0x4)
    float DecayTime;                                                                  // 0x0004 (size: 0x4)
    float AttackCurve;                                                                // 0x0008 (size: 0x4)
    float DecayCurve;                                                                 // 0x000C (size: 0x4)
    bool bLooping;                                                                    // 0x0010 (size: 0x1)
    bool bBypass;                                                                     // 0x0011 (size: 0x1)

}; // Size: 0x14

struct FSoundModulationLFOParams
{
    ESoundModulationLFOShape Shape;                                                   // 0x0000 (size: 0x1)
    float ExponentialFactor;                                                          // 0x0004 (size: 0x4)
    float Width;                                                                      // 0x0008 (size: 0x4)
    float Amplitude;                                                                  // 0x000C (size: 0x4)
    float Frequency;                                                                  // 0x0010 (size: 0x4)
    float Offset;                                                                     // 0x0014 (size: 0x4)
    float Phase;                                                                      // 0x0018 (size: 0x4)
    bool bLooping;                                                                    // 0x001C (size: 0x1)
    bool bBypass;                                                                     // 0x001D (size: 0x1)

}; // Size: 0x20

struct FSoundModulationMixValue
{
    float TargetValue;                                                                // 0x0000 (size: 0x4)
    float AttackTime;                                                                 // 0x0004 (size: 0x4)
    float ReleaseTime;                                                                // 0x0008 (size: 0x4)

}; // Size: 0x20

struct FSoundModulationParameterSettings
{
    float ValueNormalized;                                                            // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FSoundModulationTransform : public FWaveTableTransform
{
}; // Size: 0xB8

class UAudioModulationDestination : public UObject
{
    class USoundModulatorBase* Modulator;                                             // 0x0028 (size: 0x8)

    bool SetModulator(const class USoundModulatorBase* InModulator);
    float GetValue();
    class USoundModulatorBase* GetModulator();
    bool ClearModulator();
}; // Size: 0x40

class UAudioModulationSettings : public UDeveloperSettings
{
    TArray<FSoftObjectPath> Parameters;                                               // 0x0038 (size: 0x10)

}; // Size: 0x48

class UAudioModulationStatics : public UBlueprintFunctionLibrary
{

    void UpdateModulator(const class UObject* WorldContextObject, class USoundModulatorBase* Modulator);
    void UpdateMixFromObject(const class UObject* WorldContextObject, class USoundControlBusMix* Mix, float FadeTime);
    void UpdateMixByFilter(const class UObject* WorldContextObject, class USoundControlBusMix* Mix, FString AddressFilter, TSubclassOf<class USoundModulationParameter> ParamClassFilter, class USoundModulationParameter* ParamFilter, float Value, float FadeTime);
    void UpdateMix(const class UObject* WorldContextObject, class USoundControlBusMix* Mix, TArray<FSoundControlBusMixStage> Stages, float FadeTime, double Duration, bool bRetriggerOnActivation);
    void SetGlobalBusMixValue(const class UObject* WorldContextObject, class USoundControlBus* Bus, float Value, float FadeTime);
    void SaveMixToProfile(const class UObject* WorldContextObject, class USoundControlBusMix* Mix, int32 ProfileIndex);
    TArray<FSoundControlBusMixStage> LoadMixFromProfile(const class UObject* WorldContextObject, class USoundControlBusMix* Mix, bool bActivate, int32 ProfileIndex);
    bool IsControlBusMixActive(const class UObject* WorldContextObject, class USoundControlBusMix* Mix);
    float GetModulatorValue(const class UObject* WorldContextObject, class USoundModulatorBase* Modulator);
    TSet<USoundModulatorBase*> GetModulatorsFromDestination(const FSoundModulationDestinationSettings& Destination);
    void DeactivateGenerator(const class UObject* WorldContextObject, class USoundModulationGenerator* Generator);
    void DeactivateBusMix(const class UObject* WorldContextObject, class USoundControlBusMix* Mix);
    void DeactivateBus(const class UObject* WorldContextObject, class USoundControlBus* Bus);
    void DeactivateAllBusMixes(const class UObject* WorldContextObject);
    class USoundModulationParameter* CreateModulationParameter(class UObject* WorldContextObject, FName Name, TSubclassOf<class USoundModulationParameter> ParamClass, float DefaultValue);
    class UAudioModulationDestination* CreateModulationDestination(class UObject* WorldContextObject, FName Name, class USoundModulatorBase* Modulator);
    class USoundModulationGeneratorLFO* CreateLFOGenerator(class UObject* WorldContextObject, FName Name, FSoundModulationLFOParams Params);
    class USoundModulationGeneratorEnvelopeFollower* CreateEnvelopeFollowerGenerator(class UObject* WorldContextObject, FName Name, FEnvelopeFollowerGeneratorParams Params);
    FSoundControlBusMixStage CreateBusMixStage(const class UObject* WorldContextObject, class USoundControlBus* Bus, float Value, float AttackTime, float ReleaseTime);
    class USoundControlBusMix* CreateBusMixFromValue(const class UObject* WorldContextObject, FName Name, const TArray<USoundControlBus*>& Buses, float Value, float AttackTime, float ReleaseTime, bool bActivate);
    class USoundControlBusMix* CreateBusMix(class UObject* WorldContextObject, FName Name, TArray<FSoundControlBusMixStage> Stages, bool Activate, double Duration, bool bRetriggerOnActivation);
    class USoundControlBus* CreateBus(class UObject* WorldContextObject, FName Name, class USoundModulationParameter* Parameter, bool Activate);
    class USoundModulationGeneratorADEnvelope* CreateADEnvelopeGenerator(class UObject* WorldContextObject, FName Name, const FSoundModulationADEnvelopeParams& Params);
    void ClearGlobalBusMixValue(const class UObject* WorldContextObject, class USoundControlBus* Bus, float FadeTime);
    void ClearAllGlobalBusMixValues(const class UObject* WorldContextObject, float FadeTime);
    void ActivateGenerator(const class UObject* WorldContextObject, class USoundModulationGenerator* Generator);
    void ActivateBusMix(const class UObject* WorldContextObject, class USoundControlBusMix* Mix);
    void ActivateBus(const class UObject* WorldContextObject, class USoundControlBus* Bus);
}; // Size: 0x28

class UAudioModulationStyle : public UBlueprintFunctionLibrary
{

    FColor GetPatchColor();
    FColor GetParameterColor();
    FColor GetModulationGeneratorColor();
    FColor GetControlBusMixColor();
    FColor GetControlBusColor();
}; // Size: 0x28

class USoundControlBus : public USoundModulatorBase
{
    bool bBypass;                                                                     // 0x0030 (size: 0x1)
    FString Address;                                                                  // 0x0038 (size: 0x10)
    TArray<USoundModulationGenerator*> Generators;                                    // 0x0048 (size: 0x10)
    class USoundModulationParameter* Parameter;                                       // 0x0058 (size: 0x8)

}; // Size: 0x60

class USoundControlBusMix : public UObject
{
    uint32 ProfileIndex;                                                              // 0x0028 (size: 0x4)
    double Duration;                                                                  // 0x0030 (size: 0x8)
    bool bRetriggerOnActivation;                                                      // 0x0038 (size: 0x1)
    TArray<FSoundControlBusMixStage> MixStages;                                       // 0x0040 (size: 0x10)

    void SoloMix();
    void SaveMixToProfile();
    void LoadMixFromProfile();
    void DeactivateMix();
    void DeactivateAllMixes();
    void ActivateMix();
}; // Size: 0x50

class USoundModulationGenerator : public USoundModulatorBase
{
}; // Size: 0x30

class USoundModulationGeneratorADEnvelope : public USoundModulationGenerator
{
    FSoundModulationADEnvelopeParams Params;                                          // 0x0030 (size: 0x14)

}; // Size: 0x48

class USoundModulationGeneratorEnvelopeFollower : public USoundModulationGenerator
{
    FEnvelopeFollowerGeneratorParams Params;                                          // 0x0030 (size: 0x20)

}; // Size: 0x50

class USoundModulationGeneratorLFO : public USoundModulationGenerator
{
    FSoundModulationLFOParams Params;                                                 // 0x0030 (size: 0x20)

}; // Size: 0x50

class USoundModulationParameter : public UObject
{
    FSoundModulationParameterSettings Settings;                                       // 0x0030 (size: 0x4)

}; // Size: 0x38

class USoundModulationParameterAdditive : public USoundModulationParameter
{
    float UnitMin;                                                                    // 0x0038 (size: 0x4)
    float UnitMax;                                                                    // 0x003C (size: 0x4)

}; // Size: 0x40

class USoundModulationParameterBipolar : public USoundModulationParameter
{
    float UnitRange;                                                                  // 0x0038 (size: 0x4)

}; // Size: 0x40

class USoundModulationParameterFilterFrequency : public USoundModulationParameterFrequencyBase
{
}; // Size: 0x38

class USoundModulationParameterFrequency : public USoundModulationParameterFrequencyBase
{
    float UnitMin;                                                                    // 0x0038 (size: 0x4)
    float UnitMax;                                                                    // 0x003C (size: 0x4)

}; // Size: 0x40

class USoundModulationParameterFrequencyBase : public USoundModulationParameter
{
}; // Size: 0x38

class USoundModulationParameterHPFFrequency : public USoundModulationParameterFilterFrequency
{
}; // Size: 0x38

class USoundModulationParameterLPFFrequency : public USoundModulationParameterFilterFrequency
{
}; // Size: 0x38

class USoundModulationParameterScaled : public USoundModulationParameter
{
    float UnitMin;                                                                    // 0x0038 (size: 0x4)
    float UnitMax;                                                                    // 0x003C (size: 0x4)

}; // Size: 0x40

class USoundModulationParameterVolume : public USoundModulationParameter
{
    float MinVolume;                                                                  // 0x0038 (size: 0x4)

}; // Size: 0x40

class USoundModulationPatch : public USoundModulatorBase
{
    FSoundControlModulationPatch PatchSettings;                                       // 0x0030 (size: 0x20)

}; // Size: 0x50

#endif

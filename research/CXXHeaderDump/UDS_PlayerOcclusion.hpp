#ifndef UE4SS_SDK_UDS_PlayerOcclusion_HPP
#define UE4SS_SDK_UDS_PlayerOcclusion_HPP

class UUDS_PlayerOcclusion_C : public USceneComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0240 (size: 0x8)
    class UUDS_OcclusionSettings_C* Occlusion Settings;                               // 0x0248 (size: 0x8)
    bool Force Full Occlusion;                                                        // 0x0250 (size: 0x1)
    bool Calculate Directional Occlusion;                                             // 0x0251 (size: 0x1)
    TArray<AActor*> Ignored Actors;                                                   // 0x0258 (size: 0x10)
    bool Hard Update;                                                                 // 0x0268 (size: 0x1)
    double Tick Period;                                                               // 0x0270 (size: 0x8)
    FVector Sample Location;                                                          // 0x0278 (size: 0x18)
    FUDS_PlayerOcclusion_CHard Occlusion Change Hard Occlusion Change;                // 0x0290 (size: 0x10)
    void Hard Occlusion Change();
    class UUDS_OcclusionState_C* State;                                               // 0x02A0 (size: 0x8)
    TArray<UUDS_Occlusion_Portal_C*> Portal Components;                               // 0x02A8 (size: 0x10)
    class UNiagaraComponent* DF Occlusion System;                                     // 0x02B8 (size: 0x8)
    TSoftObjectPtr<class UNiagaraSystem> DF Occlusion System Asset;                   // 0x02C0 (size: 0x28)

    void Apply Directional Occlusion Modifiers(FLinearColor In, FLinearColor& Out);
    void Start Up GPU Distance Field System();
    void Test Point for Occlusion Volumes(FVector Location, double& Final Multiplier);
    void Custom Global Occlusion Sample(FVector Location, double& Global Occlusion);
    void Runtime Tick();
    TEnumAsByte<EDrawDebugTrace::Type> Trace Debugs();
    void Editor Tick(FVector Occlusion Location);
    void Portal Direction Mask(FVector Direction, FLinearColor& Mask);
    void Delayed Startup();
    void Check For Portal Components();
    void Get Sample Location();
    void Update Ignored Actors();
    void Periodic Context Checks();
    void Update Current Occlusion();
    void Incremental Occlusion Traces();
    void Initialize();
    void ReceiveParticleData(const TArray<FBasicParticleData>& Data, class UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset);
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void ExecuteUbergraph_UDS_PlayerOcclusion(int32 EntryPoint);
    void Hard Occlusion Change__DelegateSignature();
}; // Size: 0x2E8

#endif

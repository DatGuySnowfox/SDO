#ifndef UE4SS_SDK_QC_Attackers_HPP
#define UE4SS_SDK_QC_Attackers_HPP

class UQC_Attackers_C : public UEnvQueryContext_BlueprintBase
{

    void ProvideActorsSet(class UObject* QuerierObject, class AActor* QuerierActor, TArray<class AActor*>& ResultingActorsSet);
}; // Size: 0x30

#endif

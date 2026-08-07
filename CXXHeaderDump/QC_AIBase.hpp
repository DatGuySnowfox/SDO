#ifndef UE4SS_SDK_QC_AIBase_HPP
#define UE4SS_SDK_QC_AIBase_HPP

class UQC_AIBase_C : public UEnvQueryContext_BlueprintBase
{

    void ProvideSingleLocation(class UObject* QuerierObject, class AActor* QuerierActor, FVector& ResultingLocation);
}; // Size: 0x30

#endif

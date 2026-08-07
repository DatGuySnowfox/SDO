#ifndef UE4SS_SDK_AnimationBudgetAllocator_HPP
#define UE4SS_SDK_AnimationBudgetAllocator_HPP

struct FAnimationBudgetAllocatorParameters
{
    float BudgetInMs;                                                                 // 0x0000 (size: 0x4)
    float MinQuality;                                                                 // 0x0004 (size: 0x4)
    int32 MaxTickRate;                                                                // 0x0008 (size: 0x4)
    float WorkUnitSmoothingSpeed;                                                     // 0x000C (size: 0x4)
    float AlwaysTickFalloffAggression;                                                // 0x0010 (size: 0x4)
    float InterpolationFalloffAggression;                                             // 0x0014 (size: 0x4)
    int32 InterpolationMaxRate;                                                       // 0x0018 (size: 0x4)
    int32 MaxInterpolatedComponents;                                                  // 0x001C (size: 0x4)
    float InterpolationTickMultiplier;                                                // 0x0020 (size: 0x4)
    float InitialEstimatedWorkUnitTimeMs;                                             // 0x0024 (size: 0x4)
    int32 MaxTickedOffsreenComponents;                                                // 0x0028 (size: 0x4)
    int32 StateChangeThrottleInFrames;                                                // 0x002C (size: 0x4)
    float BudgetFactorBeforeReducedWork;                                              // 0x0030 (size: 0x4)
    float BudgetFactorBeforeReducedWorkEpsilon;                                       // 0x0034 (size: 0x4)
    float BudgetPressureSmoothingSpeed;                                               // 0x0038 (size: 0x4)
    int32 ReducedWorkThrottleMinInFrames;                                             // 0x003C (size: 0x4)
    int32 ReducedWorkThrottleMaxInFrames;                                             // 0x0040 (size: 0x4)
    float BudgetFactorBeforeAggressiveReducedWork;                                    // 0x0044 (size: 0x4)
    int32 ReducedWorkThrottleMaxPerFrame;                                             // 0x0048 (size: 0x4)
    float BudgetPressureBeforeEmergencyReducedWork;                                   // 0x004C (size: 0x4)
    float AutoCalculatedSignificanceMaxDistance;                                      // 0x0050 (size: 0x4)

}; // Size: 0x58

class UAnimationBudgetBlueprintLibrary : public UBlueprintFunctionLibrary
{

    void SetAnimationBudgetParameters(class UObject* WorldContextObject, const FAnimationBudgetAllocatorParameters& InParameters);
    void EnableAnimationBudget(class UObject* WorldContextObject, bool bEnabled);
}; // Size: 0x28

class USkeletalMeshComponentBudgeted : public USkeletalMeshComponent
{
    uint8 bAutoRegisterWithBudgetAllocator;                                           // 0x0FC0 (size: 0x1)
    uint8 bAutoCalculateSignificance;                                                 // 0x0FC0 (size: 0x1)
    uint8 bShouldUseActorRenderedFlag;                                                // 0x0FC0 (size: 0x1)

    void SetAutoRegisterWithBudgetAllocator(bool bInAutoRegisterWithBudgetAllocator);
}; // Size: 0xFD0

#endif

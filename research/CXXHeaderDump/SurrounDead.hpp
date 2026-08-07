#ifndef UE4SS_SDK_SurrounDead_HPP
#define UE4SS_SDK_SurrounDead_HPP

struct FActor_Dist
{
    float Distance;                                                                   // 0x0000 (size: 0x4)
    class AActor* Actor;                                                              // 0x0008 (size: 0x8)

}; // Size: 0x10

class UCPlusPlusFunctionLibrary : public UBlueprintFunctionLibrary
{

    bool IsStreamingTextures(class UPrimitiveComponent* TargetComp);
}; // Size: 0x28

class UDistanceSort : public UBlueprintFunctionLibrary
{

    void Distance_Sort(TArray<class AActor*> Array_To_Sort, class AActor* From_Actor, bool Descending, TArray<FActor_Dist>& Sorted_Array);
}; // Size: 0x28

class UGetStreamedLevel : public UBlueprintFunctionLibrary
{

    FName GetActorStreamingLevelName(class AActor* Actor);
}; // Size: 0x28

#endif

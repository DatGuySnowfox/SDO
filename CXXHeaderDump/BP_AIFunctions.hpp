#ifndef UE4SS_SDK_BP_AIFunctions_HPP
#define UE4SS_SDK_BP_AIFunctions_HPP

class UBP_AIFunctions_C : public UBlueprintFunctionLibrary
{

    void Same Team?(class UBP_SmartAIComponent_C* AI 1, class UBP_SmartAIComponent_C* AI 2, class UObject* __WorldContext, bool& Match);
    void Is Hitbox?(FName Bone Name, class UObject* __WorldContext, TEnumAsByte<E_HitboxTypeAI::Type>& HitBox);
}; // Size: 0x28

#endif

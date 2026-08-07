#ifndef UE4SS_SDK_INT_AnimBPPlayer_HPP
#define UE4SS_SDK_INT_AnimBPPlayer_HPP

class IINT_AnimBPPlayer_C : public IInterface
{

    void DeathState(bool Dead);
    void CombatState(int32 BlendSpace);
}; // Size: 0x28

#endif

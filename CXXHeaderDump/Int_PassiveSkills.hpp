#ifndef UE4SS_SDK_Int_PassiveSkills_HPP
#define UE4SS_SDK_Int_PassiveSkills_HPP

class IInt_PassiveSkills_C : public IInterface
{

    void XPMultiplier_Scavenging(bool& Used?);
    void AddXP_Scavenging(double float);
    void XPMultiplier_Fishing(bool& Used?);
    void XPMultiplier_Thief(bool& Used?);
    void XPMultiplier_Reloading(bool& Used?);
    void XPMultiplier_Marksmanship(bool& Used?);
    void XPMultiplier_FirstAid(bool& Used?);
    void XPMultiplier_Sneaking(bool& Used?);
    void XPMultiplier_Toughness(bool& Used?);
    void XPMultiplier_Strength(bool& Used?);
    void XPMultiplier_Fitness(bool& Used?);
    void AddXP_Fishing(double float);
    void AddXP_Thief(double float);
    void AddXP_Reloading(double float);
    void AddXP_Marksmanship(double float);
    void AddXP_FirstAid(double float);
    void AddXP_Sneaking(double float);
    void AddXP_Toughness(double float);
    void AddXP_Strength(double float);
    void AddXP_Fitness(double float);
}; // Size: 0x28

#endif

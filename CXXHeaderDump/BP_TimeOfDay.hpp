#ifndef UE4SS_SDK_BP_TimeOfDay_HPP
#define UE4SS_SDK_BP_TimeOfDay_HPP

class ABP_TimeOfDay_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A0 (size: 0x8)
    class ADirectionalLight* Directional Light;                                       // 0x02A8 (size: 0x8)
    class ASkyLight* SkyLight;                                                        // 0x02B0 (size: 0x8)
    class ABP_Sky_Sphere_C* Sky Sphere;                                               // 0x02B8 (size: 0x8)
    double Sun Speed;                                                                 // 0x02C0 (size: 0x8)
    double Tick;                                                                      // 0x02C8 (size: 0x8)
    FTimespan Time;                                                                   // 0x02D0 (size: 0x8)
    double New Time Delta;                                                            // 0x02D8 (size: 0x8)
    bool Night;                                                                       // 0x02E0 (size: 0x1)
    FBP_TimeOfDay_CTime Hour Time Hour;                                               // 0x02E8 (size: 0x10)
    void Time Hour(int32 Hours);
    bool Debug Text;                                                                  // 0x02F8 (size: 0x1)

    void AI Is Dead?(bool& Dead);
    void AI Can Interact?(bool& Can Interact);
    void Actor Attack Target(class AActor*& Attack Target);
    void Update Time(double Delta);
    void Day Lighting(double Delta);
    void Night Lighting(double Delta);
    void Turret Idle Start();
    void Turret Idle Stop();
    void Turret Start Reload();
    void Turret End Reload();
    void AI Alert(class AActor* Alert Actor);
    void AI End Alert();
    void AI Animation Switch(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void AI Camera Activate(bool Activate);
    void AI Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void ReceiveTick(float DeltaSeconds);
    void Turret Destroyed Effect();
    void Alert Actor Defenders(class AActor* Attacked, class AActor* Attacker);
    void Actor Aim Focus(class AActor* Instigator);
    void AI Dead();
    void ExecuteUbergraph_BP_TimeOfDay(int32 EntryPoint);
    void Time Hour__DelegateSignature(int32 Hours);
}; // Size: 0x2F9

#endif

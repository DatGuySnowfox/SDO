#ifndef UE4SS_SDK_BP_MasterWayPoint_HPP
#define UE4SS_SDK_BP_MasterWayPoint_HPP

class ABP_MasterWayPoint_C : public AActor
{
    class UArrowComponent* Arrow;                                                     // 0x0298 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02A0 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02A8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02B0 (size: 0x8)
    FVector Way Point Face Direction;                                                 // 0x02B8 (size: 0x18)
    double Wait Time Deviation;                                                       // 0x02D0 (size: 0x8)
    double Wait Time;                                                                 // 0x02D8 (size: 0x8)
    TArray<class UAnimMontage*> Way Point Montages;                                   // 0x02E0 (size: 0x10)
    TEnumAsByte<E_OrderOptions::Type> Animations Order;                               // 0x02F0 (size: 0x1)
    bool Loop Animations;                                                             // 0x02F1 (size: 0x1)
    bool Rotate To Face Direction;                                                    // 0x02F2 (size: 0x1)
    bool Hide Rotation Arrow;                                                         // 0x02F3 (size: 0x1)
    bool Change AI Behaviour;                                                         // 0x02F4 (size: 0x1)
    TEnumAsByte<E_StartingAIBehaviours::Type> New Behaviour;                          // 0x02F5 (size: 0x1)
    bool Is Temporary Behaviour;                                                      // 0x02F6 (size: 0x1)
    double Temporary Time;                                                            // 0x02F8 (size: 0x8)
    bool Show In Game;                                                                // 0x0300 (size: 0x1)

    void UserConstructionScript();
}; // Size: 0x301

#endif

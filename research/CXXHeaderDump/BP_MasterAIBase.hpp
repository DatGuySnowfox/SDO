#ifndef UE4SS_SDK_BP_MasterAIBase_HPP
#define UE4SS_SDK_BP_MasterAIBase_HPP

class ABP_MasterAIBase_C : public AActor
{
    class USphereComponent* Debug Range Sphere;                                       // 0x0298 (size: 0x8)
    class UCapsuleComponent* Capsule;                                                 // 0x02A0 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02A8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02B0 (size: 0x8)
    double Range Sphere Radius;                                                       // 0x02B8 (size: 0x8)

    void UserConstructionScript();
}; // Size: 0x2C0

#endif

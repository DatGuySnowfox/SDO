#ifndef UE4SS_SDK_BP_MasterAICharacter_HPP
#define UE4SS_SDK_BP_MasterAICharacter_HPP

class ABP_MasterAICharacter_C : public ACharacter
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0680 (size: 0x8)
    class UNavigationInvokerComponent* NavigationInvoker;                             // 0x0688 (size: 0x8)
    class USceneComponent* RangeStartPoint;                                           // 0x0690 (size: 0x8)
    class UBP_SmartAIComponent_C* BP_AIComponent;                                     // 0x0698 (size: 0x8)
    float Edge_Timeline_NewTrack_0_E35BA4E241D5E59A8EB450B4FEA52937;                  // 0x06A0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Edge_Timeline__Direction_E35BA4E241D5E59A8EB450B4FEA52937; // 0x06A4 (size: 0x1)
    class UTimelineComponent* Edge Timeline;                                          // 0x06A8 (size: 0x8)
    float Smooth_Move_Lerp_C8A3EC80484780C3B672F688B96D1C2A;                          // 0x06B0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Smooth_Move__Direction_C8A3EC80484780C3B672F688B96D1C2A; // 0x06B4 (size: 0x1)
    class UTimelineComponent* Smooth Move;                                            // 0x06B8 (size: 0x8)
    float Valt_Timeline_Up_Alpha_D0168D9449470A31646DA0ADA61D8223;                    // 0x06C0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Valt_Timeline__Direction_D0168D9449470A31646DA0ADA61D8223; // 0x06C4 (size: 0x1)
    class UTimelineComponent* Valt Timeline;                                          // 0x06C8 (size: 0x8)
    float Climb_Timeline_Across_Alpha_ABEC2831489E1A5D1118BC8DC7BB5664;               // 0x06D0 (size: 0x4)
    float Climb_Timeline_Up_Alpha_ABEC2831489E1A5D1118BC8DC7BB5664;                   // 0x06D4 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Climb_Timeline__Direction_ABEC2831489E1A5D1118BC8DC7BB5664; // 0x06D8 (size: 0x1)
    class UTimelineComponent* Climb Timeline;                                         // 0x06E0 (size: 0x8)
    FString ArgumentForQuest;                                                         // 0x06E8 (size: 0x10)
    bool Debug Mode;                                                                  // 0x06F8 (size: 0x1)
    double Debug Text Duration;                                                       // 0x0700 (size: 0x8)
    class ABP_AIWeapon_Master_C* Holdable Actor;                                      // 0x0708 (size: 0x8)
    FVector End Climb Location;                                                       // 0x0710 (size: 0x18)
    FVector End Climb Hight;                                                          // 0x0728 (size: 0x18)
    bool Up Movement Done;                                                            // 0x0740 (size: 0x1)
    FVector Edge Impact Point;                                                        // 0x0748 (size: 0x18)
    FVector Edge Start Location;                                                      // 0x0760 (size: 0x18)
    FTransform End Location;                                                          // 0x0780 (size: 0x60)
    FTransform Start Transform;                                                       // 0x07E0 (size: 0x60)
    FVector Start Climb Location;                                                     // 0x0840 (size: 0x18)
    FBP_MasterAICharacter_CBeginPlay BeginPlay;                                       // 0x0858 (size: 0x10)
    void BeginPlay();
    bool XpPopUpEnabled?;                                                             // 0x0868 (size: 0x1)

    void ActorDead?(bool& Dead?);
    void Actor Attack Target(class AActor*& Attack Target);
    void AI Can Interact?(bool& Can Interact);
    void AI Is Dead?(bool& Dead);
    void Debug Text(FString InString, FLinearColor TextColor);
    void Climb Timeline__FinishedFunc();
    void Climb Timeline__UpdateFunc();
    void Valt Timeline__FinishedFunc();
    void Valt Timeline__UpdateFunc();
    void Smooth Move__FinishedFunc();
    void Smooth Move__UpdateFunc();
    void Edge Timeline__FinishedFunc();
    void Edge Timeline__UpdateFunc();
    void AI End Alert();
    void AI Animation Switch(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void AI Camera Activate(bool Activate);
    void AI Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void Actor Aim Focus(class AActor* Instigator);
    void ReceivePossessed(class AController* NewController);
    void OnLanded(const FHitResult& Hit);
    void Climb Location Movement(bool Is Valting, FVector End Climb Location, double Play Rate);
    void Smooth Move AI Actor(FTransform End Location, double Play Time);
    void AI Alert(class AActor* Alert Actor);
    void Turret End Reload();
    void Turret Start Reload();
    void Turret Idle Stop();
    void Turret Idle Start();
    void Turret Destroyed Effect();
    void Alert Actor Defenders(class AActor* Attacked, class AActor* Attacker);
    void ReceiveBeginPlay();
    void GetQuestArgument(FString Argument);
    void AI Dead();
    void Event_BloodSplatter();
    void XP(bool NewValue);
    void ExecuteUbergraph_BP_MasterAICharacter(int32 EntryPoint);
    void BeginPlay__DelegateSignature();
}; // Size: 0x869

#endif

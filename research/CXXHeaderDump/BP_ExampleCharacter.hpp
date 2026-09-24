#ifndef UE4SS_SDK_BP_ExampleCharacter_HPP
#define UE4SS_SDK_BP_ExampleCharacter_HPP

class ABP_ExampleCharacter_C : public ACharacter
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0650 (size: 0x8)
    class UBP_PlayerAIActivator_C* BP_PlayerAIActivator;                              // 0x0658 (size: 0x8)
    class USphereComponent* Gun Muzzal Socket;                                        // 0x0660 (size: 0x8)
    class UStaticMeshComponent* Gun;                                                  // 0x0668 (size: 0x8)
    class UCameraComponent* PlayerCamera;                                             // 0x0670 (size: 0x8)
    class USpringArmComponent* CameraBoom;                                            // 0x0678 (size: 0x8)
    double BaseLookUpRate;                                                            // 0x0680 (size: 0x8)
    bool Dead;                                                                        // 0x0688 (size: 0x1)
    class UBP_HUD_C* HUD;                                                             // 0x0690 (size: 0x8)
    class ABP_ExampleCharacter_C* Respawned Character;                                // 0x0698 (size: 0x8)
    bool Aimed;                                                                       // 0x06A0 (size: 0x1)
    class AActor* Last Attack Target;                                                 // 0x06A8 (size: 0x8)
    double BaseTurnRate;                                                              // 0x06B0 (size: 0x8)
    int32 Health;                                                                     // 0x06B8 (size: 0x4)
    bool Debug Mode;                                                                  // 0x06BC (size: 0x1)
    double Debug Text Duration;                                                       // 0x06C0 (size: 0x8)
    int32 Max Health;                                                                 // 0x06C8 (size: 0x4)
    TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> Surface Emitter;            // 0x06D0 (size: 0x50)
    TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> Surface Decal;           // 0x0720 (size: 0x50)
    FVector Decal Size;                                                               // 0x0770 (size: 0x18)
    double Decal Life Span;                                                           // 0x0788 (size: 0x8)

    void Actor Attack Target(class AActor*& Attack Target);
    void AI Is Dead?(bool& Dead);
    void AI Can Interact?(bool& Can Interact);
    void Set Aimed Focus();
    void Kill Character();
    void Debug Text(FString InString, FLinearColor TextColor);
    void Damage Taken(double Damage);
    void InpActEvt_Jump_K2Node_InputActionEvent_0(FKey Key);
    void InpActEvt_C_K2Node_InputKeyEvent_6(FKey Key);
    void InpActEvt_LeftMouseButton_K2Node_InputKeyEvent_5(FKey Key);
    void InpActEvt_K_K2Node_InputKeyEvent_4(FKey Key);
    void InpActEvt_H_K2Node_InputKeyEvent_3(FKey Key);
    void InpActEvt_RightMouseButton_K2Node_InputKeyEvent_2(FKey Key);
    void InpActEvt_RightMouseButton_K2Node_InputKeyEvent_1(FKey Key);
    void InpActEvt_H_K2Node_InputKeyEvent_0(FKey Key);
    void Turret Idle Stop();
    void Turret Start Reload();
    void Turret End Reload();
    void AI Alert(class AActor* Alert Actor);
    void AI End Alert();
    void AI Animation Switch(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void AI Camera Activate(bool Activate);
    void AI Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void InpAxisEvt_MoveForward_K2Node_InputAxisEvent_180(float AxisValue);
    void InpAxisEvt_MoveRight_K2Node_InputAxisEvent_243(float AxisValue);
    void Turret Idle Start();
    void InpAxisEvt_Turn_K2Node_InputAxisEvent_256(float AxisValue);
    void InpAxisEvt_LookUp_K2Node_InputAxisEvent_268(float AxisValue);
    void Turret Destroyed Effect();
    void Alert Actor Defenders(class AActor* Attacked, class AActor* Attacker);
    void Actor Aim Focus(class AActor* Instigator);
    void ReceiveBeginPlay();
    void AI Dead();
    void InpAxisEvt_TurnRate_K2Node_InputAxisEvent_38(float AxisValue);
    void InpAxisEvt_LookUpRate_K2Node_InputAxisEvent_53(float AxisValue);
    void Server Left Mouse Button(FVector Start, FVector End);
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void Client Open Respawn Screen();
    void Client Respawn Player();
    void Server Respawn Player();
    void Multicast Player Death();
    void Multicast Sound(class USoundBase* Sound, FVector Sound Location);
    void Multicast Range Effect(FHitResult Hit);
    void Multicast Aimed(bool Aimed);
    void Server Aimed(bool Aimed);
    void ExecuteUbergraph_BP_ExampleCharacter(int32 EntryPoint);
}; // Size: 0x790

#endif

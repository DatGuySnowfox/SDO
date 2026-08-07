#ifndef UE4SS_SDK_BP_SmartAIComponent_HPP
#define UE4SS_SDK_BP_SmartAIComponent_HPP

class UBP_SmartAIComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    bool Debug Mode;                                                                  // 0x00A8 (size: 0x1)
    double Debug Text Duration;                                                       // 0x00B0 (size: 0x8)
    TArray<class AActor*> Attackers;                                                  // 0x00B8 (size: 0x10)
    class ABP_MasterInteractionPoint_C* Interaction Point;                            // 0x00C8 (size: 0x8)
    TEnumAsByte<E_AIBehaviour::Type> AI Current Behaviour;                            // 0x00D0 (size: 0x1)
    FVector Still Location;                                                           // 0x00D8 (size: 0x18)
    FVector Still Direction;                                                          // 0x00F0 (size: 0x18)
    class ABP_MasterAIController_C* AI Controller;                                    // 0x0108 (size: 0x8)
    TArray<double> Attackers Forget Time;                                             // 0x0110 (size: 0x10)
    class ACharacter* AI Character;                                                   // 0x0120 (size: 0x8)
    class ABP_MasterWayPoint_C* Way Point;                                            // 0x0128 (size: 0x8)
    bool Dead;                                                                        // 0x0130 (size: 0x1)
    int32 Health;                                                                     // 0x0134 (size: 0x4)
    double Accuracy;                                                                  // 0x0138 (size: 0x8)
    bool Combat Equip Weapon;                                                         // 0x0140 (size: 0x1)
    bool Only Melee When No Ammo;                                                     // 0x0141 (size: 0x1)
    TArray<class UAnimSequenceBase*> Death Animations;                                // 0x0148 (size: 0x10)
    TArray<class ABP_MasterInteractionPoint_C*> Specified Interaction Point;          // 0x0158 (size: 0x10)
    bool Use Interaction Point Tags;                                                  // 0x0168 (size: 0x1)
    TArray<FName> Interaction Point Tags;                                             // 0x0170 (size: 0x10)
    double Interaction Point Range;                                                   // 0x0180 (size: 0x8)
    bool Use Between IP Behavior;                                                     // 0x0188 (size: 0x1)
    TEnumAsByte<E_AIBehaviour::Type> Between IP Behaviour;                            // 0x0189 (size: 0x1)
    double Between IP Behaviour Time;                                                 // 0x0190 (size: 0x8)
    class AActor* AI Base;                                                            // 0x0198 (size: 0x8)
    double AI Base Distance;                                                          // 0x01A0 (size: 0x8)
    int32 Flee Low Health;                                                            // 0x01A8 (size: 0x4)
    bool Flee When Found;                                                             // 0x01AC (size: 0x1)
    bool Face Attacker When Hidden;                                                   // 0x01AD (size: 0x1)
    bool Should Hide;                                                                 // 0x01AE (size: 0x1)
    double Hide Time Length;                                                          // 0x01B0 (size: 0x8)
    TArray<class UAnimMontage*> Hidden Montages;                                      // 0x01B8 (size: 0x10)
    double Forget Attackers Time;                                                     // 0x01C8 (size: 0x8)
    bool Forget Attackers;                                                            // 0x01D0 (size: 0x1)
    bool Return To Start Location;                                                    // 0x01D1 (size: 0x1)
    TEnumAsByte<E_FleeSetting::Type> Flee Type;                                       // 0x01D2 (size: 0x1)
    FTimerHandle Temp Behavior Timer;                                                 // 0x01D8 (size: 0x8)
    bool Can Melee Attack;                                                            // 0x01E0 (size: 0x1)
    double Basic Melee Radius;                                                        // 0x01E8 (size: 0x8)
    bool Basic Melee Collision;                                                       // 0x01F0 (size: 0x1)
    TEnumAsByte<E_OrderOptions::Type> Melee Animation Order;                          // 0x01F1 (size: 0x1)
    TArray<FS_MeleeAnimations> Melee Animations;                                      // 0x01F8 (size: 0x10)
    double Melee Distance;                                                            // 0x0208 (size: 0x8)
    double Melee Switch Distance;                                                     // 0x0210 (size: 0x8)
    TArray<FS_MeleeAnimations> Lower Melee Animations;                                // 0x0218 (size: 0x10)
    TMap<TEnumAsByte<E_HitboxTypeAI::Type>, double> Melee Damage;                     // 0x0228 (size: 0x50)
    double Melee Attack Frequancy;                                                    // 0x0278 (size: 0x8)
    bool Use Melee Anim Time Frequency;                                               // 0x0280 (size: 0x1)
    bool Can Melee Attack While Moving;                                               // 0x0281 (size: 0x1)
    class AActor* Attack Target;                                                      // 0x0288 (size: 0x8)
    FVector Attack Target Last Location;                                              // 0x0290 (size: 0x18)
    FVector Search Direction;                                                         // 0x02A8 (size: 0x18)
    bool Combat;                                                                      // 0x02C0 (size: 0x1)
    double Wait Time Between Searching;                                               // 0x02C8 (size: 0x8)
    double Search Area Size;                                                          // 0x02D0 (size: 0x8)
    class USceneComponent* RangeStartPoint;                                           // 0x02D8 (size: 0x8)
    int32 Current Behavior Index;                                                     // 0x02E0 (size: 0x4)
    class AActor* Move To Actor;                                                      // 0x02E8 (size: 0x8)
    bool Being Interacted With;                                                       // 0x02F0 (size: 0x1)
    bool Available To AI Interact;                                                    // 0x02F1 (size: 0x1)
    bool Can AI Interact;                                                             // 0x02F2 (size: 0x1)
    bool Only Interact With AI Teams;                                                 // 0x02F3 (size: 0x1)
    TArray<FName> AI Teams;                                                           // 0x02F8 (size: 0x10)
    double AI Interact Ranage;                                                        // 0x0308 (size: 0x8)
    double AI Interact Time Deviation;                                                // 0x0310 (size: 0x8)
    TArray<FS_AIInteractAnim> AI Interaction Animations;                              // 0x0318 (size: 0x10)
    TEnumAsByte<E_OrderOptions::Type> AI Interaction Animation Order;                 // 0x0328 (size: 0x1)
    double AI Interact Time;                                                          // 0x0330 (size: 0x8)
    TArray<class AActor*> Melee Hit Actors;                                           // 0x0338 (size: 0x10)
    bool Temporary Stop;                                                              // 0x0348 (size: 0x1)
    TEnumAsByte<E_AIBehaviour::Type> End Behaviour;                                   // 0x0349 (size: 0x1)
    TMap<FName, TEnumAsByte<E_StimuliReaction::Type>> Current Audio Response;         // 0x0350 (size: 0x50)
    FVector Search Location;                                                          // 0x03A0 (size: 0x18)
    TArray<class AActor*> Flee From Actors;                                           // 0x03B8 (size: 0x10)
    FTimerHandle Forget Attackers Timer Reference;                                    // 0x03C8 (size: 0x8)
    TEnumAsByte<E_AIBehaviour::Type> Temp Behaviour;                                  // 0x03D0 (size: 0x1)
    bool Use Trigger Cooldown;                                                        // 0x03D1 (size: 0x1)
    TArray<FS_TriggerResponse> Trigger Activated Response;                            // 0x03D8 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Deactivated Response;                          // 0x03E8 (size: 0x10)
    double Trigger Cooldown Time;                                                     // 0x03F8 (size: 0x8)
    bool Use Proximity Response;                                                      // 0x0400 (size: 0x1)
    TArray<FS_ReactSettings> Proximity Activated Response;                            // 0x0408 (size: 0x10)
    TArray<FS_ReactSettings> Proximity Deactivated Response;                          // 0x0418 (size: 0x10)
    bool Proximity Responce Requires Sight;                                           // 0x0428 (size: 0x1)
    bool Use Reaction Cooldown;                                                       // 0x0429 (size: 0x1)
    double Proximity Cool Down Time;                                                  // 0x0430 (size: 0x8)
    double Proximity Sphere Radius;                                                   // 0x0438 (size: 0x8)
    double Current Trigger Cooldown Time;                                             // 0x0440 (size: 0x8)
    class AActor* Focus Actor;                                                        // 0x0448 (size: 0x8)
    double Current Proximity Cool Down Time;                                          // 0x0450 (size: 0x8)
    TArray<FS_AttachedHoldables> Attached Holdables;                                  // 0x0458 (size: 0x10)
    class ABP_AIWeapon_Master_C* Holdable Actor;                                      // 0x0468 (size: 0x8)
    bool Block Attack;                                                                // 0x0470 (size: 0x1)
    int32 Current Emote Index;                                                        // 0x0474 (size: 0x4)
    FS_EmoteAnimations Current Emote;                                                 // 0x0478 (size: 0x10)
    class ABP_EmoteProp_C* Emote Actor;                                               // 0x0488 (size: 0x8)
    bool Reloading;                                                                   // 0x0490 (size: 0x1)
    int32 Current Ammo;                                                               // 0x0494 (size: 0x4)
    bool Searching For Target;                                                        // 0x0498 (size: 0x1)
    FVector Attack Target Velocity;                                                   // 0x04A0 (size: 0x18)
    FTimerHandle Emote Timer;                                                         // 0x04B8 (size: 0x8)
    TSubclassOf<class ABP_AIWeapon_Master_C> Temp Holdable Class;                     // 0x04C0 (size: 0x8)
    bool In Transition;                                                               // 0x04C8 (size: 0x1)
    double Current Anim Transition Cooldown;                                          // 0x04D0 (size: 0x8)
    double Current Audio Transition Cooldown;                                         // 0x04D8 (size: 0x8)
    class USoundCue* Second Audio Transition;                                         // 0x04E0 (size: 0x8)
    TEnumAsByte<E_AIBehaviour::Type> Last Non Combat Behaviour;                       // 0x04E8 (size: 0x1)
    bool In Override Behaviour;                                                       // 0x04E9 (size: 0x1)
    TEnumAsByte<E_AIBehaviour::Type> Base Behaviour;                                  // 0x04EA (size: 0x1)
    double Base Behaviour Timer;                                                      // 0x04F0 (size: 0x8)
    bool Use Base Behaviour;                                                          // 0x04F8 (size: 0x1)
    FTimerHandle Emote Done Timer;                                                    // 0x0500 (size: 0x8)
    class UAnimMontage* Second Anim Transition;                                       // 0x0508 (size: 0x8)
    bool Ragdoll On Death;                                                            // 0x0510 (size: 0x1)
    bool AI Interact Loop Order Anims;                                                // 0x0511 (size: 0x1)
    TEnumAsByte<E_StartingAIBehaviours::Type> AI Interact Between Behaviour;          // 0x0512 (size: 0x1)
    double Between AI Interact Time;                                                  // 0x0518 (size: 0x8)
    class UParticleSystemComponent* Muzzle Effect;                                    // 0x0520 (size: 0x8)
    double From Audio Length;                                                         // 0x0528 (size: 0x8)
    double LOS Break Search Time;                                                     // 0x0530 (size: 0x8)
    FBP_SmartAIComponent_CAI Killed AI Killed;                                        // 0x0538 (size: 0x10)
    void AI Killed(class ACharacter* AI Character);
    double Dead Despawned Time;                                                       // 0x0548 (size: 0x8)
    TArray<FS_ReactedActors> Reacted Actors;                                          // 0x0550 (size: 0x10)
    FTimerHandle Melee Trace Timer;                                                   // 0x0560 (size: 0x8)
    FS_AIOptimization AI Manager Settings;                                            // 0x0568 (size: 0xC)
    bool Allow Friendly Fire;                                                         // 0x0574 (size: 0x1)
    double Combat Search Time;                                                        // 0x0578 (size: 0x8)
    TArray<FName> Friendly Tags;                                                      // 0x0580 (size: 0x10)
    double Flee Time;                                                                 // 0x0590 (size: 0x8)
    class UAnimMontage* Still Override Animation;                                     // 0x0598 (size: 0x8)
    TEnumAsByte<E_OrderOptions::Type> Still Override Order;                           // 0x05A0 (size: 0x1)
    TArray<class UAnimMontage*> Still Override Anims;                                 // 0x05A8 (size: 0x10)
    bool Still Behaviour Override;                                                    // 0x05B8 (size: 0x1)
    FS_GeneralSettings Still Reaction Settings;                                       // 0x05C0 (size: 0xA8)
    TSet<TEnumAsByte<E_AIBehaviour::Type>> Override Behaviours;                       // 0x0668 (size: 0x50)
    TArray<class UAnimMontage*> Priority Montages;                                    // 0x06B8 (size: 0x10)
    double Audio Transition Cooldown;                                                 // 0x06C8 (size: 0x8)
    double Animation Transition Cooldown;                                             // 0x06D0 (size: 0x8)
    FS_AllAIAnims AI Animations;                                                      // 0x06D8 (size: 0x4C8)
    bool Use Sight Reaction Time;                                                     // 0x0BA0 (size: 0x1)
    double Reaction Time;                                                             // 0x0BA8 (size: 0x8)
    double Reacted Actor Time;                                                        // 0x0BB0 (size: 0x8)
    bool Can Crouch;                                                                  // 0x0BB8 (size: 0x1)
    TMap<TEnumAsByte<E_AIBehaviour::Type>, double> Movement Speeds;                   // 0x0BC0 (size: 0x50)
    FS_GeneralSettings Default Reaction Settings;                                     // 0x0C10 (size: 0xA8)
    double Flee Distance;                                                             // 0x0CB8 (size: 0x8)
    double Flee Distance Cut Off;                                                     // 0x0CC0 (size: 0x8)
    FS_GeneralSettings Flee Reaction Settings;                                        // 0x0CC8 (size: 0xA8)
    double Roam Wait Time;                                                            // 0x0D70 (size: 0x8)
    double Roam Wait Time Deviation;                                                  // 0x0D78 (size: 0x8)
    double Min Roam Distance;                                                         // 0x0D80 (size: 0x8)
    double Max Roam Distance;                                                         // 0x0D88 (size: 0x8)
    bool Use Prop Anims;                                                              // 0x0D90 (size: 0x1)
    double Prop Anim Freequency;                                                      // 0x0D98 (size: 0x8)
    double Prop Anim Frequency Deviation;                                             // 0x0DA0 (size: 0x8)
    TEnumAsByte<E_OrderOptions::Type> Prop Anim Order;                                // 0x0DA8 (size: 0x1)
    TArray<FS_EmoteAnimations> Prop Anims Animations;                                 // 0x0DB0 (size: 0x10)
    TEnumAsByte<E_OrderOptions::Type> Roam Wait Anim Order;                           // 0x0DC0 (size: 0x1)
    TArray<class UAnimMontage*> Roam Wait Anims;                                      // 0x0DC8 (size: 0x10)
    bool Loop Roam Wait Order Anims;                                                  // 0x0DD8 (size: 0x1)
    FS_GeneralSettings Roam Reaction Settings;                                        // 0x0DE0 (size: 0xA8)
    FS_GeneralSettings Search Reaction Settings;                                      // 0x0E88 (size: 0xA8)
    FS_GeneralSettings AI Interact Reaction Settings;                                 // 0x0F30 (size: 0xA8)
    TEnumAsByte<E_OrderOptions::Type> Way Point Options;                              // 0x0FD8 (size: 0x1)
    bool Loop Way Points;                                                             // 0x0FD9 (size: 0x1)
    bool Reverse Loop Way Point;                                                      // 0x0FDA (size: 0x1)
    TArray<class ABP_MasterWayPoint_C*> Way Points;                                   // 0x0FE0 (size: 0x10)
    FS_GeneralSettings Way Point Reaction Settings;                                   // 0x0FF0 (size: 0xA8)
    FS_GeneralSettings Face Direction Reaction Settings;                              // 0x1098 (size: 0xA8)
    FS_GeneralSettings Melee Reaction Settings;                                       // 0x1140 (size: 0xA8)
    double Melee Trace Lenth;                                                         // 0x11E8 (size: 0x8)
    int32 Max Magazine Ammo;                                                          // 0x11F0 (size: 0x4)
    int32 Per Shot Ammo Reduction;                                                    // 0x11F4 (size: 0x4)
    class UAnimMontage* Reload Montage;                                               // 0x11F8 (size: 0x8)
    int32 Max Total Ammo;                                                             // 0x1200 (size: 0x4)
    bool Use Ammo;                                                                    // 0x1204 (size: 0x1)
    bool Can Reload;                                                                  // 0x1205 (size: 0x1)
    bool Start With Full Magazine;                                                    // 0x1206 (size: 0x1)
    double Max Hit Deviation (Projectile);                                            // 0x1208 (size: 0x8)
    double Accuracy (Projectile);                                                     // 0x1210 (size: 0x8)
    double Max Miss Deviation (Projectile);                                           // 0x1218 (size: 0x8)
    TSubclassOf<class AActor> Range Projectile;                                       // 0x1220 (size: 0x8)
    double Trace Range;                                                               // 0x1228 (size: 0x8)
    double Trace Radius;                                                              // 0x1230 (size: 0x8)
    double Spread (Trace);                                                            // 0x1238 (size: 0x8)
    double Decal LifeSpan (Trace);                                                    // 0x1240 (size: 0x8)
    TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> Surface Decal (Trace);   // 0x1248 (size: 0x50)
    double Decal Fade Distance (Trace);                                               // 0x1298 (size: 0x8)
    TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> Surface Emitters (Trace);   // 0x12A0 (size: 0x50)
    FVector Surface Decal Size (Trace);                                               // 0x12F0 (size: 0x18)
    class USoundCue* RangeShootSound;                                                 // 0x1308 (size: 0x8)
    bool Can Range Attack While Moving;                                               // 0x1310 (size: 0x1)
    bool Use Range Anim Time Frequency;                                               // 0x1311 (size: 0x1)
    double Range Attack Frequency;                                                    // 0x1318 (size: 0x8)
    bool Can Range Attack;                                                            // 0x1320 (size: 0x1)
    TEnumAsByte<E_OrderOptions::Type> Range Attack Anim Order;                        // 0x1321 (size: 0x1)
    TArray<class UAnimMontage*> Range Attack Montages;                                // 0x1328 (size: 0x10)
    TMap<TEnumAsByte<E_HitboxTypeAI::Type>, double> Range Damage;                     // 0x1338 (size: 0x50)
    double Range Attack Distance;                                                     // 0x1388 (size: 0x8)
    double Projectile Homing Acceleration Magnitude;                                  // 0x1390 (size: 0x8)
    bool Homing Projectile;                                                           // 0x1398 (size: 0x1)
    bool Use Projectile Actor Settings;                                               // 0x1399 (size: 0x1)
    FS_GeneralSettings Range Reaction Settings;                                       // 0x13A0 (size: 0xA8)
    class UParticleSystem* Range Attack Particle Effect;                              // 0x1448 (size: 0x8)
    FName Range Attack Particle Socket;                                               // 0x1450 (size: 0x8)
    bool Use Cover;                                                                   // 0x1458 (size: 0x1)
    FS_GeneralSettings Follow Reaction Settings;                                      // 0x1460 (size: 0xA8)
    class AActor* Follow Actor;                                                       // 0x1508 (size: 0x8)
    double Max Follow Distance;                                                       // 0x1510 (size: 0x8)
    bool Copy Follow Actor Direction;                                                 // 0x1518 (size: 0x1)
    bool Attack Follow Attack Target;                                                 // 0x1519 (size: 0x1)
    bool Attack Follow Attackers;                                                     // 0x151A (size: 0x1)
    double Follow Attack Distance;                                                    // 0x1520 (size: 0x8)
    TEnumAsByte<E_StartingAIBehaviours::Type> Follow End Behaviour;                   // 0x1528 (size: 0x1)
    FS_GeneralSettings Interaction Point Reaction Settings;                           // 0x1530 (size: 0xA8)
    double Flee Base Distance;                                                        // 0x15D8 (size: 0x8)
    bool Require Sight Of Attacker;                                                   // 0x15E0 (size: 0x1)
    double Alert Defenders Distances;                                                 // 0x15E8 (size: 0x8)
    TArray<FName> Defend Tags;                                                        // 0x15F0 (size: 0x10)
    TArray<class AActor*> Defend Actors;                                              // 0x1600 (size: 0x10)
    double Defend Cooldown Time;                                                      // 0x1610 (size: 0x8)
    double Standing Half Hight;                                                       // 0x1618 (size: 0x8)
    class ABP_MasterWayPoint_C* Current Way Point;                                    // 0x1620 (size: 0x8)
    TEnumAsByte<E_AIBehaviour::Type> Last Behaviour;                                  // 0x1628 (size: 0x1)
    FVector Climb End Location;                                                       // 0x1630 (size: 0x18)
    double Time;                                                                      // 0x1648 (size: 0x8)
    FRotator Jump Look At Rotation;                                                   // 0x1650 (size: 0x18)
    FVector Jump End Location;                                                        // 0x1668 (size: 0x18)
    FVector End Climb Location;                                                       // 0x1680 (size: 0x18)
    bool Climbing;                                                                    // 0x1698 (size: 0x1)
    bool Forward Blocked;                                                             // 0x1699 (size: 0x1)
    double Current Climb Hight;                                                       // 0x16A0 (size: 0x8)
    bool Valt;                                                                        // 0x16A8 (size: 0x1)
    FVector Valt End Location;                                                        // 0x16B0 (size: 0x18)
    FVector Object Normal;                                                            // 0x16C8 (size: 0x18)
    class UAnimMontage* Climb Anim;                                                   // 0x16E0 (size: 0x8)
    double Current Alert Cooldown Time;                                               // 0x16E8 (size: 0x8)
    TArray<FS_ClimbAnims> Climb Animations;                                           // 0x16F0 (size: 0x10)
    TArray<FS_ClimbAnims> Valt Animations;                                            // 0x1700 (size: 0x10)
    double Max Drop Down Height;                                                      // 0x1710 (size: 0x8)
    TArray<class AActor*> Attack Targets;                                             // 0x1718 (size: 0x10)
    bool In Cover;                                                                    // 0x1728 (size: 0x1)
    bool Can Climb;                                                                   // 0x1729 (size: 0x1)
    bool Use AI Routine;                                                              // 0x172A (size: 0x1)
    TArray<FS_RoutineSettings> AI Routines;                                           // 0x1730 (size: 0x10)
    class AActor* Time of Day;                                                        // 0x1740 (size: 0x8)
    FTimerHandle Routine Timer;                                                       // 0x1748 (size: 0x8)
    TEnumAsByte<E_OrderOptions::Type> Face Direction Anim Order;                      // 0x1750 (size: 0x1)
    bool Loop Face Direction Anims;                                                   // 0x1751 (size: 0x1)
    TArray<class UAnimMontage*> Face Direction Anims;                                 // 0x1758 (size: 0x10)
    bool Delay Range Attack;                                                          // 0x1768 (size: 0x1)
    double Range Delay Time;                                                          // 0x1770 (size: 0x8)
    bool Delay Done;                                                                  // 0x1778 (size: 0x1)
    bool Hit React Timer;                                                             // 0x1779 (size: 0x1)
    bool Use Combat Stance;                                                           // 0x177A (size: 0x1)
    double Hit Reaction Cooldown;                                                     // 0x1780 (size: 0x8)
    double Combat Stance Time;                                                        // 0x1788 (size: 0x8)
    bool Use Dead Body Reaction;                                                      // 0x1790 (size: 0x1)
    TEnumAsByte<E_DeadBodyReaction::Type> Dead Body Responce;                         // 0x1791 (size: 0x1)
    bool Dead Body Reaction Temporary;                                                // 0x1792 (size: 0x1)
    double Dead Body Temporary Time;                                                  // 0x1798 (size: 0x8)
    TArray<FName> Dead Body Alert Tags;                                               // 0x17A0 (size: 0x10)
    double Dead Body Alert Range;                                                     // 0x17B0 (size: 0x8)
    TArray<FName> Dead Body React Tags;                                               // 0x17B8 (size: 0x10)
    bool Dead Body Alert;                                                             // 0x17C8 (size: 0x1)
    TArray<class AActor*> Dead Body Alert Actors;                                     // 0x17D0 (size: 0x10)
    TArray<class AActor*> Seen Dead Bodys;                                            // 0x17E0 (size: 0x10)
    bool Temperary Response;                                                          // 0x17F0 (size: 0x1)
    double Temparary Time;                                                            // 0x17F8 (size: 0x8)
    TEnumAsByte<E_DeadBodyReaction::Type> Alerted Response;                           // 0x1800 (size: 0x1)
    bool Can Be Alerted AI;                                                           // 0x1801 (size: 0x1)
    double Yaw Aim Offset;                                                            // 0x1808 (size: 0x8)
    bool Aim Offset Enabled;                                                          // 0x1810 (size: 0x1)
    FS_MeleeAnimations Current Melee Anim;                                            // 0x1818 (size: 0x58)
    double Alert Cooldown Time;                                                       // 0x1870 (size: 0x8)
    bool Use Strafe;                                                                  // 0x1878 (size: 0x1)
    double Min Strafe Distance;                                                       // 0x1880 (size: 0x8)
    double Max Strafe Distance;                                                       // 0x1888 (size: 0x8)
    bool Allow Strafe;                                                                // 0x1890 (size: 0x1)
    TArray<class AActor*> Last Proximity Check;                                       // 0x1898 (size: 0x10)
    TEnumAsByte<E_StartingAIBehaviours::Type> Start Behaviour;                        // 0x18A8 (size: 0x1)
    TSubclassOf<class ABP_AIWeapon_Master_C> Start Holdable;                          // 0x18B0 (size: 0x8)
    TSubclassOf<class ABP_AIWeapon_Master_C> Start Range Weapon;                      // 0x18B8 (size: 0x8)
    TSubclassOf<class ABP_AIWeapon_Master_C> Start Melee Weapon;                      // 0x18C0 (size: 0x8)
    bool Activated;                                                                   // 0x18C8 (size: 0x1)
    FTimerHandle Combat Stance Timer;                                                 // 0x18D0 (size: 0x8)
    bool Combat Stance;                                                               // 0x18D8 (size: 0x1)
    bool Call Defenders;                                                              // 0x18D9 (size: 0x1)
    bool AI Ready;                                                                    // 0x18DA (size: 0x1)
    double Climb Speed;                                                               // 0x18E0 (size: 0x8)
    bool Can Block;                                                                   // 0x18E8 (size: 0x1)
    TMap<TEnumAsByte<E_BlockType::Type>, int32> Block Chances;                        // 0x18F0 (size: 0x50)
    TArray<class UAnimMontage*> Melee Block Anims;                                    // 0x1940 (size: 0x10)
    FTimerHandle Block Timer;                                                         // 0x1950 (size: 0x8)
    double Block Cooldown;                                                            // 0x1958 (size: 0x8)
    TArray<class UAnimMontage*> Blocked Attack Montages;                              // 0x1960 (size: 0x10)
    bool Flee Alert;                                                                  // 0x1970 (size: 0x1)
    bool Can Be Flee Alerted;                                                         // 0x1971 (size: 0x1)
    TArray<class AActor*> Flee Alert Actors;                                          // 0x1978 (size: 0x10)
    TArray<FName> Flee Alert Tags;                                                    // 0x1988 (size: 0x10)
    double Flee Alert Distance;                                                       // 0x1998 (size: 0x8)
    bool Aimed Still Override;                                                        // 0x19A0 (size: 0x1)
    FVector Flee Start Location;                                                      // 0x19A8 (size: 0x18)
    bool Use LOS Search Time;                                                         // 0x19C0 (size: 0x1)
    double Projectile Speed;                                                          // 0x19C8 (size: 0x8)
    bool Melee Allow Rotation;                                                        // 0x19D0 (size: 0x1)
    TEnumAsByte<ETraceTypeQuery> CoverTraceChannel;                                   // 0x19D1 (size: 0x1)
    TEnumAsByte<ETraceTypeQuery> AttackTraceChannel;                                  // 0x19D2 (size: 0x1)
    FTimerHandle Despawn Timer;                                                       // 0x19D8 (size: 0x8)
    double DamageMulti;                                                               // 0x19E0 (size: 0x8)
    bool InAttack;                                                                    // 0x19E8 (size: 0x1)
    bool Despawn AI;                                                                  // 0x19E9 (size: 0x1)
    TMap<TEnumAsByte<E_Radius::Type>, int32> Acceptable Radius;                       // 0x19F0 (size: 0x50)
    FName DeadAICapsuleCollision;                                                     // 0x1A40 (size: 0x8)
    FName DeadAIMeshCollision;                                                        // 0x1A48 (size: 0x8)
    TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> Surface Sound (Trace);           // 0x1A50 (size: 0x50)
    class USoundCue* DeathSound;                                                      // 0x1AA0 (size: 0x8)
    TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> Melee Surface Decal;     // 0x1AA8 (size: 0x50)
    TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> Melee Surface Sounds;            // 0x1AF8 (size: 0x50)
    TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> Melee Emitters;             // 0x1B48 (size: 0x50)
    double MeleeDetectHighOffset;                                                     // 0x1B98 (size: 0x8)
    FVector MeleeDetectSize;                                                          // 0x1BA0 (size: 0x18)
    bool Respawn;                                                                     // 0x1BB8 (size: 0x1)
    TArray<class UAudioComponent*> Sounds;                                            // 0x1BC0 (size: 0x10)
    bool AllowDynamicRespawnCheck;                                                    // 0x1BD0 (size: 0x1)
    FName StreamLevelPackageName;                                                     // 0x1BD4 (size: 0x8)
    class AActor* SpawnedBy;                                                          // 0x1BE0 (size: 0x8)
    class UBTS_FaceDirectionAnimations_C* FaceDirectionTask;                          // 0x1BE8 (size: 0x8)
    int32 MaxHealth;                                                                  // 0x1BF0 (size: 0x4)
    class AActor* Sight Actor;                                                        // 0x1BF8 (size: 0x8)

    void Actor Attack Target(class AActor*& Attack Target);
    void AI Can Interact?(bool& Can Interact);
    void AI Is Dead?(bool& Dead);
    void DespawnCheck();
    void CreateNewController();
    void StopCurrentAudio();
    void UpdateCapsuleCollision(FName Name);
    void ChangeDamageMulti(double ChangeAmount);
    void Set BB AllowRotate(bool AllowRotate);
    void Set BB BlockMove(bool StopNode);
    void Check Flee Alert(class AActor* Actor);
    void Check Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void Check Attack Targets(bool& Target Found, class AActor*& Actor);
    void Check Start Deactivated();
    void Proximity Updates();
    void Update Aim Offset Yaw(const class AActor*& Look At Actor);
    void Tag Check(TArray<FName>& Tags 1, TArray<FName>& Tags 2, bool& Return value, FName& Found Tag);
    void Dead Body Reaction(class AActor* Reaction Actor);
    void Return Hit Reaction Anims(TArray<class UAnimMontage*>& Hit Reactions);
    void Spawn Hit Effects(FHitResult Hit Result);
    void Update Routine(int32 Hour);
    void Attack Distance Check();
    void Up Climb Check();
    void Climbing Checks();
    void Forward Check(double Forward Distance, double Down Distance, FVector Trace Towards, FHitResult& Hit Result, bool& Return value);
    void Drop Down Check();
    void Crouch Check();
    void Defend Check(class AActor* Attacked, class AActor* Attacker);
    void Alert Defenders(class AActor* Attacker);
    void Take Damage(double Damage);
    void Damage Reaction(double Damage, class AActor* Actor);
    void Convert To Behaviour(TEnumAsByte<E_StartingAIBehaviours::Type> Starting Behaviour, TEnumAsByte<E_AIBehaviour::Type>& Behaviour);
    void Current Reaction Settings(FS_GeneralSettings& Reaction Settings);
    void Aimed Focus(class AActor* Aimed Instigator);
    void Basic Melee Attack Collision();
    void Reacted Actor Check(class AActor* Actor, bool& Actor Found);
    void Set BB Follow Actor(class AActor* Follow Actor);
    bool Is In Combat?();
    void Transitions(FS_GeneralSettings Transition From, FS_GeneralSettings Transition To);
    void Transition Audio(bool Start Transition, FS_GeneralSettings AI Settings, bool& Use Transition, class USoundCue*& Audio);
    void Transition Anim(bool Start Transition, FS_GeneralSettings AI Settings, bool& Use Transition, class UAnimMontage*& Anim);
    void Remove Total Amount(int32 Remove Amount, bool& Removed All, int32& Remaining Ammo);
    void Add Current Ammo(int32 Add Amount);
    void Current Ammo Check(bool& Has Ammo, int32& Ammo);
    void Total Ammo Check(bool& Has Ammo, int32& Ammo);
    void Remove Current Ammo(int32 Remove Amount);
    void Current Range Attack Distance(double& Attack Range);
    void Get Attached Weapon Ammo(bool& Ammo Found, class ABP_AIWeapon_Master_C*& Holdable With Ammo);
    void Destroy Emote Actor();
    void Return Emote Montages(TArray<class UAnimMontage*>& Emote Montages);
    void Stop Current Montage(bool Keep Emote Montages);
    void Is Holding Weapon(bool& Is Weapon, class ABP_AIWeapon_Master_C*& Holdable Actor);
    void Spawn Attached Holdables();
    void Check Attached Holdables(TSubclassOf<class ABP_AIWeapon_Master_C> Holdable Class, bool& Return value, class ABP_AIWeapon_Master_C*& Holdable);
    void Holdable Unequipped();
    void Unequip Holdable();
    void Holdable Equip Socket(TSubclassOf<class ABP_AIWeapon_Master_C> Holdable Class, FName& Equip Socket, FName& Unequip Socket, bool& Equip Montage);
    void Equip Holdable(TSubclassOf<class ABP_AIWeapon_Master_C> Holdable Class);
    void Spawn Holdable(TSubclassOf<class ABP_AIWeapon_Master_C> AI Weapon, FName Attach Socket, bool Is Visible, class ABP_AIWeapon_Master_C*& Holdable Actor);
    void Set BB Focus Actor(class AActor* Focus Actor);
    void Proximity Deactivated(class AActor* Proximity Actor, bool& Proximity Deactivated);
    void Reset Trigger Cooldown Timer();
    void Reset Proximity Cooldown Timer();
    void Cooldown Timer();
    void Proximity Activated(class AActor* Proximity Actor, bool& Proximity Triggered);
    void Trigger Deactivated(class AActor* Trigger Actor, class ABP_AITrigger_C* AI Trigger);
    void Trigger Activated(class AActor* Trigger Actor, class ABP_AITrigger_C* AI Trigger);
    void Set BB Dead(bool Dead);
    void Closest Flee From Actor(class AActor*& Actor, double& Distance);
    void Sight Reaction(class AActor* Sight Actor, FAIStimulus AI Stimulus);
    void Stop Fleeing();
    void Set Flee Actor(class AActor* Flee From Actor);
    void Audio Reaction(class AActor* Actor, FAIStimulus AI Stimulus);
    void Set BB Temporary Stop(bool Temp Stop);
    void Cancel AI Interact();
    void Set BB Being Interacted With(bool Interacted With);
    void AI Interact Time Total(double& Time);
    void Set BB Move To Actor(class AActor* Actor);
    void Determine Attack Type();
    void Reset Attack Target(bool& New Attack Target, class AActor*& Attack Target);
    void Range Attack();
    void Is Low Health(bool& Low Health, int32& Health);
    void Melee Collision Check(class AActor* Hit Actor, class UPrimitiveComponent* Hit Component, FName Bone Name, FHitResult Hit);
    void Deactive Melee Detect();
    void Active Melee Detect();
    void Remove Attacker(class AActor* Attacker);
    void Set BB Attack Target(class AActor* Attacker);
    void Set BB Wait Time(double FloatValue);
    void Set BB Location(FVector VectorValue);
    void Set BB Facing Location(FVector VectorValue);
    void Forget Attackers Timer();
    void Set BB Current Behaviour(TEnumAsByte<E_AIBehaviour::Type> AI New Behaviour, bool Is Base Behaviour);
    void End Flee();
    void Begin Flee();
    void Last Attacker(class AActor*& Attacker);
    void Random IP(class ABP_MasterInteractionPoint_C* Last Interaction Point, bool& Return value, class ABP_MasterInteractionPoint_C*& Interaction Point);
    void Random Specified IP(class ABP_MasterInteractionPoint_C* Last Interaction Point, bool& Return value, class ABP_MasterInteractionPoint_C*& Interaction Point);
    void Set AI Dead();
    void Generic Damage Reaction(class AActor* Damage Cause);
    void Debug Text(FString InString, FLinearColor TextColor);
    void OnFail_EC17E16E4EA49390E6C1F7A05964C877(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void OnSuccess_EC17E16E4EA49390E6C1F7A05964C877(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void OnNotifyEnd_2049CB47425917B35D40CBAEB03142CE(FName NotifyName);
    void OnNotifyBegin_2049CB47425917B35D40CBAEB03142CE(FName NotifyName);
    void OnInterrupted_2049CB47425917B35D40CBAEB03142CE(FName NotifyName);
    void OnBlendOut_2049CB47425917B35D40CBAEB03142CE(FName NotifyName);
    void OnCompleted_2049CB47425917B35D40CBAEB03142CE(FName NotifyName);
    void Set Movement Speed(double Movement Speed);
    void Multicast Sound(class USoundCue* Sound, double Volume);
    void Server Holdable Visibility(bool Visible);
    void Server Starting Weapon();
    void Server Unequip Holdable();
    void Server Holdable Unequipped();
    void Server Block Attack();
    void Server Unblock Attack();
    void Roam Emotes();
    void Emote();
    void Emote Done();
    void Server Reload();
    void Transition Anim Timer(FS_GeneralSettings Transition From, FS_GeneralSettings Transition To);
    void End Anim Transition();
    void Clear Emote();
    void Multicast Range Attack Partical(FHitResult Hit Result);
    void Multicast Play Transition to Audio(double Play In, class USoundCue* Audio);
    void Transition To Audio Timer();
    void Multicast Despawn Body();
    void Server Despawn Body();
    void Server Respawn(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void Multicast Respawn();
    void Deactivate AI();
    void Activate AI();
    void Start Melee Trace();
    void Melee Trace Timer Events();
    void Actor Aim Focus(class AActor* Instigator);
    void Possessed();
    void Server AI Setup();
    void AI Take Damage(class AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void Alert Actor Defenders(class AActor* Attacked, class AActor* Attacker);
    void Jump Down(FVector End Location);
    void ReceiveTick(float DeltaSeconds);
    void Jump Down Close();
    void Jump Down Enter();
    void Climb Up();
    void Climb Up Close();
    void Move Across();
    void End Climb();
    void Routine(int32 Hour);
    void Delayed Range Attack();
    void Start Hit React Timer();
    void Hit React Timer Done();
    void AI Alert(class AActor* Alert Actor);
    void Multicast Start Aim Offset(class AActor* Target);
    void Multicast Stop Aim Offset();
    void End Combat();
    void Entered Combat();
    void Multicast Stop Current Montage(bool Keep EmoteMontages);
    void Multicast Homing Projectile Spawn(FVector Spawn Transform Location, class AActor* Attack Target, TSubclassOf<class AActor> Class);
    void Multicast Projectile Spawn(FVector Spawn Transform Location, FVector Velocity, TSubclassOf<class AActor> Class, double Speed);
    void Cancel Temp Behaviour();
    void Set Behavior();
    void Start Temp Behaviour(TEnumAsByte<E_AIBehaviour::Type> Temp Behavior, double Length, TEnumAsByte<E_AIBehaviour::Type> End Behavior);
    void Multicast Stop Montage(class UAnimMontage* Montage);
    void Multicast AI Dead();
    void Multicast Play Montage(class UAnimMontage* Montage, double Play Rate, FName Start Name);
    void AI Camera Activate(bool Activate);
    void AI Animation Switch(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void AI End Alert();
    void Turret End Reload();
    void Turret Start Reload();
    void Turret Idle Stop();
    void Turret Idle Start();
    void Turret Destroyed Effect();
    void AI Dead();
    void Multicast Set Collision Profile(FName Capsule Profile, FName Mesh Profile);
    void Server Start Combat Stance();
    void End Combat Stance();
    void Multicast Combat Stance(bool Combat Stance);
    void Cancel Melee Timer();
    void AI Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void Start Block Timer(double Time);
    void End Block Timer();
    void Start Flee Timer();
    void Flee Timer End();
    void Stop Despawn Timer();
    void ReceiveBeginPlay();
    void FrequencyTimer(double Time);
    void StartTimer();
    void Multicast Melee Emitter(FHitResult Hit);
    void StreamLevelUnLoaded();
    void SetupLevelStreamUnload();
    void AI Trigger Deactivated Bind(class AActor* End Overlap Actor, class ABP_AITrigger_C* AI Trigger);
    void AI Trigger Activated Bind(class AActor* Overlap Actor, class ABP_AITrigger_C* AI Trigger);
    void ExecuteUbergraph_BP_SmartAIComponent(int32 EntryPoint);
    void AI Killed__DelegateSignature(class ACharacter* AI Character);
}; // Size: 0x1C00

#endif

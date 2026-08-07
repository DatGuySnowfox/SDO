#ifndef UE4SS_SDK_DragonIKPlugin_HPP
#define UE4SS_SDK_DragonIKPlugin_HPP

#include "DragonIKPlugin_enums.hpp"

struct FAnimNode_ChineseDragonSolver : public FAnimNode_DragonControlBase
{
    FDragonData_ChineseDragonBoneInput Custom_Bone_Input;                             // 0x0110 (size: 0x10)
    int32 Stability_Reference_Bone_Index;                                             // 0x0120 (size: 0x4)
    FTransform EffectorTransform;                                                     // 0x0130 (size: 0x60)
    FDragonData_ChineseDragonBoneRotationOffset Extra_Rotation_Offset_Per_Bone;       // 0x0190 (size: 0x10)
    FRotator Extra_Rotation_Offset_Overall;                                           // 0x01A0 (size: 0x18)
    FRotator Extra_Rotation_Offset;                                                   // 0x01B8 (size: 0x18)
    bool Use_Advanced_Strict_Chain_Logic;                                             // 0x0290 (size: 0x1)
    bool Limit_Z_Solving;                                                             // 0x0298 (size: 0x1)
    bool Fix_Root_in_Place;                                                           // 0x0299 (size: 0x1)
    bool Continous_Normalization;                                                     // 0x029A (size: 0x1)
    float Normalization_Speed;                                                        // 0x029C (size: 0x4)
    FRuntimeFloatCurve Normalization_Multiplier_Rel_Velocity;                         // 0x02A0 (size: 0x88)
    bool Preserve_Original_Pose;                                                      // 0x0328 (size: 0x1)
    FVector Test_Ref_Forward_Axis;                                                    // 0x0330 (size: 0x18)
    float Pelvis_Positioning_Calibration_Value;                                       // 0x0348 (size: 0x4)
    bool Enable_AnimBP_Viewport_Output;                                               // 0x0370 (size: 0x1)
    bool Use_Scale_Add_Mode;                                                          // 0x0371 (size: 0x1)
    bool Use_Rot_Add_Mode;                                                            // 0x0372 (size: 0x1)
    bool rotation_relative_to_mesh;                                                   // 0x0373 (size: 0x1)
    bool Use_Warmup_Logic;                                                            // 0x0374 (size: 0x1)
    float Initial_Warmup_Distance;                                                    // 0x0378 (size: 0x4)
    bool Reset_To_Normal;                                                             // 0x037C (size: 0x1)
    bool Use_Interpolation;                                                           // 0x038C (size: 0x1)
    float Interpolation_Speed;                                                        // 0x0390 (size: 0x4)
    float Reset_ON_Interpolation_Speed;                                               // 0x0394 (size: 0x4)
    float Reset_OFF_Interpolation_Speed;                                              // 0x0398 (size: 0x4)
    float Precision;                                                                  // 0x039C (size: 0x4)
    FTransform CachedEffectorCSTransform;                                             // 0x03B0 (size: 0x60)
    class USkeletalMeshComponent* owning_skel;                                        // 0x0428 (size: 0x8)

}; // Size: 0x4C0

struct FAnimNode_DragonAimSolver : public FAnimNode_DragonControlBase
{
    bool Use_FName_Bone_Input;                                                        // 0x00D0 (size: 0x1)
    FBoneReference EndSplineBone;                                                     // 0x00D4 (size: 0x10)
    FBoneReference StartSplineBone;                                                   // 0x00E4 (size: 0x10)
    FName Name_EndSpline_Bone;                                                        // 0x00F4 (size: 0x8)
    FName Name_StartSpline_Bone;                                                      // 0x00FC (size: 0x8)
    FTransform LookAtLocation;                                                        // 0x0110 (size: 0x60)
    bool Use_Advanced_Input_Mode;                                                     // 0x0170 (size: 0x1)
    FDragonData_AimSolver_SkeletonInput Bone_Structure_Precise;                       // 0x0178 (size: 0x10)
    FDragonData_MultiInput dragon_input_data;                                         // 0x0198 (size: 0x20)
    TArray<FDragonData_ArmsData> Aiming_Hand_Limbs;                                   // 0x0260 (size: 0x10)
    FDragonData_Overrided_Location_Data Arm_TargetLocation_Overrides;                 // 0x0280 (size: 0x10)
    bool Use_Separate_Targets;                                                        // 0x0290 (size: 0x1)
    bool Override_Hand_Rotation;                                                      // 0x0291 (size: 0x1)
    bool enable_head_aim;                                                             // 0x0292 (size: 0x1)
    bool bAllowHandStretching;                                                        // 0x0293 (size: 0x1)
    FDragonData_ElbowOffset_Struct Extra_Struct_Elbow_Pole_Array;                     // 0x0298 (size: 0x10)
    bool reach_instead;                                                               // 0x02A8 (size: 0x1)
    bool Aggregate_Hand_Body;                                                         // 0x02A9 (size: 0x1)
    bool Let_Arm_Twist_With_Hand;                                                     // 0x02AA (size: 0x1)
    EPole_System_DragonIK pole_system_input;                                          // 0x02AB (size: 0x1)
    ETwist_Type_DragonIK arm_twist_axis;                                              // 0x02AC (size: 0x1)
    ERotation_Type_DragonIK hand_rotation_method;                                     // 0x02AD (size: 0x1)
    bool Override_Head_Rotation;                                                      // 0x02AE (size: 0x1)
    bool Enable_Hand_Interpolation;                                                   // 0x02AF (size: 0x1)
    float Hand_Interpolation_Speed;                                                   // 0x02B0 (size: 0x4)
    FDragonData_CustomArmLengths custom_arm_lengths;                                  // 0x02B8 (size: 0x10)
    EInputTransformSpace_DragonIK arm_transform_space;                                // 0x02C8 (size: 0x1)
    int32 Main_Arm_Index;                                                             // 0x02CC (size: 0x4)
    float Lookat_Radius;                                                              // 0x03F0 (size: 0x4)
    FRotator Inner_Body_Clamp;                                                        // 0x03F8 (size: 0x18)
    float Lookat_Clamp;                                                               // 0x0410 (size: 0x4)
    float Limbs_Clamp;                                                                // 0x0414 (size: 0x4)
    FDragonData_CustomLimbClamp Per_Limbs_Clamp_Array;                                // 0x0418 (size: 0x10)
    FRotator Extra_Aiming_Rotation_Offset;                                            // 0x0428 (size: 0x18)
    float Downward_Dip_Multiplier;                                                    // 0x0440 (size: 0x4)
    float Inverted_Dip_Multiplier;                                                    // 0x0444 (size: 0x4)
    float Vertical_Dip_Treshold;                                                      // 0x0448 (size: 0x4)
    float Side_Move_Multiplier;                                                       // 0x044C (size: 0x4)
    float Side_Down_Multiplier;                                                       // 0x0450 (size: 0x4)
    float Up_Rot_Clamp;                                                               // 0x0454 (size: 0x4)
    FVector2D Verticle_Range_Angles;                                                  // 0x0458 (size: 0x10)
    FVector2D Horizontal_Range_Angles;                                                // 0x0468 (size: 0x10)
    float Aim_Frontal_Offset;                                                         // 0x0478 (size: 0x4)
    FRuntimeFloatCurve Look_Bending_Curve;                                            // 0x0480 (size: 0x88)
    FRuntimeFloatCurve Look_Multiplier_Curve;                                         // 0x0508 (size: 0x88)
    EInputTransformSpace_DragonIK look_transform_space;                               // 0x0590 (size: 0x1)
    bool Lock_Legs;                                                                   // 0x0591 (size: 0x1)
    bool ignore_elbow_modification;                                                   // 0x0592 (size: 0x1)
    bool ignore_separate_hand_solving;                                                // 0x0593 (size: 0x1)
    bool Use_Natural_Method;                                                          // 0x0594 (size: 0x1)
    bool Head_Use_Separate_Clamp;                                                     // 0x0595 (size: 0x1)
    bool Is_Head_Accurate;                                                            // 0x0596 (size: 0x1)
    bool automatic_leg_make;                                                          // 0x0597 (size: 0x1)
    bool enable_solver;                                                               // 0x0599 (size: 0x1)
    bool Work_Outside_PIE;                                                            // 0x059A (size: 0x1)
    bool Use_Physics_Adaptation;                                                      // 0x059B (size: 0x1)
    FName physics_root_reference;                                                     // 0x059C (size: 0x8)
    bool Adaptive_Terrain_Tail;                                                       // 0x05A4 (size: 0x1)
    TEnumAsByte<ETraceTypeQuery> Trace_Channel;                                       // 0x05A5 (size: 0x1)
    float Trace_Up_Height;                                                            // 0x05A8 (size: 0x4)
    float Trace_Down_Height;                                                          // 0x05AC (size: 0x4)
    EInterpoLocation_Type_Plugin loc_interp_type;                                     // 0x069C (size: 0x1)
    bool Enable_Interpolation;                                                        // 0x06A5 (size: 0x1)
    float Interpolation_Speed;                                                        // 0x06A8 (size: 0x4)
    float Toggle_Interpolation_Speed;                                                 // 0x06AC (size: 0x4)
    FVector LookAt_Axis;                                                              // 0x0710 (size: 0x18)
    FVector Upward_Axis;                                                              // 0x0728 (size: 0x18)
    FVector TargetOffset;                                                             // 0x0740 (size: 0x18)
    bool Use_Reference_Forward_Axis;                                                  // 0x0758 (size: 0x1)
    FVector Reference_Constant_Forward_Axis;                                          // 0x0760 (size: 0x18)
    bool DisplayLineTrace;                                                            // 0x0895 (size: 0x1)
    FTransform Debug_LookAtLocation;                                                  // 0x08A0 (size: 0x60)
    TArray<FTransform> Debug_Hand_Locations;                                          // 0x0900 (size: 0x10)
    class USkeletalMeshComponent* owning_skel;                                        // 0x0CC0 (size: 0x8)

}; // Size: 0xD80

struct FAnimNode_DragonControlBase : public FAnimNode_Base
{
    FComponentSpacePoseLink ComponentPose;                                            // 0x0010 (size: 0x10)
    int32 LODThreshold;                                                               // 0x0020 (size: 0x4)
    float ActualAlpha;                                                                // 0x0024 (size: 0x4)
    EAnimAlphaInputType AlphaInputType;                                               // 0x002C (size: 0x1)
    bool bAlphaBoolEnabled;                                                           // 0x002D (size: 0x1)
    float Alpha;                                                                      // 0x0030 (size: 0x4)
    FInputScaleBias AlphaScaleBias;                                                   // 0x0034 (size: 0x8)
    FInputAlphaBoolBlend AlphaBoolBlend;                                              // 0x0040 (size: 0x48)
    FName AlphaCurveName;                                                             // 0x0088 (size: 0x8)
    FInputScaleBiasClamp AlphaScaleBiasClamp;                                         // 0x0090 (size: 0x30)

}; // Size: 0xD0

struct FAnimNode_DragonFabrikSolver : public FAnimNode_DragonControlBase
{
    FBoneReference TipBone_Input;                                                     // 0x00D0 (size: 0x10)
    FBoneReference RootBone_Input;                                                    // 0x00E0 (size: 0x10)
    float Precision;                                                                  // 0x00F0 (size: 0x4)
    float MaxIterations;                                                              // 0x00F4 (size: 0x4)
    FTransform Target_Transform;                                                      // 0x0100 (size: 0x60)
    class USkeletalMeshComponent* owning_skel;                                        // 0x01A0 (size: 0x8)

}; // Size: 0x1C0

struct FAnimNode_DragonFatSolver : public FAnimNode_DragonControlBase
{
    FDragonData_DragonFatBoneInput fat_bone_input;                                    // 0x0120 (size: 0x10)
    FDragonData_DragonFatControlInput fat_control_input;                              // 0x0130 (size: 0x10)
    float Fat_Strength_Value;                                                         // 0x0140 (size: 0x4)
    float Fat_Alpha_Value;                                                            // 0x0144 (size: 0x4)
    bool Use_Axis_Limitation;                                                         // 0x0148 (size: 0x1)
    class USkeletalMeshComponent* owning_skel;                                        // 0x0170 (size: 0x8)

}; // Size: 0x178

struct FAnimNode_DragonFeetSolver : public FAnimNode_DragonControlBase
{
    class UDragonIKTraceManagerComponent* Dragon_Trace_Manager;                       // 0x00D0 (size: 0x8)
    bool use_trace_manager_component;                                                 // 0x00D8 (size: 0x1)
    int32 solver_identifier;                                                          // 0x00DC (size: 0x4)
    FDragonData_MultiInput dragon_input_data;                                         // 0x00E8 (size: 0x20)
    EIK_Type_Plugin ik_type;                                                          // 0x01A0 (size: 0x1)
    EIKTrace_Type_Plugin trace_type;                                                  // 0x01A1 (size: 0x1)
    float Trace_Radius;                                                               // 0x01A4 (size: 0x4)
    bool line_trace_hit_complex;                                                      // 0x01A8 (size: 0x1)
    float trace_interval_duration;                                                    // 0x01B8 (size: 0x4)
    bool Use_LOD_Specific_Intervals;                                                  // 0x01BC (size: 0x1)
    float LOD0_Interval;                                                              // 0x01C0 (size: 0x4)
    float LOD1_Interval;                                                              // 0x01C4 (size: 0x4)
    float LOD2_Interval;                                                              // 0x01C8 (size: 0x4)
    bool Use_Trace_Distance_Adapting;                                                 // 0x01CC (size: 0x1)
    float Trace_Max_Distance;                                                         // 0x01D0 (size: 0x4)
    bool Use_Interval_Velocity_Curve;                                                 // 0x01D4 (size: 0x1)
    FRuntimeFloatCurve Interval_Velocity_Curve;                                       // 0x01D8 (size: 0x88)
    bool Override_Curve_Velocity;                                                     // 0x0260 (size: 0x1)
    float custom_velocity;                                                            // 0x0264 (size: 0x4)
    EInterpoLocation_Type_Plugin loc_interp_type;                                     // 0x0268 (size: 0x1)
    EInterpoRotation_Type_Plugin rot_interp_type;                                     // 0x0274 (size: 0x1)
    float virtual_scale;                                                              // 0x0278 (size: 0x4)
    FDragonData_FeetAlpha_Struct feet_alpha_multiplier_array;                         // 0x0280 (size: 0x10)
    FDragonData_FeetOffset_Struct feet_extra_offset_array;                            // 0x0290 (size: 0x10)
    FRotator Overall_Feet_Rotation_Offset;                                            // 0x02A0 (size: 0x18)
    bool automatic_leg_make;                                                          // 0x02B8 (size: 0x1)
    bool Use_OptionalRef_Feet_As_Ref;                                                 // 0x02B9 (size: 0x1)
    bool enable_solver;                                                               // 0x02BA (size: 0x1)
    bool Work_Outside_PIE;                                                            // 0x02BB (size: 0x1)
    FComponentSpacePoseLink OptionalRefPose;                                          // 0x02C0 (size: 0x10)
    bool interpolate_only_z;                                                          // 0x02D0 (size: 0x1)
    float shift_speed;                                                                // 0x02D4 (size: 0x4)
    float Location_Lerp_Speed;                                                        // 0x02E0 (size: 0x4)
    float feet_rotation_speed;                                                        // 0x02E4 (size: 0x4)
    bool ignore_shift_speed;                                                          // 0x02E8 (size: 0x1)
    bool Ignore_Lerping;                                                              // 0x02E9 (size: 0x1)
    bool Ignore_Location_Lerping;                                                     // 0x02EA (size: 0x1)
    FRuntimeFloatCurve Interpolation_Velocity_Curve;                                  // 0x02F0 (size: 0x88)
    bool Enable_Complex_Rotation_Method;                                              // 0x0378 (size: 0x1)
    FRuntimeFloatCurve ComplexSimpleFoot_Velocity_Curve;                              // 0x0380 (size: 0x88)
    TEnumAsByte<ETraceTypeQuery> Trace_Channel;                                       // 0x0448 (size: 0x1)
    TEnumAsByte<ETraceTypeQuery> Anti_Trace_Channel;                                  // 0x0449 (size: 0x1)
    float FPS_Lerp_Treshold;                                                          // 0x044C (size: 0x4)
    float line_trace_upper_height;                                                    // 0x0614 (size: 0x4)
    float line_trace_down_height;                                                     // 0x0618 (size: 0x4)
    FRuntimeFloatCurve Trace_Down_Multiplier_Curve;                                   // 0x0620 (size: 0x88)
    bool Use_Anti_Channel;                                                            // 0x06A8 (size: 0x1)
    bool use_footstep_listening;                                                      // 0x06A9 (size: 0x1)
    class UDragonIKFootStepsComponent* dragon_footsteps_component;                    // 0x06B0 (size: 0x8)
    bool Should_Rotate_Feet;                                                          // 0x06B8 (size: 0x1)
    bool show_trace_in_game;                                                          // 0x06BA (size: 0x1)
    bool execute_in_preview;                                                          // 0x06BB (size: 0x1)
    class AActor* Character_Actor;                                                    // 0x06C8 (size: 0x8)
    bool Enable_Pitch;                                                                // 0x06D8 (size: 0x1)
    bool Enable_Roll;                                                                 // 0x06D9 (size: 0x1)
    FVector character_direction_vector_CS;                                            // 0x06E0 (size: 0x18)
    FVector character_forward_direction_vector_CS;                                    // 0x06F8 (size: 0x18)
    FVector poles_forward_direction_vector_CS;                                        // 0x0710 (size: 0x18)
    bool Use_Four_Point_Feets;                                                        // 0x0728 (size: 0x1)
    bool Enable_Foot_Lift_Limit;                                                      // 0x0729 (size: 0x1)
    bool Affect_Toes_Always;                                                          // 0x072A (size: 0x1)
    FRuntimeFloatCurve Finger_Alpha_Velocity_Curve;                                   // 0x0730 (size: 0x88)
    float Max_Limb_Radius;                                                            // 0x07B8 (size: 0x4)
    bool Feet_Lift_Warping;                                                           // 0x07BC (size: 0x1)
    float Feet_Lift_Warp_Multiplier;                                                  // 0x07C0 (size: 0x4)
    float feet_vertical_size;                                                         // 0x07C4 (size: 0x4)
    float Feet_Lift_Limit;                                                            // 0x07C8 (size: 0x4)
    bool sticky_feet_mode;                                                            // 0x07CC (size: 0x1)
    float sticky_feet_on_speed;                                                       // 0x07D0 (size: 0x4)
    float sticky_feet_off_speed;                                                      // 0x07D4 (size: 0x4)
    float Sticky_Feet_Range;                                                          // 0x07D8 (size: 0x4)
    bool Lock_Feet_Rotation;                                                          // 0x07DC (size: 0x1)
    FDragonData_StickyFeetStruct sticky_feets_data;                                   // 0x07E0 (size: 0x20)
    bool sticky_floor_detection;                                                      // 0x0800 (size: 0x1)
    float floor_value;                                                                // 0x0804 (size: 0x4)
    bool Auto_Sticky_Toggle;                                                          // 0x0808 (size: 0x1)
    FDragonData_StickySocketStruct sticky_sockets_data;                               // 0x0810 (size: 0x10)
    class USkeletalMeshComponent* owning_skel;                                        // 0x08D8 (size: 0x8)
    float Foot_01_Height_Offset;                                                      // 0x09A0 (size: 0x4)
    float Foot_02_Height_Offset;                                                      // 0x09A4 (size: 0x4)
    float Foot_03_Height_Offset;                                                      // 0x09A8 (size: 0x4)
    float Foot_04_Height_Offset;                                                      // 0x09AC (size: 0x4)

}; // Size: 0x9B0

struct FAnimNode_DragonPhysicsSolver : public FAnimNode_DragonControlBase
{
    class USkeletalMeshComponent* owning_skel;                                        // 0x00F8 (size: 0x8)
    int32 physanim_tag_index;                                                         // 0x0110 (size: 0x4)

}; // Size: 0x118

struct FAnimNode_DragonSpineSolver : public FAnimNode_DragonControlBase
{
    bool use_trace_manager_component;                                                 // 0x00D0 (size: 0x1)
    int32 solver_identifier;                                                          // 0x00D4 (size: 0x4)
    FDragonData_MultiInput dragon_input_data;                                         // 0x00E0 (size: 0x20)
    bool use_type2_input;                                                             // 0x0180 (size: 0x1)
    FDragonData_Type2MultiInput dragon_input_type2_data;                              // 0x0188 (size: 0x20)
    float Precision;                                                                  // 0x01A8 (size: 0x4)
    float RotationLimit;                                                              // 0x01AC (size: 0x4)
    int32 MaxIterations;                                                              // 0x01B0 (size: 0x4)
    class UDragonIKTraceManagerComponent* Dragon_Trace_Manager;                       // 0x01B8 (size: 0x8)
    float shift_speed;                                                                // 0x01C0 (size: 0x4)
    TEnumAsByte<ETraceTypeQuery> Trace_Channel;                                       // 0x01C4 (size: 0x1)
    TEnumAsByte<ETraceTypeQuery> Anti_Trace_Channel;                                  // 0x01C5 (size: 0x1)
    EIKTrace_Type_Plugin trace_type;                                                  // 0x01C6 (size: 0x1)
    float Trace_Radius;                                                               // 0x01C8 (size: 0x4)
    bool line_trace_hit_complex;                                                      // 0x01CC (size: 0x1)
    float trace_interval_duration;                                                    // 0x01DC (size: 0x4)
    bool Use_LOD_Specific_Intervals;                                                  // 0x01E0 (size: 0x1)
    float LOD0_Interval;                                                              // 0x01E4 (size: 0x4)
    float LOD1_Interval;                                                              // 0x01E8 (size: 0x4)
    float LOD2_Interval;                                                              // 0x01EC (size: 0x4)
    bool Use_Trace_Distance_Adapting;                                                 // 0x01F0 (size: 0x1)
    float Trace_Max_Distance;                                                         // 0x01F4 (size: 0x4)
    bool Use_Interval_Velocity_Curve;                                                 // 0x01F8 (size: 0x1)
    FRuntimeFloatCurve Interval_Velocity_Curve;                                       // 0x0200 (size: 0x88)
    bool Override_Curve_Velocity;                                                     // 0x0288 (size: 0x1)
    float custom_velocity;                                                            // 0x028C (size: 0x4)
    bool Rotate_Around_Translate;                                                     // 0x0290 (size: 0x1)
    ESolverComplexityPluginEnum complexity_type;                                      // 0x0291 (size: 0x1)
    bool Ignore_Lerping;                                                              // 0x0292 (size: 0x1)
    float virtual_scale;                                                              // 0x02A8 (size: 0x4)
    float line_trace_downward_height;                                                 // 0x02AC (size: 0x4)
    float line_trace_upper_height;                                                    // 0x02B0 (size: 0x4)
    bool Use_Anti_Channel;                                                            // 0x02B4 (size: 0x1)
    bool stabilize_pelvis_legs;                                                       // 0x02C8 (size: 0x1)
    float Pelvis_UpSlopeStabilization_Alpha;                                          // 0x02CC (size: 0x4)
    float Pelvis_DownSlopeStabilization_Alpha;                                        // 0x02D0 (size: 0x4)
    bool stabilize_chest_legs;                                                        // 0x02D4 (size: 0x1)
    float Chest_UpSlopeStabilization_Alpha;                                           // 0x02D8 (size: 0x4)
    float Chest_DownslopeStabilization_Alpha;                                         // 0x02DC (size: 0x4)
    FBoneReference Stabilization_Head_Bone;                                           // 0x02E0 (size: 0x10)
    FBoneReference Stabilization_Tail_Bone;                                           // 0x02F0 (size: 0x10)
    bool Use_Ducking_Feature;                                                         // 0x0300 (size: 0x1)
    TEnumAsByte<ETraceTypeQuery> Ducking_Trace_Channel;                               // 0x0301 (size: 0x1)
    float Ducking_Limit;                                                              // 0x0304 (size: 0x4)
    float Pelvis_Crouch_Height;                                                       // 0x0308 (size: 0x4)
    float Pelvis_Crouch_Rotation_Intensity;                                           // 0x030C (size: 0x4)
    FVector Duck_Pelvis_Trace_Offset;                                                 // 0x0310 (size: 0x18)
    float Chest_Crouch_Height;                                                        // 0x0328 (size: 0x4)
    float Chest_Crouch_Rotation_Intensity;                                            // 0x032C (size: 0x4)
    FVector Duck_Chest_Trace_Offset;                                                  // 0x0330 (size: 0x18)
    float Slanted_Height_Up_Offset;                                                   // 0x0578 (size: 0x4)
    float Slanted_Height_Down_Offset;                                                 // 0x0580 (size: 0x4)
    float dip_multiplier;                                                             // 0x0584 (size: 0x4)
    float pelvis_adaptive_gravity;                                                    // 0x0588 (size: 0x4)
    bool reverse_fabrik;                                                              // 0x058C (size: 0x1)
    bool Calculation_To_RefPose;                                                      // 0x0594 (size: 0x1)
    float Chest_Slanted_Height_Up_Offset;                                             // 0x0598 (size: 0x4)
    float Chest_Slanted_Height_Down_Offset;                                           // 0x059C (size: 0x4)
    float chest_side_dip_multiplier;                                                  // 0x05A0 (size: 0x4)
    float chest_adaptive_gravity;                                                     // 0x05A4 (size: 0x4)
    float Chest_Base_Offset;                                                          // 0x05A8 (size: 0x4)
    float Pelvis_Base_Offset;                                                         // 0x05AC (size: 0x4)
    float virtual_leg_width;                                                          // 0x05B0 (size: 0x4)
    float Maximum_Dip_Height;                                                         // 0x05B4 (size: 0x4)
    FRuntimeFloatCurve Pelvis_Height_Multiplier_Curve;                                // 0x05C0 (size: 0x88)
    float Maximum_Dip_Height_Chest;                                                   // 0x0648 (size: 0x4)
    FRuntimeFloatCurve Chest_Height_Multiplier_Curve;                                 // 0x0650 (size: 0x88)
    float rotation_power_between;                                                     // 0x06E4 (size: 0x4)
    bool Use_Automatic_Fabrik_Selection;                                              // 0x06E8 (size: 0x1)
    float Trace_Lerp_Speed;                                                           // 0x06EC (size: 0x4)
    float Location_Lerp_Speed;                                                        // 0x06F0 (size: 0x4)
    float Rotation_Lerp_Speed;                                                        // 0x0704 (size: 0x4)
    FRuntimeFloatCurve Interpolation_Multiplier_Curve;                                // 0x0710 (size: 0x88)
    float Chest_Influence_Alpha;                                                      // 0x0798 (size: 0x4)
    float Pelvis_ForwardRotation_Intensity;                                           // 0x079C (size: 0x4)
    float Pelvis_UpwardForwardRotation_Intensity;                                     // 0x07A0 (size: 0x4)
    float Body_Rotation_Intensity;                                                    // 0x07A4 (size: 0x4)
    FRotator Pelvis_Rotation_Offset_Rotator;                                          // 0x07A8 (size: 0x18)
    float Chest_ForwardRotation_Intensity;                                            // 0x07C0 (size: 0x4)
    float Chest_UpwardForwardRotation_Intensity;                                      // 0x07C4 (size: 0x4)
    float Chest_SidewardRotation_Intensity;                                           // 0x07C8 (size: 0x4)
    FRotator Chest_Rotation_Offset_Rotator;                                           // 0x07D0 (size: 0x18)
    bool Full_Extended_Spine;                                                         // 0x0850 (size: 0x1)
    float max_extension_ratio;                                                        // 0x0854 (size: 0x4)
    float min_extension_ratio;                                                        // 0x0858 (size: 0x4)
    float extension_switch_speed;                                                     // 0x085C (size: 0x4)
    bool enable_solver;                                                               // 0x0864 (size: 0x1)
    bool Work_Outside_PIE;                                                            // 0x0865 (size: 0x1)
    bool Use_Fake_Chest_Rotations;                                                    // 0x0866 (size: 0x1)
    bool Use_Fake_Pelvis_Rotations;                                                   // 0x0867 (size: 0x1)
    bool Force_Activation;                                                            // 0x086C (size: 0x1)
    bool accurate_feet_placement;                                                     // 0x086D (size: 0x1)
    FRuntimeFloatCurve Accurate_Foot_Curve;                                           // 0x0870 (size: 0x88)
    bool use_crosshair_trace_also_for_fail_distance;                                  // 0x08F8 (size: 0x1)
    bool Only_Root_Solve;                                                             // 0x08F9 (size: 0x1)
    bool Ignore_Chest_Solve;                                                          // 0x08FA (size: 0x1)
    FVector Overall_PostSolved_Offset;                                                // 0x0908 (size: 0x18)
    FVector character_direction_vector_CS;                                            // 0x0920 (size: 0x18)
    FVector Forward_Direction_Vector;                                                 // 0x0938 (size: 0x18)
    bool flip_forward_and_right;                                                      // 0x0950 (size: 0x1)
    ERefPosePluginEnum SolverReferencePose;                                           // 0x0970 (size: 0x1)
    bool Spine_Feet_Connect;                                                          // 0x0971 (size: 0x1)
    float Snake_Joint_Speed;                                                          // 0x0D34 (size: 0x4)
    bool Enable_Snake_Interpolation;                                                  // 0x0D38 (size: 0x1)
    bool is_snake;                                                                    // 0x0D39 (size: 0x1)
    bool Ignore_End_Points;                                                           // 0x0D3A (size: 0x1)
    float Maximum_Feet_Distance;                                                      // 0x0D3C (size: 0x4)
    float Minimum_Feet_Distance;                                                      // 0x0D40 (size: 0x4)
    bool DisplayLineTrace;                                                            // 0x0D44 (size: 0x1)
    class USkeletalMeshComponent* owning_skel;                                        // 0x0D50 (size: 0x8)
    class AActor* Character_Actor;                                                    // 0x0D60 (size: 0x8)
    class USkeleton* skeleton_ref;                                                    // 0x0D68 (size: 0x8)

}; // Size: 0xEA0

struct FAnimNode_DragonTransformRelayer : public FAnimNode_DragonControlBase
{
    class USkeletalMeshComponent* owning_skel;                                        // 0x00F8 (size: 0x8)
    class UDragonIKTransformReceiverComp* transform_receiver_component;               // 0x0100 (size: 0x8)

}; // Size: 0x1A8

struct FAnimNode_DragonTwistArmSolver : public FAnimNode_DragonControlBase
{
    FDragonData_ArmsData Hand_Input_Info;                                             // 0x00E8 (size: 0x200)
    FTransform Target_Transform;                                                      // 0x0300 (size: 0x60)
    FVector Extra_Elbow_Pole_Offset;                                                  // 0x0360 (size: 0x18)
    EPole_System_DragonIK pole_system_input;                                          // 0x037B (size: 0x1)
    ETwist_Type_DragonIK arm_twist_axis;                                              // 0x037C (size: 0x1)
    ERotation_Type_DragonIK hand_rotation_method;                                     // 0x037D (size: 0x1)
    bool Let_Arm_Twist_With_Hand;                                                     // 0x04A5 (size: 0x1)
    bool allow_arm_stretch;                                                           // 0x04A6 (size: 0x1)
    bool Enable_Interpolation;                                                        // 0x05A5 (size: 0x1)
    float Interpolation_Speed;                                                        // 0x05A8 (size: 0x4)
    FVector LookAt_Axis;                                                              // 0x0610 (size: 0x18)
    FVector Upward_Axis;                                                              // 0x0628 (size: 0x18)
    bool Use_Physics_Adaptation;                                                      // 0x0640 (size: 0x1)
    FName physics_root_reference;                                                     // 0x0644 (size: 0x8)
    bool DisplayLineTrace;                                                            // 0x0754 (size: 0x1)
    TArray<FTransform> Debug_Hand_Locations;                                          // 0x0758 (size: 0x10)
    class USkeletalMeshComponent* owning_skel;                                        // 0x0940 (size: 0x8)

}; // Size: 0xA00

struct FAnimNode_DragonWarpSolver : public FAnimNode_DragonControlBase
{
    TArray<FDragonData_WarpLimbsData> dragon_limb_input;                              // 0x00D0 (size: 0x10)
    FName Hip_Bone_Name;                                                              // 0x00E0 (size: 0x8)
    bool enable_solver;                                                               // 0x012C (size: 0x1)
    FVector character_direction_vector_CS;                                            // 0x02B8 (size: 0x18)
    FVector forward_vector_CS;                                                        // 0x02D0 (size: 0x18)
    float speed_warping_const;                                                        // 0x02E8 (size: 0x4)
    bool enable_slope_warp;                                                           // 0x02EC (size: 0x1)
    float automatic_speed_warping_const;                                              // 0x02F0 (size: 0x4)
    float slope_detection_tolerance;                                                  // 0x02F4 (size: 0x4)
    float Warp_Slope_Interpolation;                                                   // 0x02F8 (size: 0x4)
    TEnumAsByte<ETraceTypeQuery> Trace_Channel;                                       // 0x0360 (size: 0x1)
    float line_trace_downward_height;                                                 // 0x0364 (size: 0x4)
    float line_trace_upper_height;                                                    // 0x0368 (size: 0x4)
    float virtual_leg_width;                                                          // 0x036C (size: 0x4)
    float virtual_scale;                                                              // 0x0370 (size: 0x4)
    bool DisplayLineTrace;                                                            // 0x0378 (size: 0x1)
    float Limb_Compression_Intensity;                                                 // 0x037C (size: 0x4)
    FRuntimeFloatCurve Limb_Lifting_Curve;                                            // 0x0380 (size: 0x88)
    float Hip_Change_Intensity;                                                       // 0x0408 (size: 0x4)
    FRuntimeFloatCurve Hip_Lifting_Curve;                                             // 0x0410 (size: 0x88)
    class USkeletalMeshComponent* owning_skel;                                        // 0x06A8 (size: 0x8)

}; // Size: 0x6D0

struct FAnimNode_DragonWeaponArmSolver : public FAnimNode_DragonControlBase
{
    class UDragonIKWeaponPhysicsComponent* weapon_handler_component;                  // 0x00D0 (size: 0x8)
    int32 Physweapon_Component_Tag;                                                   // 0x0154 (size: 0x4)
    TArray<FDragonData_ArmsData> Aiming_Hand_Limbs;                                   // 0x0158 (size: 0x10)
    int32 primary_hand_index;                                                         // 0x0168 (size: 0x4)
    ETwist_Type_DragonIK arm_twist_axis;                                              // 0x01CC (size: 0x1)
    bool Let_Arm_Twist_With_Hand;                                                     // 0x02F5 (size: 0x1)
    bool Enable_Interpolation;                                                        // 0x03F5 (size: 0x1)
    float Interpolation_Speed;                                                        // 0x03F8 (size: 0x4)
    FVector LookAt_Axis;                                                              // 0x0730 (size: 0x18)
    FVector Upward_Axis;                                                              // 0x0748 (size: 0x18)
    bool DisplayLineTrace;                                                            // 0x08A8 (size: 0x1)
    TArray<FTransform> Debug_Hand_Locations;                                          // 0x08B0 (size: 0x10)
    class USkeletalMeshComponent* owning_skel;                                        // 0x0AA0 (size: 0x8)

}; // Size: 0xB60

struct FAnimNode_TailSolver : public FAnimNode_DragonControlBase
{
    FName HowToUse;                                                                   // 0x00D8 (size: 0x8)
    FDragonData_TailBoneInput Custom_Bone_Input;                                      // 0x0108 (size: 0x10)
    bool use_custom_heights;                                                          // 0x0130 (size: 0x1)
    float Tail_Bone_Height;                                                           // 0x0134 (size: 0x4)
    FDragonData_TailHeightInput Custom_Bone_Heights;                                  // 0x0138 (size: 0x10)
    FVector character_direction_vector_CS;                                            // 0x0148 (size: 0x18)
    float Animation_Interpolation_Speed;                                              // 0x0170 (size: 0x4)
    TEnumAsByte<ETraceTypeQuery> Trace_Channel;                                       // 0x0174 (size: 0x1)
    bool line_trace_hit_complex;                                                      // 0x0175 (size: 0x1)
    bool Use_Sphere_Trace;                                                            // 0x0176 (size: 0x1)
    float Sphere_Trace_Radius;                                                        // 0x0178 (size: 0x4)
    float Trace_Up_Height;                                                            // 0x0240 (size: 0x4)
    float Trace_Down_Height;                                                          // 0x0244 (size: 0x4)
    float TraceBlock_Interpolation_Speed;                                             // 0x0248 (size: 0x4)
    float TraceUnBlock_Interpolation_Speed;                                           // 0x024C (size: 0x4)
    float trace_interval_duration;                                                    // 0x0290 (size: 0x4)
    bool Use_LOD_Specific_Intervals;                                                  // 0x0294 (size: 0x1)
    float LOD0_Interval;                                                              // 0x0298 (size: 0x4)
    float LOD1_Interval;                                                              // 0x029C (size: 0x4)
    float LOD2_Interval;                                                              // 0x02A0 (size: 0x4)
    bool Use_Trace_Distance_Adapting;                                                 // 0x02A4 (size: 0x1)
    float Trace_Max_Distance;                                                         // 0x02A8 (size: 0x4)
    FTransform CachedEffectorCSTransform;                                             // 0x02B0 (size: 0x60)
    class USkeletalMeshComponent* owning_skel;                                        // 0x0318 (size: 0x8)

}; // Size: 0x380

struct FBoneDragonSocketTarget
{
    bool bUseSocket;                                                                  // 0x0000 (size: 0x1)
    FBoneReference BoneReference;                                                     // 0x0004 (size: 0x10)
    FSocketDragonReference SocketReference;                                           // 0x0020 (size: 0x70)

}; // Size: 0x90

struct FCCDIK_Modified_ChainLink
{
}; // Size: 0xA0

struct FChinaDragonChainLink
{
}; // Size: 0x1D0

struct FDragonData_AimSolver_SkeletonInput
{
    TArray<FName> Bone_Input;                                                         // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_ArmSizeStruct
{
    bool Use_Custom_Arm_Sizes;                                                        // 0x0000 (size: 0x1)
    float custom_upperArm_length;                                                     // 0x0004 (size: 0x4)
    float custom_lowerArm_length;                                                     // 0x0008 (size: 0x4)

}; // Size: 0xC

struct FDragonData_ArmsData
{
    FBoneReference Clavicle_Bone;                                                     // 0x0000 (size: 0x10)
    FBoneReference Shoulder_Bone_Name;                                                // 0x0010 (size: 0x10)
    FBoneReference Elbow_Bone_Name;                                                   // 0x0020 (size: 0x10)
    FBoneReference Hand_Bone_Name;                                                    // 0x0030 (size: 0x10)
    bool is_this_right_hand;                                                          // 0x0040 (size: 0x1)
    bool invert_lower_twist;                                                          // 0x0041 (size: 0x1)
    bool invert_upper_twist;                                                          // 0x0042 (size: 0x1)
    FBoneReference Twist_Forearm_Bone;                                                // 0x0044 (size: 0x10)
    FBoneReference Twist_Shoulder_Bone;                                               // 0x0054 (size: 0x10)
    FVector Local_Direction_Axis;                                                     // 0x0068 (size: 0x18)
    FVector Arm_Aiming_Offset;                                                        // 0x0080 (size: 0x18)
    bool accurate_hand_rotation;                                                      // 0x0098 (size: 0x1)
    bool relative_axis;                                                               // 0x0099 (size: 0x1)
    float Maximum_Extension;                                                          // 0x009C (size: 0x4)
    float Minimum_Extension;                                                          // 0x00A0 (size: 0x4)
    float Max_Stretch_Ratio;                                                          // 0x00A4 (size: 0x4)
    float Stretch_lower_arm_Priorty;                                                  // 0x00A8 (size: 0x4)
    FVector Elbow_Pole_Offset;                                                        // 0x00B0 (size: 0x18)
    FVector North_Pole_Offset;                                                        // 0x00C8 (size: 0x18)
    FVector South_Pole_Offset;                                                        // 0x00E0 (size: 0x18)
    FVector West_Pole_Offset;                                                         // 0x00F8 (size: 0x18)
    FVector East_Pole_Offset;                                                         // 0x0110 (size: 0x18)
    bool override_limits;                                                             // 0x0128 (size: 0x1)
    FVector2D Max_Arm_H_Angle;                                                        // 0x0130 (size: 0x10)
    FVector2D Max_Arm_V_Angle;                                                        // 0x0140 (size: 0x10)
    FVector2D Inner_Clavicle_Side_Limit;                                              // 0x0150 (size: 0x10)
    FVector2D Inner_Clavicle_Vertical_Limit;                                          // 0x0160 (size: 0x10)
    FVector2D Outer_Clavicle_Side_Limit;                                              // 0x0170 (size: 0x10)
    FVector2D Outer_Clavicle_Vertical_Limit;                                          // 0x0180 (size: 0x10)
    FVector2D Shoulder_Inner_Clamp;                                                   // 0x0190 (size: 0x10)
    FVector2D Shoulder_Outer_Clamp;                                                   // 0x01A0 (size: 0x10)
    FVector2D ForeArm_Angle_Limit;                                                    // 0x01B0 (size: 0x10)
    float Twist_Offset_Reverse;                                                       // 0x01C0 (size: 0x4)
    bool should_shoulder_twist_if_inward;                                             // 0x01C4 (size: 0x1)
    bool clavicle_part_of_extension;                                                  // 0x01C5 (size: 0x1)

}; // Size: 0x200

struct FDragonData_BoneFatStruct
{
    FName bone_name;                                                                  // 0x0000 (size: 0x8)
    EInput_FatBoneFAxis Bone_Axis;                                                    // 0x0008 (size: 0x1)

}; // Size: 0x1C

struct FDragonData_ChineseDragonBoneInput
{
    TArray<FName> Custom_Bone_Structure;                                              // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_ChineseDragonBoneRotationOffset
{
    TArray<FRotator> Custom_Bone_Rotation_Offset;                                     // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_CustomArmLengths
{
    TArray<FDragonData_ArmSizeStruct> CustomArmSizeArray;                             // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_CustomLimbClamp
{
    TArray<float> Custom_Limb_Clamp_Array;                                            // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_DragonFatBoneInput
{
    TArray<FDragonData_BoneFatStruct> Custom_Bone_Structure;                          // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_DragonFatControlInput
{
    TArray<float> FatControlStrengthArray;                                            // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_ElbowOffset_Struct
{
    TArray<FVector> elbow_offset_array;                                               // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_FeetAlpha_Struct
{
    TArray<float> feet_IK_alpha_array;                                                // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_FeetOffset_Struct
{
    TArray<FVector> feet_extra_offset_array;                                          // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_FingerData
{
    FName Finger_Bone_Name;                                                           // 0x0000 (size: 0x8)
    float Trace_Scale;                                                                // 0x0008 (size: 0x4)
    FVector Trace_Offset;                                                             // 0x0010 (size: 0x18)
    bool Is_Finger_Backward;                                                          // 0x0028 (size: 0x1)

}; // Size: 0x30

struct FDragonData_FootData
{
    FName Feet_Bone_Name;                                                             // 0x0000 (size: 0x8)
    FName Knee_Bone_Name;                                                             // 0x0008 (size: 0x8)
    FName Second_Knee_Bone_Name;                                                      // 0x0010 (size: 0x8)
    FName Thigh_Bone_Name;                                                            // 0x0018 (size: 0x8)
    FRotator Feet_Rotation_Offset;                                                    // 0x0028 (size: 0x18)
    bool Fixed_Pole;                                                                  // 0x0040 (size: 0x1)
    FVector Knee_Direction_Offset;                                                    // 0x0048 (size: 0x18)
    FVector Feet_Trace_Offset;                                                        // 0x0060 (size: 0x18)
    float Front_Trace_Point_Spacing;                                                  // 0x0078 (size: 0x4)
    float Side_Traces_Spacing;                                                        // 0x007C (size: 0x4)
    float Feet_Rotation_Limit;                                                        // 0x0080 (size: 0x4)
    bool Fixed_Foot_Height;                                                           // 0x0084 (size: 0x1)
    float Feet_Heights;                                                               // 0x0088 (size: 0x4)
    float Feet_Alpha;                                                                 // 0x008C (size: 0x4)
    float Min_Feet_Extension;                                                         // 0x0090 (size: 0x4)
    float Max_Feet_Extension;                                                         // 0x0094 (size: 0x4)
    float Feet_Upslope_Offset_Multiplier;                                             // 0x0098 (size: 0x4)
    float Feet_Downslope_Offset_Multiplier;                                           // 0x009C (size: 0x4)
    float Feet_Slope_Offset_Clamp_Radius;                                             // 0x00A0 (size: 0x4)
    float Max_Feet_Lift;                                                              // 0x00A4 (size: 0x4)
    float Overrided_Trace_Radius;                                                     // 0x00A8 (size: 0x4)
    TArray<FName> FeetFollowBones;                                                    // 0x00B0 (size: 0x10)
    TArray<FDragonData_FingerData> Finger_Array;                                      // 0x00C0 (size: 0x10)

}; // Size: 0xD0

struct FDragonData_FootPairData
{
    FName CommonSpineBone;                                                            // 0x0000 (size: 0x8)
    TArray<FName> ChildFollowBones;                                                   // 0x0008 (size: 0x10)
    TArray<FDragonData_FootData> Feet_Pair_Array;                                     // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FDragonData_MultiInput
{
    FName Start_Spine;                                                                // 0x0000 (size: 0x8)
    FName Pelvis;                                                                     // 0x0008 (size: 0x8)
    TArray<FDragonData_FootData> FeetBones;                                           // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FDragonData_Overrided_Location_Data
{
    TArray<FDragonData_SingleArmElement> Arm_TargetLocation_Overrides;                // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_PhysicsBoneStrip
{
    FBoneReference Bone_Chain_Start;                                                  // 0x0000 (size: 0x10)
    FBoneReference Bone_Chain_End;                                                    // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FDragonData_PhysicsHandleMultiplier
{
    FName bone_name;                                                                  // 0x0000 (size: 0x8)
    float bone_strength;                                                              // 0x0008 (size: 0x4)

}; // Size: 0xC

struct FDragonData_PhysicsParentRelationship
{
    FName child_bone_name;                                                            // 0x0000 (size: 0x8)
    FName parent_bone_name;                                                           // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FDragonData_SingleArmElement
{
    FTransform Overrided_Arm_Transform;                                               // 0x0000 (size: 0x60)
    float Arm_Alpha;                                                                  // 0x0060 (size: 0x4)
    FRotator rotation_offset;                                                         // 0x0068 (size: 0x18)

}; // Size: 0x80

struct FDragonData_StickyFeetStruct
{
    TArray<bool> sticky_feet_array;                                                   // 0x0000 (size: 0x10)
    TArray<float> sticky_feet_alpha_array;                                            // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FDragonData_StickySocketStruct
{
    TArray<FBoneSocketTarget> sticky_socket_array;                                    // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_TailBoneInput
{
    TArray<FName> Custom_Bone_Structure;                                              // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_TailHeightInput
{
    TArray<float> Tail_Chain_Heights;                                                 // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FDragonData_Type2MultiInput
{
    TArray<FName> Spine_Structure;                                                    // 0x0000 (size: 0x10)
    TArray<FDragonData_FootPairData> FeetRelatedData;                                 // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FDragonData_WarpLimbsData
{
    FName Foot_Bone_Name;                                                             // 0x0000 (size: 0x8)
    FName Knee_Bone_Name;                                                             // 0x0008 (size: 0x8)
    FName Thigh_Bone_Name;                                                            // 0x0010 (size: 0x8)
    float Warp_Lift_Reference_Location;                                               // 0x0018 (size: 0x4)
    float Warp_Param_Adder;                                                           // 0x001C (size: 0x4)
    FVector2D Min_Max_Warp;                                                           // 0x0020 (size: 0x10)
    float max_extra_compression_height;                                               // 0x0030 (size: 0x4)

}; // Size: 0x38

struct FDragonIKTraceKeyValuePair
{
    FName Key;                                                                        // 0x0000 (size: 0x8)
    FName BoneName;                                                                   // 0x0008 (size: 0x8)
    FHitResult hit_result;                                                            // 0x0010 (size: 0xE8)

}; // Size: 0xF8

struct FDragonIKTraceMParams
{
}; // Size: 0x138

struct FDragonIKTraceParamKeyValuePair
{
    FName Key;                                                                        // 0x0000 (size: 0x8)
    FDragonIKTraceMParams trace_params;                                               // 0x0008 (size: 0x138)

}; // Size: 0x140

struct FDragonTailChainLink
{
}; // Size: 0x260

struct FSocketDragonReference
{
    FName SocketName;                                                                 // 0x0060 (size: 0x8)

}; // Size: 0x70

class ADragonIKPhysicsHolder : public AActor
{
    TArray<class UPhysicsHandleComponent*> physics_handle_array;                      // 0x0298 (size: 0x10)
    class AActor* connected_actor;                                                    // 0x02A8 (size: 0x8)

}; // Size: 0x2B0

class UDragonIKFootStepsComponent : public UActorComponent
{
    FDragonIKFootStepsComponentOnCharacterFootStep OnCharacterFootStep;               // 0x00A0 (size: 0x10)
    void CharacterFootStep(FName bone_name, FTransform foot_transform, float height_difference, FHitResult hit_info);
    FName read_this;                                                                  // 0x0214 (size: 0x8)
    TArray<FName> foot_socket;                                                        // 0x0220 (size: 0x10)
    float foot_enter_height;                                                          // 0x0230 (size: 0x4)
    float foot_exit_height;                                                           // 0x0234 (size: 0x4)

    void CallFootSteps(FName bone_name, FTransform foot_transform, float height_difference, FHitResult hit_info);
}; // Size: 0x240

class UDragonIKPhysicsComponent : public UActorComponent
{
    class USkeletalMeshComponent* owning_skeleton;                                    // 0x00B0 (size: 0x8)
    class ADragonIKPhysicsHolder* handler_holder;                                     // 0x00B8 (size: 0x8)
    int32 physanim_tag_index;                                                         // 0x00C0 (size: 0x4)
    FName Custom_Root_Bone;                                                           // 0x00C4 (size: 0x8)
    TArray<FName> individual_bone_selection;                                          // 0x00D0 (size: 0x10)
    TArray<FName> Influence_Stopping_Bones;                                           // 0x0100 (size: 0x10)
    TArray<FDragonData_PhysicsHandleMultiplier> Override_Handle_Strength_Multiplier;  // 0x0110 (size: 0x10)
    int32 Linear_Strength;                                                            // 0x0150 (size: 0x4)
    int32 Angular_Strength;                                                           // 0x0154 (size: 0x4)
    int32 Linear_Damp;                                                                // 0x0158 (size: 0x4)
    int32 Angular_Damp;                                                               // 0x015C (size: 0x4)
    int32 Interpolation_Speed;                                                        // 0x0160 (size: 0x4)
    float Teleport_Distance;                                                          // 0x0164 (size: 0x4)
    bool parent_child_ragdoll_state;                                                  // 0x01D0 (size: 0x1)
    TMap<class FName, class FName> Parent_Child_Connection_Array;                     // 0x01D8 (size: 0x50)
    bool use_auto_blending_logic;                                                     // 0x0228 (size: 0x1)
    float Autoblendalpha;                                                             // 0x022C (size: 0x4)
    bool modify_physics_states;                                                       // 0x0231 (size: 0x1)
    float Hit_Enable_Speed;                                                           // 0x0234 (size: 0x4)
    float Hit_Disable_Speed;                                                          // 0x0238 (size: 0x4)
    float hit_delay_time;                                                             // 0x023C (size: 0x4)
    FRuntimeFloatCurve Physanim_Enter_Curve;                                          // 0x0248 (size: 0x88)
    bool blend_to_zero;                                                               // 0x02D0 (size: 0x1)
    class USceneComponent* mesh_parent_reference;                                     // 0x03E0 (size: 0x8)

    void UpdateSpecificPhysanimData(FName BoneName, int32 linear_strength_var, int32 angular_strength_var, int32 linear_damp_var, int32 angular_damp_var);
    void UpdatePhysanimData(int32 LinearStrength, int32 AngularStrength, int32 LinearDamp, int32 AngularDamp);
    void SetInterpolation(int32 inter_value);
    void Server_UpdatePhysanimData(int32 LinearStrength, int32 AngularStrength, int32 LinearDamp, int32 AngularDamp);
    void Server_SetInterpolation(int32 inter_value);
    void Server_ReleaseAllHandles(bool go_ragdoll);
    void Server_ReGrabAllHandles(bool exit_ragdoll);
    void Server_PerformActiveRagdoll(float pelvis_mass_override);
    void Server_OnHitCallback(const FName Optional_Override_Hit_Bone_Name);
    void Server_Modify_Reset_Bone_Info(const TArray<FName>& bone_list);
    void Server_FirstTimeInitialization(class USkeletalMeshComponent* skeleton_input);
    void Server_DragonIKSimulatePhysics(class USkeletalMeshComponent* skmesh, FName BoneName, bool IncludeSelf);
    void Server_Control_Bone_Handle(FName bone_name, FTransform override_transform, bool Enable);
    void Server_CacheSimulationStates();
    void ReleaseAllHandles(bool go_ragdoll);
    void ReGrabAllHandles(bool exit_ragdoll);
    void PerformActiveRagdoll(float pelvis_mass_override);
    void OnHitCallback(const FHitResult& Hit, FName Optional_Override_Hit_Bone_Name, FVector& hit_location, FVector& hit_impulse_direction, FName& hit_bone_name);
    void Multicast_UpdatePhysanimData(int32 LinearStrength, int32 AngularStrength, int32 LinearDamp, int32 AngularDamp);
    void Multicast_SetInterpolation(int32 inter_value);
    void Multicast_ReleaseAllHandles(bool go_ragdoll);
    void Multicast_ReGrabAllHandles(bool exit_ragdoll);
    void Multicast_PerformActiveRagdoll(float pelvis_mass_override);
    void Multicast_OnHitCallback(const FName Optional_Override_Hit_Bone_Name);
    void Multicast_Modify_Reset_Bone_Info(const TArray<FName>& bone_list);
    void Multicast_FirstTimeInitialization(class USkeletalMeshComponent* skeleton_input);
    void Multicast_DragonIKSimulatePhysics(class USkeletalMeshComponent* skmesh, FName BoneName, bool IncludeSelf);
    void Multicast_Control_Bone_Handle(FName bone_name, FTransform override_transform, bool Enable);
    void Multicast_CacheSimulationStates();
    void Modify_Reset_Bone_Info(TArray<FName> bone_list);
    void FirstTimeInitialization(class USkeletalMeshComponent* skeleton_input);
    void Control_Bone_Handle(FName bone_name, FTransform override_transform, bool Enable);
    void CacheSimulationStates();
}; // Size: 0x460

class UDragonIKTraceManagerComponent : public UActorComponent
{
    FName read_this;                                                                  // 0x00A0 (size: 0x8)
    bool Is_Enabled;                                                                  // 0x00A8 (size: 0x1)
    bool bShow_Spine_Trace_Lines_InGame;                                              // 0x00A9 (size: 0x1)
    bool bShow_Foot_Trace_Lines_InGame;                                               // 0x00AA (size: 0x1)
    FDragonIKTraceManagerComponentOnDragonikFootHitData OnDragonikFootHitData;        // 0x00E8 (size: 0x10)
    void DragonIKFootSolverTraceData(const TArray<FDragonIKTraceKeyValuePair>& hit_data_pair);

    void SpineSolverTraceData(const TArray<FDragonIKTraceKeyValuePair>& hit_data_pair);
}; // Size: 0xF8

class UDragonIKTransformReceiverComp : public UActorComponent
{
    class USkeletalMeshComponent* owning_skeleton;                                    // 0x00B0 (size: 0x8)
    FName read_this;                                                                  // 0x00B8 (size: 0x8)
    TMap<class FName, class FTransform> World_Name_Transform_Map;                     // 0x00C0 (size: 0x50)
    TMap<class FName, class FTransform> Component_Name_Transform_Map;                 // 0x0110 (size: 0x50)

}; // Size: 0x160

class UDragonIKWeaponPhysicsComponent : public UActorComponent
{
    class USkeletalMeshComponent* owning_skeleton;                                    // 0x00B0 (size: 0x8)
    class UPhysicsHandleComponent* weapon_physics_handle;                             // 0x00B8 (size: 0x8)
    class UPhysicsConstraintComponent* weapon_physics_constraint;                     // 0x00C0 (size: 0x8)
    class UPrimitiveComponent* weapon_input_component;                                // 0x00D0 (size: 0x8)
    class UPrimitiveComponent* active_weapon_component;                               // 0x00D8 (size: 0x8)
    FName read_this;                                                                  // 0x04B0 (size: 0x8)
    float Max_Arm_Length_Percent;                                                     // 0x04B8 (size: 0x4)
    int32 physanim_tag_index;                                                         // 0x04BC (size: 0x4)
    int32 Linear_Strength;                                                            // 0x04C0 (size: 0x4)
    int32 Angular_Strength;                                                           // 0x04C4 (size: 0x4)
    int32 Linear_Damp;                                                                // 0x04C8 (size: 0x4)
    int32 Angular_Damp;                                                               // 0x04CC (size: 0x4)
    int32 Interpolation_Speed;                                                        // 0x04D0 (size: 0x4)
    float unfreeze_speed;                                                             // 0x04D4 (size: 0x4)
    bool is_grabbing;                                                                 // 0x04D8 (size: 0x1)
    float Teleport_Distance;                                                          // 0x04DC (size: 0x4)

    void Weapon_Transform_Override(FTransform Weapon_Transform, bool bShould_Override_Weapon_Transform);
    void Unfreeze_Elbows_Function(FName hand_bone);
    void Server_Weapon_Transform_Override(FTransform Weapon_Transform, bool bShould_Override_Weapon_Transform);
    void Server_Unfreeze_Elbows_Function(FName hand_bone);
    void Server_Release_Weapon(bool should_simulatephysics, bool use_gravity, bool bKeep_Attachment);
    void Server_Release_Hand_Plus_Override(FName hand_bone, FTransform hand_transform, bool override_hand_location, bool Override_Hand_Rotation);
    void Server_Release_Hand_Only(FName hand_bone);
    void Server_Grab_Weapon(FName grabbed_bone, bool simulate_physics, bool use_gravity, FName WeaponBoneName);
    void Server_Freeze_Elbows_Function(FName hand_bone);
    void Server_FirstTimeInitialization(class USkeletalMeshComponent* skeleton_input);
    void Server_EnableWeaponsArms();
    void Server_DisableWeaponsArms(bool reset_weapon_transform);
    void Server_Destroy_Weapon(class AActor* weapon);
    void Server_Attach_Hand_Only(FName hand_bone, bool arm_position_recalibrate);
    void Release_Weapon(bool should_simulatephysics, bool use_gravity, bool bKeep_Attachment);
    void Release_Hand_Plus_Override(FName hand_bone, FTransform hand_transform, bool override_hand_location, bool Override_Hand_Rotation);
    void Release_Hand_Only(FName hand_bone);
    void Multicast_Weapon_Transform_Override(FTransform Weapon_Transform, bool bShould_Override_Weapon_Transform);
    void Multicast_Unfreeze_Elbows_Function(FName hand_bone);
    void Multicast_Release_Weapon(bool should_simulatephysics, bool use_gravity, bool bKeep_Attachment);
    void Multicast_Release_Hand_Plus_Override(FName hand_bone, FTransform hand_transform, bool override_hand_location, bool Override_Hand_Rotation);
    void Multicast_Release_Hand_Only(FName hand_bone);
    void Multicast_Grab_Weapon(FName grabbed_bone, bool simulate_physics, bool use_gravity, FName WeaponBoneName);
    void Multicast_Freeze_Elbows_Function(FName hand_bone);
    void Multicast_FirstTimeInitialization(class USkeletalMeshComponent* skeleton_input);
    void Multicast_EnableWeaponsArms();
    void Multicast_DisableWeaponsArms(bool reset_weapon_transform);
    void Multicast_Destroy_Weapon(class AActor* weapon);
    void Multicast_Attach_Hand_Only(FName hand_bone, bool arm_position_recalibrate);
    void Grab_Weapon(FName grabbed_bone, bool simulate_physics, bool use_gravity, FName WeaponBoneName);
    void Freeze_Elbows_Function(FName hand_bone);
    void FirstTimeInitialization(class USkeletalMeshComponent* skeleton_input);
    void EnableWeaponsArms();
    void DisableWeaponsArms(bool reset_weapon_transform);
    void Destroy_Weapon(class AActor* weapon);
    void Attach_Hand_Only(FName hand_bone, bool arm_position_recalibrate);
}; // Size: 0x630

class UDragonIK_Library : public UObject
{

    FTransform QuatLookXatLocation(const FTransform& LookAtFromTransform, const FVector& LookAtTarget);
    FRotator LookAtVector_V2(FVector Source_Location, FVector lookAt, FVector upDirection);
    FRotator LookAtRotation_V3(FVector Source, FVector Target, FVector UpVector);
    FRotator CustomLookRotation(FVector lookAt, FVector upDirection);
}; // Size: 0x30

#endif

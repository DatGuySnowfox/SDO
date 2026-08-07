#ifndef UE4SS_SDK_Buildable_WoodenWallDoor_HPP
#define UE4SS_SDK_Buildable_WoodenWallDoor_HPP

class ABuildable_WoodenWallDoor_C : public ABuildable_WoodenWall_C
{
    class UBoxComponent* NavObstacleBox2;                                             // 0x0520 (size: 0x8)
    class UBoxComponent* NavObstacleBox1;                                             // 0x0528 (size: 0x8)
    class UChildActorComponent* Door2;                                                // 0x0530 (size: 0x8)
    class UChildActorComponent* Door1;                                                // 0x0538 (size: 0x8)
    class USceneComponent* Doors;                                                     // 0x0540 (size: 0x8)

}; // Size: 0x548

#endif

#ifndef UE4SS_SDK_BPI_AI_HPP
#define UE4SS_SDK_BPI_AI_HPP

class IBPI_AI_C : public IInterface
{

    void CallAttackEvent(bool& Attack?);
    void CanAIAttack?(bool& CanAttack?);
    void GetMovementSpeed(TEnumAsByte<Enum_ZombieMovementType::Type> MovementType, double& Speed);
    void ChangeMovementSpeed(TEnumAsByte<Enum_ZombieMovementType::Type> MovementType);
    void OverrideName(FString Name);
    void HitByVehicle(double Speed, FVector Direction);
    void SetBurning(bool Burning?, double Dmg, class AActor* Actor);
    void IsAIBurning?(bool& IsBurning?);
    void GetMarker(class UTexture2D*& Texture, FLinearColor& Color, double& MarkerOffset);
    void CanAddMarkerToAI?(bool& CanAdd?);
    void AddMarker(class UWidgetComponent* Marker);
    void GetMesh(class USkeletalMeshComponent*& Mesh);
    void CanStompAI?(bool& Stomp?);
    void AIHealthBarInfo(bool& Visible?, FString& Name, bool& Boss?);
}; // Size: 0x28

#endif

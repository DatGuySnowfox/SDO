#ifndef UE4SS_SDK_BPI_AI_HPP
#define UE4SS_SDK_BPI_AI_HPP

class IBPI_AI_C : public IInterface
{

    void OverrideName(FString Name);
    void HitByVehicle(double Speed, FVector Direction);
    void IsAIBurning?(bool& IsBurning?);
    void SetBurning(bool Burning?, double Dmg, class AActor* Actor);
    void GetMarker(class UTexture2D*& Texture, FLinearColor& Color, double& MarkerOffset);
    void CanAddMarkerToAI?(bool& CanAdd?);
    void AddMarker(class UWidgetComponent* Marker);
    void GetMesh(class USkeletalMeshComponent*& Mesh);
    void GetMovementSpeeds(double& RoamingSpeed, double& AlertSpeed, double& AttackSpeed);
    void CanStompAI?(bool& Stomp?);
    void AIHealthBarInfo(bool& Visible?, FString& Name, bool& Boss?);
}; // Size: 0x28

#endif

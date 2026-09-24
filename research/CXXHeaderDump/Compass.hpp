#ifndef UE4SS_SDK_Compass_HPP
#define UE4SS_SDK_Compass_HPP

class UCompass_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class URetainerBox* RetainerBox;                                                  // 0x0348 (size: 0x8)
    class UImage* CompassPoints;                                                      // 0x0350 (size: 0x8)
    class UCanvasPanel* CompassPanel;                                                 // 0x0358 (size: 0x8)
    class UImage* CompassMarker;                                                      // 0x0360 (size: 0x8)
    class UBorder* Border_99;                                                         // 0x0368 (size: 0x8)
    class UWidgetAnimation* FadeOut;                                                  // 0x0370 (size: 0x8)
    class UWidgetAnimation* FadeIn;                                                   // 0x0378 (size: 0x8)
    class UCameraComponent* Rotation;                                                 // 0x0380 (size: 0x8)
    TArray<UCompassMarkerUI_C*> UsedObjectWidgets;                                    // 0x0388 (size: 0x10)

    void RemoveFromCompass(class UObject* Object);
    void UsedActor(TArray<UCompassMarkerUI_C*>& Widgets, class UObject* Actor, bool& Used);
    double NewXPosition(class AActor* Object);
    void GetWidgetReference(class UObject* Object, class UCompassMarkerUI_C*& Array Element);
    void ObjectBehind(class AActor* Target, bool& Behind);
    void NewPointPosition(class AActor* Object);
    void NavPoints();
    void SetDirection();
    void Construct();
    void OnVisibilityChanged_Event_0(ESlateVisibility InVisibility);
    void NavPointsTimer();
    void ExecuteUbergraph_Compass(int32 EntryPoint);
}; // Size: 0x398

#endif

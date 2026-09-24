#ifndef UE4SS_SDK_MapWidget_HPP
#define UE4SS_SDK_MapWidget_HPP

class UMapWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* West;                                                           // 0x0348 (size: 0x8)
    class UOverlay* StandardMarkerOverlay;                                            // 0x0350 (size: 0x8)
    class UTextBlock* South;                                                          // 0x0358 (size: 0x8)
    class UOverlay* SmallPOIMarkerOverlay;                                            // 0x0360 (size: 0x8)
    class UImage* PlayerTexture;                                                      // 0x0368 (size: 0x8)
    class UPlayerMarkerSelectionUI_C* PlayerMarkerSelectionUI;                        // 0x0370 (size: 0x8)
    class UTextBlock* North;                                                          // 0x0378 (size: 0x8)
    class UImage* Map;                                                                // 0x0380 (size: 0x8)
    class UOverlay* LargePOIMarkerOverlay;                                            // 0x0388 (size: 0x8)
    class UCanvasPanel* InnerCanvas;                                                  // 0x0390 (size: 0x8)
    class UTextBlock* East;                                                           // 0x0398 (size: 0x8)
    class UCheckBox* CheckBox_ToggleSpecialMarkers;                                   // 0x03A0 (size: 0x8)
    class UCheckBox* CheckBox_TogglePOIMarkers;                                       // 0x03A8 (size: 0x8)
    class ABP_MapData_C* Map Data;                                                    // 0x03B0 (size: 0x8)
    bool DraggingMap?;                                                                // 0x03B8 (size: 0x1)
    double ZoomFactor;                                                                // 0x03C0 (size: 0x8)
    int32 MaxZoom;                                                                    // 0x03C8 (size: 0x4)
    int32 CurrentZoomStep;                                                            // 0x03CC (size: 0x4)
    int32 MinZoom;                                                                    // 0x03D0 (size: 0x4)
    TArray<UW_StandardMarker_C*> Standard Marker References;                          // 0x03D8 (size: 0x10)
    TArray<UW_PlayerMarker_C*> Player Marker References;                              // 0x03E8 (size: 0x10)
    TArray<UW_QuestMarker_C*> Quest Marker References;                                // 0x03F8 (size: 0x10)
    TArray<UW_POIMarker_C*> POI Marker References;                                    // 0x0408 (size: 0x10)
    TArray<UW_WorldMarker_C*> World Marker References;                                // 0x0418 (size: 0x10)

    void AddWorldCreatedMarker(FS_MarkerData Marker, class AActor* Target Actor, FText Description, class UW_WorldMarker_C*& Return);
    void SetScreenPositionToPlayer();
    void OnScroll();
    void AddPOICreatedMarker(FS_MarkerData Marker, class AActor* Target Actor, bool Explored?, FStruct_POI POI Data, class UW_POIMarker_C*& Return);
    void AddQuestCreatedMarker(FS_MarkerData Marker, class ABP_QuestMarker_C* Target Actor, class UW_QuestMarker_C*& Return);
    void Add Standard Marker(FS_MarkerData Marker, class AActor* Target Actor, class UW_StandardMarker_C*& Return);
    void AddPlayerCreatedMarker(FS_MarkerData Marker, class ABP_PlayerMarker_C* Target Actor, class UW_PlayerMarker_C*& Return);
    FEventReply OnMouseButtonDoubleClick(FGeometry InMyGeometry, const FPointerEvent& InMouseEvent);
    FVector2D CalculateMarkerToWorld(FVector2D Mouse Position);
    FEventReply OnMouseWheel(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void ClampSides(const FGeometry& Geometry, FVector2D ClampTo, FVector2D& Clamped);
    FEventReply OnMouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FEventReply OnMouseMove(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void Calculate Actor Position(class AActor* Actor, FVector2D& Location);
    void Construct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ToggleMap(bool Toggle);
    void AddPlayerMarker();
    void BndEvt__MapWidget_CheckBox_ToggleMarkers_K2Node_ComponentBoundEvent_1_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked);
    void BndEvt__MapWidget_CheckBox_TogglePOIMarkers_K2Node_ComponentBoundEvent_0_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked);
    void ExecuteUbergraph_MapWidget(int32 EntryPoint);
}; // Size: 0x428

#endif

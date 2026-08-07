#ifndef UE4SS_SDK_MapWidget_HPP
#define UE4SS_SDK_MapWidget_HPP

class UMapWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UCheckBox* CheckBox_TogglePOIMarkers;                                       // 0x02C8 (size: 0x8)
    class UCheckBox* CheckBox_ToggleSpecialMarkers;                                   // 0x02D0 (size: 0x8)
    class UTextBlock* East;                                                           // 0x02D8 (size: 0x8)
    class UCanvasPanel* InnerCanvas;                                                  // 0x02E0 (size: 0x8)
    class UOverlay* LargePOIMarkerOverlay;                                            // 0x02E8 (size: 0x8)
    class UImage* Map;                                                                // 0x02F0 (size: 0x8)
    class UTextBlock* North;                                                          // 0x02F8 (size: 0x8)
    class UPlayerMarkerSelectionUI_C* PlayerMarkerSelectionUI;                        // 0x0300 (size: 0x8)
    class UImage* PlayerTexture;                                                      // 0x0308 (size: 0x8)
    class UOverlay* SmallPOIMarkerOverlay;                                            // 0x0310 (size: 0x8)
    class UTextBlock* South;                                                          // 0x0318 (size: 0x8)
    class UOverlay* StandardMarkerOverlay;                                            // 0x0320 (size: 0x8)
    class UTextBlock* West;                                                           // 0x0328 (size: 0x8)
    class ABP_MapData_C* Map Data;                                                    // 0x0330 (size: 0x8)
    bool DraggingMap?;                                                                // 0x0338 (size: 0x1)
    double ZoomFactor;                                                                // 0x0340 (size: 0x8)
    int32 MaxZoom;                                                                    // 0x0348 (size: 0x4)
    int32 CurrentZoomStep;                                                            // 0x034C (size: 0x4)
    int32 MinZoom;                                                                    // 0x0350 (size: 0x4)
    TArray<class UW_StandardMarker_C*> Standard Marker References;                    // 0x0358 (size: 0x10)
    TArray<class UW_PlayerMarker_C*> Player Marker References;                        // 0x0368 (size: 0x10)
    TArray<class UW_QuestMarker_C*> Quest Marker References;                          // 0x0378 (size: 0x10)
    TArray<class UW_POIMarker_C*> POI Marker References;                              // 0x0388 (size: 0x10)
    TArray<class UW_WorldMarker_C*> World Marker References;                          // 0x0398 (size: 0x10)

    void AddWorldCreatedMarker(FS_MarkerData Marker, class AActor* Target Actor, FText Description, class UW_WorldMarker_C*& Return);
    void SetScreenPositionToPlayer();
    void OnScroll();
    void AddPOICreatedMarker(FS_MarkerData Marker, class AActor* Target Actor, bool Explored?, FStruct_POI POI Data, class UW_POIMarker_C*& Return);
    void AddQuestCreatedMarker(FS_MarkerData Marker, class ABP_QuestMarker_C* Target Actor, class UW_QuestMarker_C*& Return);
    void Add Standard Marker(FS_MarkerData Marker, class AActor* Target Actor, class UW_StandardMarker_C*& Return);
    void AddPlayerCreatedMarker(FS_MarkerData Marker, class ABP_PlayerMarker_C* Target Actor, int32 Index, class UW_PlayerMarker_C*& Return);
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
    void AddMarker();
    void BndEvt__MapWidget_CheckBox_ToggleMarkers_K2Node_ComponentBoundEvent_1_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked);
    void BndEvt__MapWidget_CheckBox_TogglePOIMarkers_K2Node_ComponentBoundEvent_0_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked);
    void ExecuteUbergraph_MapWidget(int32 EntryPoint);
}; // Size: 0x3A8

#endif

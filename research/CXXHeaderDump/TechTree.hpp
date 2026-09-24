#ifndef UE4SS_SDK_TechTree_HPP
#define UE4SS_SDK_TechTree_HPP

#include "TechTree_enums.hpp"

struct FTechNodeLink
{
    class UTechnologyAsset* ParentTechnology;                                         // 0x0000 (size: 0x8)
    class UTechnologyAsset* ChildTechnology;                                          // 0x0008 (size: 0x8)
    FVector2D LinkFromPostion;                                                        // 0x0010 (size: 0x10)
    FVector2D LinkToPostion;                                                          // 0x0020 (size: 0x10)

}; // Size: 0x30

struct FUnlockedTechnologiesData
{
    TArray<UTechnologyAsset*> UnlockedTechnologies;                                   // 0x0000 (size: 0x10)

}; // Size: 0x10

class UTTBaseNode : public UObject
{
    int32 NodePosX;                                                                   // 0x0028 (size: 0x4)
    int32 NodePosY;                                                                   // 0x002C (size: 0x4)
    TArray<UTTBaseNode*> ParentNodes;                                                 // 0x0030 (size: 0x10)
    TArray<UTTBaseNode*> ChildNodes;                                                  // 0x0040 (size: 0x10)
    uint32 UniqueID;                                                                  // 0x0050 (size: 0x4)

}; // Size: 0x58

class UTTTechNode : public UTTBaseNode
{
    class UTechnologyAsset* TechnologyAsset;                                          // 0x0058 (size: 0x8)
    bool IsUnlockedByDefault;                                                         // 0x0060 (size: 0x1)

}; // Size: 0x70

class UTechNodeArrowWidget : public UUserWidget
{
    ETechNodePinType PinType;                                                         // 0x0340 (size: 0x1)
    float ConnectionAngle;                                                            // 0x0344 (size: 0x4)
    class UTechnologyAsset* ParentTechnology;                                         // 0x0348 (size: 0x8)
    class UTechnologyAsset* ChildTechnology;                                          // 0x0350 (size: 0x8)

    void OnNodeArrowInitialized();
}; // Size: 0x358

class UTechNodeWidget : public UUserWidget
{
    class UTechnologyAsset* TechnologyAsset;                                          // 0x0340 (size: 0x8)

    void OnTechNodeInitialized();
}; // Size: 0x348

class UTechTreeManager : public UActorComponent
{
    FTechTreeManagerOnTechnologyUnlockStateChanged OnTechnologyUnlockStateChanged;    // 0x00B8 (size: 0x10)
    void OnTechnologyUnlockStateChanged(class UTechnologyTree* TechTree, class UTTTechNode* TechNode, bool IsUnlocked);
    FTechTreeManagerOnTechTreeAssigned OnTechTreeAssigned;                            // 0x00C8 (size: 0x10)
    void OnTechTreeAssigned(class UTechnologyTree* TechTree);
    TSubclassOf<class UTechnologySaveGame> SaveGameTemplate;                          // 0x00D8 (size: 0x8)
    FTechTreeManagerOnTechTreeRecovered OnTechTreeRecovered;                          // 0x00E0 (size: 0x10)
    void OnTechTreeRecovered(const class UTechnologyTree* TechTree);
    class UTechnologyTree* TechnologyTree;                                            // 0x00F0 (size: 0x8)
    class UTechTreeWidget* TechTreeWidget;                                            // 0x00F8 (size: 0x8)
    TMap<UTechnologyTree*, UTechnologyTree*> AssignedTechTrees;                       // 0x0100 (size: 0x50)

    void UnlockTechnology(class UTechnologyAsset* Technology);
    void SelectTechTree(class UTechnologyTree* TechTree);
    void SaveTechToObject(class USaveGame* SaveGameObject);
    void SaveTechStateFromSlot(FString SlotName, const int32 UserIndex);
    void Reset();
    void OnTechTreeRecovered__DelegateSignature(const class UTechnologyTree* TechTree);
    void OnTechTreeAssigned__DelegateSignature(class UTechnologyTree* TechTree);
    void OnTechnologyUnlockStateChanged__DelegateSignature(class UTechnologyTree* TechTree, class UTTTechNode* TechNode, bool IsUnlocked);
    void LockTechnology(class UTechnologyAsset* Technology);
    void LoadTechStateFromSlot(FString SlotName, const int32 UserIndex);
    void LoadTechFromObject(class USaveGame* SaveGameObject);
    bool IsTechnologyUnlocked(class UTechnologyAsset* Technology);
    bool IsChildTechnology(class UTTTechNode* ParentNode, class UTechnologyAsset* ChildNodeTechnology);
    bool HasUnlockedParents(class UTechnologyAsset* Technology);
    void HandleTechnologyStateChanged(class UTTTechNode* TechNode, bool IsUnlocked);
    class UTechTreeWidget* GetTechTreeWidget();
    class USaveGame* GetSavegameTechObject();
    int32 GetNumberTreesAssigned();
    class UTTTechNode* GetNodeFromTechAsset(class UTechnologyAsset* Technology);
    class UTechnologyTree* GetAssignedTechTree();
    void AssignTechTree(class UTechnologyTree* TechTree, class UTechTreeWidget* TechTreeWidget);
}; // Size: 0x158

class UTechTreeWidget : public UWidget
{
    class UTechnologyTree* TechTree;                                                  // 0x0168 (size: 0x8)
    TArray<TWeakObjectPtr<class UTechNodeWidget>> SlotWidgetsCached;                  // 0x01B0 (size: 0x10)
    TArray<TWeakObjectPtr<class UTechNodeArrowWidget>> ArrowWidgetsCached;            // 0x01C0 (size: 0x10)
    TArray<FTechNodeLink> ConnectionsCached;                                          // 0x01D0 (size: 0x10)

    void UpdateConnections(const TArray<FTechNodeLink>& Connections);
    void UpdateArrowPinWidgets();
    TArray<UTechNodeWidget*> GetTechNodeWidgets();
    class UTechNodeWidget* GetTechNode(class UTechnologyAsset* TechnologyAsset);
    TArray<UTechNodeArrowWidget*> GetArrowWidgets();
}; // Size: 0x1E0

class UTechnologyAsset : public UPrimaryDataAsset
{
    FSlateBrush TechIcon;                                                             // 0x0030 (size: 0xB0)

}; // Size: 0xE0

class UTechnologySaveGame : public USaveGame
{
    TArray<UTechnologyAsset*> UnlockedTechnologies;                                   // 0x0028 (size: 0x10)
    TMap<UTechnologyTree*, FUnlockedTechnologiesData> MultipleTreesUnlockedTechnologies; // 0x0038 (size: 0x50)

}; // Size: 0x88

class UTechnologyTree : public UObject
{
    FVector2D TreeSize;                                                               // 0x0028 (size: 0x10)
    ETechTreeOrientation Orientation;                                                 // 0x0038 (size: 0x1)
    FSlateBrush BackgroundImage;                                                      // 0x0040 (size: 0xB0)
    float ConnectionLineThickness;                                                    // 0x00F0 (size: 0x4)
    FLinearColor ConnectionLineColor;                                                 // 0x00F4 (size: 0x10)
    TSubclassOf<class UTechNodeWidget> SlotTemplate;                                  // 0x0108 (size: 0x8)
    FVector2D SlotSize;                                                               // 0x0110 (size: 0x10)
    TSubclassOf<class UTechNodeArrowWidget> ArrowTemplate;                            // 0x0120 (size: 0x8)
    FVector2D ArrowSize;                                                              // 0x0128 (size: 0x10)
    int32 GridSnapSize;                                                               // 0x0138 (size: 0x4)
    bool ShowBoundaries;                                                              // 0x013C (size: 0x1)
    FLinearColor BoundariesColor;                                                     // 0x0140 (size: 0x10)
    float BoundariesThickness;                                                        // 0x0150 (size: 0x4)
    bool ShowPinArrows;                                                               // 0x0154 (size: 0x1)
    bool ShowAlignmentGrid;                                                           // 0x0155 (size: 0x1)
    FVector2D AlignmentGridSize;                                                      // 0x0158 (size: 0x10)
    FVector2D AlignmentGridOffset;                                                    // 0x0168 (size: 0x10)
    FLinearColor AlignmentGridColor;                                                  // 0x0178 (size: 0x10)
    float AlignmentGridThickness;                                                     // 0x0188 (size: 0x4)
    TArray<UTTTechNode*> TechNodes;                                                   // 0x0190 (size: 0x10)
    TMap<UTechnologyAsset*, UTTTechNode*> TechnologiesMap;                            // 0x01A0 (size: 0x50)
    class UEdGraph* UpdateGraph;                                                      // 0x01F0 (size: 0x8)
    int32 UniqueID;                                                                   // 0x01F8 (size: 0x4)
    FTechnologyTreeOnTechnologyUnlockStateChanged OnTechnologyUnlockStateChanged;     // 0x0200 (size: 0x10)
    void OnTechnologyUnlockStateChanged(class UTTTechNode* TechNode, bool IsUnlocked);
    TMap<UTechnologyAsset*, UTTTechNode*> UnlockedTechnologies;                       // 0x0210 (size: 0x50)
    TArray<UTTTechNode*> CurrentlyActiveNodes;                                        // 0x0260 (size: 0x10)
    class UTechnologyTree* TechTreeTemplate;                                          // 0x0270 (size: 0x8)

    void OnTechnologyUnlockStateChanged__DelegateSignature(class UTTTechNode* TechNode, bool IsUnlocked);
    bool IsTechnologyUnlocked(class UTechnologyAsset* Technology);
    bool HasUnlockedParents(class UTechnologyAsset* Technology);
    FString GetTechAssetName();
    TArray<FTechNodeLink> GetConnections();
    TArray<FTechNodeLink> FilterConnections(const TArray<FTechNodeLink>& ConnectionsIn, const TArray<UTechnologyAsset*>& IgnoredParentTechnologies, const TArray<UTechnologyAsset*>& IgnoredChildTechnologies);
}; // Size: 0x280

#endif

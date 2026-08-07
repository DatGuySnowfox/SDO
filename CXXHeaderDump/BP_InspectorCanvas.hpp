#ifndef UE4SS_SDK_BP_InspectorCanvas_HPP
#define UE4SS_SDK_BP_InspectorCanvas_HPP

class UBP_InspectorCanvas_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UCanvasPanel* MainCanvas;                                                   // 0x02C8 (size: 0x8)
    class UCanvasPanelSlot* CurrentActive;                                            // 0x02D0 (size: 0x8)
    FBP_InspectorCanvas_COnNoActivableWFound OnNoActivableWFound;                     // 0x02D8 (size: 0x10)
    void OnNoActivableWFound();
    FBP_InspectorCanvas_COnActivableAdded OnActivableAdded;                           // 0x02E8 (size: 0x10)
    void OnActivableAdded(class UWidget* Widget);
    FBP_InspectorCanvas_COnRequestInventoryActivation OnRequestInventoryActivation;   // 0x02F8 (size: 0x10)
    void OnRequestInventoryActivation();

    void ActivateHighestZ();
    void AddNewItem(class UWidget* Content);
    void SetActiveWindow(class UCanvasPanelSlot* CanvasSlot);
    void OnWindowRemoved(class UUserWidget* Widget);
    void ExecuteUbergraph_BP_InspectorCanvas(int32 EntryPoint);
    void OnRequestInventoryActivation__DelegateSignature();
    void OnActivableAdded__DelegateSignature(class UWidget* Widget);
    void OnNoActivableWFound__DelegateSignature();
}; // Size: 0x308

#endif

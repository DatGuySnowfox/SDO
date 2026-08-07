#ifndef UE4SS_SDK_UDS_Cloud_Paint_Container_HPP
#define UE4SS_SDK_UDS_Cloud_Paint_Container_HPP

class AUDS_Cloud_Paint_Container_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02A0 (size: 0x8)
    class USceneCaptureComponent2D* SceneCaptureComponent2D;                          // 0x02A8 (size: 0x8)
    TMap<class FIntPoint, class UUDS_Cloud_Paint_Cell_C*> Cloud Painting Cell Data;   // 0x02B0 (size: 0x50)
    int32 Cloud Painting Cell Size;                                                   // 0x0300 (size: 0x4)
    int32 Cloud Painting Cell Resolution;                                             // 0x0304 (size: 0x4)
    TMap<class FIntPoint, class UTextureRenderTarget2D*> Cell Render Targets;         // 0x0308 (size: 0x50)

    void ReceiveBeginPlay();
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void ExecuteUbergraph_UDS_Cloud_Paint_Container(int32 EntryPoint);
}; // Size: 0x358

#endif

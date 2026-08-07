#ifndef UE4SS_SDK_UDS_Utility_Opener_Interface_HPP
#define UE4SS_SDK_UDS_Utility_Opener_Interface_HPP

class IUDS_Utility_Opener_Interface_C : public IInterface
{

    void Show Editor Warning Window(FString Title, FString Message);
    void Open UDS Readme Entry Set(const TArray<FName>& Entries);
    void Open UDS Readme(FString Entry Row, int32 Category Index);
}; // Size: 0x28

#endif

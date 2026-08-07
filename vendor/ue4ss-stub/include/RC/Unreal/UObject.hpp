#pragma once
#include "Core.hpp"

namespace RC::Unreal {

// Minimal UObject surface matching UE4SS.dll exports:
//   ?ProcessEvent@UObject@Unreal@RC@@QEAAXPEAVUFunction@23@PEAX@Z
//   ?GetValuePtrByPropertyNameInChain@UObject@Unreal@RC@@QEAAPEAXPEB_W@Z
//   ?GetFunctionByNameInChain@UObject@Unreal@RC@@QEAAPEAVUFunction@23@PEB_W@Z
//   ?GetFullName@UObject@Unreal@RC@@QEBA?AV?$basic_string@...@Z
//   ?IsA@UObjectBase@Unreal@RC@@QEBA_NPEAVUClass@23@@Z
//   ?GetClassPrivate@UObjectBase@Unreal@RC@@QEAAAEAPEAVUClass@23@XZ

class UObject {
public:
    // Call a UFUNCTION by address; params is a pointer to the in/out struct.
    RC_API void ProcessEvent(UFunction* fn, void* params);

    // Walk the property chain and return a pointer to the named property value.
    RC_API void* GetValuePtrByPropertyNameInChain(const wchar_t* name);

    // Walk the function chain and return the named UFunction.
    RC_API UFunction* GetFunctionByNameInChain(const wchar_t* name);

    // Human-readable path (slow – debug only)
    RC_API std::wstring GetFullName(UObject* stopOuter = nullptr) const;

    // Type checks
    RC_API bool IsA(UClass* cls) const;
    RC_API UClass*& GetClassPrivate();
};

} // namespace RC::Unreal

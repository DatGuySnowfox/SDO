#pragma once
#include "UObject.hpp"
#include <vector>

// UE4SS.dll exports these as free functions (YA mangling) but we declare them
// as static class methods (SA mangling) to keep call-site syntax unchanged.
// These pragmas tell the linker to satisfy each SA __imp_ with the YA one from UE4SS.lib.
#pragma comment(linker, "/alternatename:__imp_?FindFirstOf@UObjectGlobals@Unreal@RC@@SAPEAVUObject@23@PEB_W@Z=__imp_?FindFirstOf@UObjectGlobals@Unreal@RC@@YAPEAVUObject@23@PEB_W@Z")
#pragma comment(linker, "/alternatename:__imp_?FindAllOf@UObjectGlobals@Unreal@RC@@SAXPEB_WAEAV?$vector@PEAVUObject@Unreal@RC@@V?$allocator@PEAVUObject@Unreal@RC@@@std@@@std@@@Z=__imp_?FindAllOf@UObjectGlobals@Unreal@RC@@YAXPEB_WAEAV?$vector@PEAVUObject@Unreal@RC@@V?$allocator@PEAVUObject@Unreal@RC@@@std@@@std@@@Z")
#pragma comment(linker, "/alternatename:__imp_?FindObject@UObjectGlobals@Unreal@RC@@SAPEAVUObject@23@PEAVUClass@23@PEAV423@PEB_W_NPEAUObjectSearcher@23@@Z=__imp_?FindObject@UObjectGlobals@Unreal@RC@@YAPEAVUObject@23@PEAVUClass@23@PEAV423@PEB_W_NPEAUObjectSearcher@23@@Z")
#pragma comment(linker, "/alternatename:__imp_?ForEachUObject@UObjectGlobals@Unreal@RC@@SAXAEBV?$function@$$A6A?AW4LoopAction@RC@@PEAVUObject@Unreal@2@HH@Z@std@@@Z=__imp_?ForEachUObject@UObjectGlobals@Unreal@RC@@YAXAEBV?$function@$$A6A?AW4LoopAction@RC@@PEAVUObject@Unreal@2@HH@Z@std@@@Z")

namespace RC::Unreal {

enum class LoopAction { Continue, Break };
struct ObjectSearcher; // opaque

class UObjectGlobals {
public:
    // Returns the first UObject whose class name matches (e.g. L"BP_PlayerController_C").
    RC_API static UObject* FindFirstOf(const wchar_t* className);

    // Fills vec with every matching instance.
    RC_API static void FindAllOf(const wchar_t* className,
                                 std::vector<UObject*>& vec);

    // Full search with class/outer/name filters.
    RC_API static UObject* FindObject(UClass*         Class,
                                      UObject*        InOuter,
                                      const wchar_t*  Name,
                                      bool            ExactClass = false,
                                      ObjectSearcher* searcher   = nullptr);

    // Iterate every live UObject.
    RC_API static void ForEachUObject(
        const std::function<LoopAction(UObject*, int32_t, int32_t)>& fn);
};

} // namespace RC::Unreal

#pragma once
#include "UObject.hpp"

namespace RC::Unreal {

// Opaque – we only need pointers to UClass and UFunction.
class UFunction : public UObject {};
class UClass    : public UObject {};
class UEngine   : public UObject {};

} // namespace RC::Unreal

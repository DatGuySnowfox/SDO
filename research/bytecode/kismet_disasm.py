#!/usr/bin/env python3
"""Best-effort Kismet (UE5) bytecode disassembler for linked/runtime script
arrays (pointers already resolved, not a cooked/relocatable blob).

EExprToken values and operand shapes per public UE5 source
(Engine/Source/Runtime/CoreUObject/Public/UObject/Script.h and
Class.cpp's SerializeExpr). Best-effort: unknown/rare opcodes fall back to
a raw byte dump of a small guessed window rather than aborting, so a single
misunderstood opcode doesn't take down the rest of the decode.
"""
import sys, struct

# Authoritative table fetched from CUE4Parse (FabianFG/CUE4Parse, the open-source
# library FModel itself uses) — CUE4Parse/UE4/Kismet/EExprToken.cs. Cross-checked
# against every empirical anchor from JSI_Slot_C::OnDrop's 33-byte stub (0x00, 0x04,
# 0x14, 0x28, 0x48, 0x53) and all matched exactly. An earlier hand-typed version of
# this table had at least two wrong slots (0x14, 0x48) caught by that same anchor —
# this replaces it entirely rather than patching further guesses.
TOK = {
    0x00: "EX_LocalVariable", 0x01: "EX_InstanceVariable", 0x02: "EX_DefaultVariable",
    0x04: "EX_Return", 0x06: "EX_Jump", 0x07: "EX_JumpIfNot", 0x09: "EX_Assert",
    0x0B: "EX_Nothing", 0x0C: "EX_NothingInt32", 0x0F: "EX_Let", 0x11: "EX_BitFieldConst",
    0x12: "EX_ClassContext", 0x13: "EX_MetaCast", 0x14: "EX_LetBool",
    0x15: "EX_EndParmValue", 0x16: "EX_EndFunctionParms", 0x17: "EX_Self", 0x18: "EX_Skip",
    0x19: "EX_Context", 0x1A: "EX_Context_FailSilent", 0x1B: "EX_VirtualFunction",
    0x1C: "EX_FinalFunction", 0x1D: "EX_IntConst", 0x1E: "EX_FloatConst",
    0x1F: "EX_StringConst", 0x20: "EX_ObjectConst", 0x21: "EX_NameConst",
    0x22: "EX_RotationConst", 0x23: "EX_VectorConst", 0x24: "EX_ByteConst",
    0x25: "EX_IntZero", 0x26: "EX_IntOne", 0x27: "EX_True", 0x28: "EX_False",
    0x29: "EX_TextConst", 0x2A: "EX_NoObject", 0x2B: "EX_TransformConst",
    0x2C: "EX_IntConstByte", 0x2D: "EX_NoInterface", 0x2E: "EX_DynamicCast",
    0x2F: "EX_StructConst", 0x30: "EX_EndStructConst", 0x31: "EX_SetArray",
    0x32: "EX_EndArray", 0x33: "EX_PropertyConst", 0x34: "EX_UnicodeStringConst",
    0x35: "EX_Int64Const", 0x36: "EX_UInt64Const", 0x37: "EX_DoubleConst", 0x38: "EX_Cast",
    0x39: "EX_SetSet", 0x3A: "EX_EndSet", 0x3B: "EX_SetMap", 0x3C: "EX_EndMap",
    0x3D: "EX_SetConst", 0x3E: "EX_EndSetConst", 0x3F: "EX_MapConst", 0x40: "EX_EndMapConst",
    0x41: "EX_Vector3fConst", 0x42: "EX_StructMemberContext", 0x43: "EX_LetMulticastDelegate",
    0x44: "EX_LetDelegate", 0x45: "EX_LocalVirtualFunction", 0x46: "EX_LocalFinalFunction",
    0x48: "EX_LocalOutVariable", 0x4A: "EX_DeprecatedOp4A", 0x4B: "EX_InstanceDelegate",
    0x4C: "EX_PushExecutionFlow", 0x4D: "EX_PopExecutionFlow", 0x4E: "EX_ComputedJump",
    0x4F: "EX_PopExecutionFlowIfNot", 0x50: "EX_Breakpoint", 0x51: "EX_InterfaceContext",
    0x52: "EX_ObjToInterfaceCast", 0x53: "EX_EndOfScript", 0x54: "EX_CrossInterfaceCast",
    0x55: "EX_InterfaceToObjCast", 0x5A: "EX_WireTracepoint", 0x5B: "EX_SkipOffsetConst",
    0x5C: "EX_AddMulticastDelegate", 0x5D: "EX_ClearMulticastDelegate", 0x5E: "EX_Tracepoint",
    0x5F: "EX_LetObj", 0x60: "EX_LetWeakObjPtr", 0x61: "EX_BindDelegate",
    0x62: "EX_RemoveMulticastDelegate", 0x63: "EX_CallMulticastDelegate",
    0x64: "EX_LetValueOnPersistentFrame", 0x65: "EX_ArrayConst", 0x66: "EX_EndArrayConst",
    0x67: "EX_SoftObjectConst", 0x68: "EX_CallMath", 0x69: "EX_SwitchValue",
    0x6A: "EX_InstrumentationEvent", 0x6B: "EX_ArrayGetByRef", 0x6C: "EX_ClassSparseDataVariable",
    0x6D: "EX_FieldPathConst", 0x70: "EX_AutoRtfmTransact", 0x71: "EX_AutoRtfmStopTransact",
    0x72: "EX_AutoRtfmAbortIfNot", 0x73: "EX_AutoRtfmAbort",
}

NO_OPERAND = {
    "EX_Nothing", "EX_EndParmValue", "EX_EndFunctionParms", "EX_Self", "EX_EndOfScript",
    "EX_EndStructConst", "EX_EndArray", "EX_EndSet", "EX_EndMap",
    "EX_EndSetConst", "EX_EndMapConst", "EX_EndArrayConst", "EX_IntZero", "EX_IntOne",
    "EX_True", "EX_False", "EX_NoObject", "EX_NoInterface", "EX_Self", "EX_PopExecutionFlow",
    "EX_Breakpoint", "EX_WireTracepoint", "EX_Tracepoint", "EX_DeprecatedOp4A",
}

class Reader:
    def __init__(self, data):
        self.d = data
        self.i = 0
    def u8(self):
        v = self.d[self.i]; self.i += 1; return v
    def i32(self):
        v = struct.unpack_from("<i", self.d, self.i)[0]; self.i += 4; return v
    def u32(self):
        v = struct.unpack_from("<I", self.d, self.i)[0]; self.i += 4; return v
    def u64(self):
        v = struct.unpack_from("<Q", self.d, self.i)[0]; self.i += 8; return v
    def f32(self):
        v = struct.unpack_from("<f", self.d, self.i)[0]; self.i += 4; return v
    def f64(self):
        v = struct.unpack_from("<d", self.d, self.i)[0]; self.i += 8; return v
    def cstr(self):
        start = self.i
        while self.d[self.i] != 0: self.i += 1
        s = self.d[start:self.i].decode("ascii", errors="replace")
        self.i += 1
        return s
    def wstr(self):
        start = self.i
        while not (self.d[self.i] == 0 and self.d[self.i+1] == 0):
            self.i += 2
        s = self.d[start:self.i].decode("utf-16-le", errors="replace")
        self.i += 2
        return s
    def eof(self):
        return self.i >= len(self.d)

out = []

def emit(depth, s):
    out.append("  " * depth + s)

def parse_expr(r: Reader, depth=0, max_depth=40):
    if r.eof():
        emit(depth, "<eof>")
        return
    off = r.i
    op = r.u8()
    name = TOK.get(op, f"EX_UNKNOWN_0x{op:02x}")

    if depth > max_depth:
        emit(depth, f"[0x{off:04x}] {name} <max depth, stopping recursion>")
        return

    if name in NO_OPERAND:
        emit(depth, f"[0x{off:04x}] {name}")
        return

    try:
        if name in ("EX_LocalVariable", "EX_InstanceVariable", "EX_DefaultVariable", "EX_PropertyConst",
                    "EX_ClassSparseDataVariable"):
            ptr = r.u64()
            emit(depth, f"[0x{off:04x}] {name} prop=0x{ptr:x}")

        elif name == "EX_Return":
            emit(depth, f"[0x{off:04x}] {name}")
            parse_expr(r, depth+1, max_depth)

        elif name in ("EX_Jump",):
            target = r.u32()
            emit(depth, f"[0x{off:04x}] {name} -> 0x{target:04x}")

        elif name in ("EX_JumpIfNot",):
            target = r.u32()
            emit(depth, f"[0x{off:04x}] {name} -> 0x{target:04x} if NOT:")
            parse_expr(r, depth+1, max_depth)

        elif name == "EX_PushExecutionFlow":
            target = r.u32()
            emit(depth, f"[0x{off:04x}] {name} -> 0x{target:04x}")

        elif name == "EX_PopExecutionFlowIfNot":
            emit(depth, f"[0x{off:04x}] {name} if NOT:")
            parse_expr(r, depth+1, max_depth)

        elif name == "EX_ComputedJump":
            emit(depth, f"[0x{off:04x}] {name} offset_expr:")
            parse_expr(r, depth+1, max_depth)

        elif name == "EX_Assert":
            line = struct.unpack_from("<H", r.d, r.i)[0]; r.i += 2
            dbg = r.u8()
            emit(depth, f"[0x{off:04x}] {name} line={line} dbg={dbg}")
            parse_expr(r, depth+1, max_depth)

        elif name == "EX_Skip":
            skip = r.u32()
            emit(depth, f"[0x{off:04x}] {name} skip={skip}")
            parse_expr(r, depth+1, max_depth)

        elif name in ("EX_LetBool", "EX_LetObj", "EX_LetWeakObjPtr",
                      "EX_LetDelegate", "EX_LetMulticastDelegate"):
            # No leading property pointer for the specialized Let variants —
            # empirically confirmed for EX_LetBool against the 33-byte anchor.
            emit(depth, f"[0x{off:04x}] {name}")
            emit(depth+1, "variable:")
            parse_expr(r, depth+2, max_depth)
            emit(depth+1, "assignment:")
            parse_expr(r, depth+2, max_depth)

        elif name == "EX_LetValueOnPersistentFrame":
            # Different shape from the other specialized Lets: a direct
            # 8-byte destination (the persistent-frame slot identifier, not
            # a heap pointer — smaller/differently-ranged values are
            # expected) followed by a single Assignment expression, no
            # nested "variable" expression at all. Empirically confirmed
            # against BP_JigMultiplayer_C::HandleItemOverItem's 36-byte body
            # (a thin Ubergraph-dispatch wrapper storing its one parameter).
            frame_slot = r.u64()
            emit(depth, f"[0x{off:04x}] {name} frame_slot=0x{frame_slot:x}")
            emit(depth+1, "assignment:")
            parse_expr(r, depth+2, max_depth)

        elif name == "EX_Let":
            # Plain EX_Let (used for non-specialized property types) DOES
            # serialize a leading FProperty* before the two nested
            # expressions, unlike EX_LetBool — this was never actually
            # exercised by the 33-byte anchor (which only used LetBool), so
            # removing it earlier was an unverified assumption, not a fix.
            ptr = r.u64()
            emit(depth, f"[0x{off:04x}] {name} property=0x{ptr:x}")
            emit(depth+1, "variable:")
            parse_expr(r, depth+2, max_depth)
            emit(depth+1, "assignment:")
            parse_expr(r, depth+2, max_depth)

        elif name in ("EX_ClassContext", "EX_Context", "EX_Context_FailSilent", "EX_InterfaceContext"):
            emit(depth, f"[0x{off:04x}] {name}")
            emit(depth+1, "object_expr:")
            object_op = TOK.get(r.d[r.i])  # peek before consuming
            parse_expr(r, depth+2, max_depth)
            if object_op == "EX_InterfaceContext":
                # No skip+field pair when object_expr is itself an
                # EX_InterfaceContext — empirically confirmed twice against
                # BP_JigMultiplayer_C's Ubergraph (2026-08-10): the skip/field
                # bytes assumed here in the general case turned out to
                # actually be the start of context_expr, both times producing
                # a real, already-resolved function pointer once the
                # skip/field read was removed for this specific nesting.
                # Plausibly because interface dispatch has its own null
                # handling, making the outer null-check skip redundant.
                emit(depth+1, "(no skip/field — object_expr was InterfaceContext)")
            else:
                skip = r.u32()
                fieldptr = r.u64()
                emit(depth+1, f"skip={skip} field=0x{fieldptr:x}")
            emit(depth+1, "context_expr:")
            parse_expr(r, depth+2, max_depth)

        elif name == "EX_StructMemberContext":
            ptr = r.u64()
            emit(depth, f"[0x{off:04x}] {name} member_prop=0x{ptr:x}")
            emit(depth+1, "struct_expr:")
            parse_expr(r, depth+2, max_depth)

        elif name in ("EX_MetaCast", "EX_DynamicCast", "EX_ObjToInterfaceCast",
                      "EX_CrossInterfaceCast", "EX_InterfaceToObjCast"):
            ptr = r.u64()
            emit(depth, f"[0x{off:04x}] {name} class=0x{ptr:x}")
            parse_expr(r, depth+1, max_depth)

        elif name in ("EX_VirtualFunction", "EX_LocalVirtualFunction"):
            # Originally read as an 8-byte FName with a separate "4 extra
            # bytes for the Local variant" hack that happened to produce the
            # right total byte count — turned out to be the wrong explanation.
            # The real FName in this build is 12 bytes (see EX_NameConst
            # above); both variants need the same 12-byte read, no special
            # casing between them.
            ci = r.i32(); disp = r.i32(); num = r.i32()
            emit(depth, f"[0x{off:04x}] {name} FName(ci={ci}, disp={disp}, num={num})")
            n = 0
            while True:
                if TOK.get(r.d[r.i]) == "EX_EndFunctionParms":
                    r.u8()
                    emit(depth+1, "EX_EndFunctionParms")
                    break
                emit(depth+1, f"param[{n}]:")
                parse_expr(r, depth+2, max_depth)
                n += 1
                if n > 64: emit(depth+1, "<too many params, abort>"); break

        elif name in ("EX_FinalFunction", "EX_LocalFinalFunction", "EX_CallMath"):
            # These call by direct UFunction* pointer, no embedded FName at
            # all — the earlier "extra 4 bytes" applied to LocalFinalFunction
            # by analogy with LocalVirtualFunction was never actually
            # exercised/confirmed and is removed; nothing here needs it.
            ptr = r.u64()
            emit(depth, f"[0x{off:04x}] {name} func=0x{ptr:x}")
            n = 0
            while True:
                if TOK.get(r.d[r.i]) == "EX_EndFunctionParms":
                    r.u8()
                    emit(depth+1, "EX_EndFunctionParms")
                    break
                emit(depth+1, f"param[{n}]:")
                parse_expr(r, depth+2, max_depth)
                n += 1
                if n > 64: emit(depth+1, "<too many params, abort>"); break

        elif name == "EX_IntConst":
            v = r.i32(); emit(depth, f"[0x{off:04x}] {name} {v}")
        elif name == "EX_Int64Const":
            v = struct.unpack_from("<q", r.d, r.i)[0]; r.i += 8; emit(depth, f"[0x{off:04x}] {name} {v}")
        elif name == "EX_UInt64Const":
            v = r.u64(); emit(depth, f"[0x{off:04x}] {name} {v}")
        elif name == "EX_FloatConst":
            v = r.f32(); emit(depth, f"[0x{off:04x}] {name} {v}")
        elif name == "EX_DoubleConst":
            v = r.f64(); emit(depth, f"[0x{off:04x}] {name} {v}")
        elif name == "EX_ByteConst" or name == "EX_IntConstByte":
            v = r.u8(); emit(depth, f"[0x{off:04x}] {name} {v}")
        elif name == "EX_SkipOffsetConst":
            v = r.u32(); emit(depth, f"[0x{off:04x}] {name} 0x{v:04x}")
        elif name == "EX_StringConst":
            s = r.cstr(); emit(depth, f"[0x{off:04x}] {name} \"{s}\"")
        elif name == "EX_UnicodeStringConst":
            s = r.wstr(); emit(depth, f"[0x{off:04x}] {name} \"{s}\"")
        elif name == "EX_ObjectConst" or name == "EX_SoftObjectConst":
            ptr = r.u64(); emit(depth, f"[0x{off:04x}] {name} obj=0x{ptr:x}")
        elif name == "EX_NameConst":
            # 12 bytes, not 8 — empirically confirmed (2026-08-10) against a
            # KismetSystemLibrary::PrintString call's Key=NAME_None parameter,
            # where EX_EndFunctionParms landed exactly 4 bytes past where an
            # 8-byte read would have stopped. This build's FName likely has
            # case-preserving-name support (an extra DisplayIndex field).
            # Layout assumed ComparisonIndex, DisplayIndex, Number, all int32 —
            # unconfirmed which of the two non-comparison fields is which, but
            # doesn't matter for resolving common (non-case-modified) names.
            ci = r.i32(); disp = r.i32(); num = r.i32()
            emit(depth, f"[0x{off:04x}] {name} FName(ci={ci}, disp={disp}, num={num})")
        elif name == "EX_RotationConst":
            p,y,rl = r.f64(), r.f64(), r.f64(); emit(depth, f"[0x{off:04x}] {name} ({p},{y},{rl})")
        elif name in ("EX_VectorConst",):
            x,y,z = r.f64(), r.f64(), r.f64(); emit(depth, f"[0x{off:04x}] {name} ({x},{y},{z})")
        elif name == "EX_Vector3fConst":
            x,y,z = r.f32(), r.f32(), r.f32(); emit(depth, f"[0x{off:04x}] {name} ({x},{y},{z})")
        elif name == "EX_TransformConst":
            vals = [r.f64() for _ in range(10)]
            emit(depth, f"[0x{off:04x}] {name} {vals}")
        elif name == "EX_StructConst":
            ptr = r.u64(); size = r.i32()
            emit(depth, f"[0x{off:04x}] {name} struct=0x{ptr:x} size={size}")
            n = 0
            while r.d[r.i] != 0x30:
                emit(depth+1, f"member[{n}]:")
                parse_expr(r, depth+2, max_depth)
                n += 1
                if n > 64: emit(depth+1, "<too many members, abort>"); break
            r.u8(); emit(depth+1, "EX_EndStructConst")
        elif name in ("EX_SetArray",):
            emit(depth, f"[0x{off:04x}] {name}")
            # either an array-inner-property expr precedes elements, or elements start directly
            n = 0
            while r.d[r.i] != 0x32:
                emit(depth+1, f"elem[{n}]:")
                parse_expr(r, depth+2, max_depth)
                n += 1
                if n > 128: emit(depth+1, "<too many elems, abort>"); break
            r.u8(); emit(depth+1, "EX_EndArray")
        elif name == "EX_ArrayConst":
            ptr = r.u64(); count = r.i32()
            emit(depth, f"[0x{off:04x}] {name} inner_prop=0x{ptr:x} count={count}")
            n = 0
            while r.d[r.i] != 0x66:
                emit(depth+1, f"elem[{n}]:")
                parse_expr(r, depth+2, max_depth)
                n += 1
                if n > 256: emit(depth+1, "<too many elems, abort>"); break
            r.u8(); emit(depth+1, "EX_EndArrayConst")
        elif name in ("EX_TextConst",):
            sub = r.u8()
            emit(depth, f"[0x{off:04x}] {name} subtype={sub} <best-effort, may misparse>")
        elif name == "EX_SwitchValue":
            numcases = struct.unpack_from("<H", r.d, r.i)[0]; r.i += 2
            endoff = r.u32()
            emit(depth, f"[0x{off:04x}] {name} numcases={numcases} end=0x{endoff:04x}")
            emit(depth+1, "index_expr:")
            parse_expr(r, depth+2, max_depth)
            for c in range(numcases):
                emit(depth+1, f"case[{c}] value:")
                parse_expr(r, depth+2, max_depth)
                nextoff = r.u32()
                emit(depth+1, f"case[{c}] next=0x{nextoff:04x} result:")
                parse_expr(r, depth+2, max_depth)
            emit(depth+1, "default_result:")
            parse_expr(r, depth+2, max_depth)
        elif name == "EX_ArrayGetByRef":
            emit(depth, f"[0x{off:04x}] {name}")
            emit(depth+1, "array_expr:"); parse_expr(r, depth+2, max_depth)
            emit(depth+1, "index_expr:"); parse_expr(r, depth+2, max_depth)
        elif name == "EX_InstanceDelegate":
            ci = r.i32(); disp = r.i32(); num = r.i32()
            emit(depth, f"[0x{off:04x}] {name} FName(ci={ci},disp={disp},num={num})")
        elif name == "EX_BindDelegate":
            ci = r.i32(); disp = r.i32(); num = r.i32()
            emit(depth, f"[0x{off:04x}] {name} FName(ci={ci},disp={disp},num={num})")
            emit(depth+1, "delegate:"); parse_expr(r, depth+2, max_depth)
            emit(depth+1, "object:"); parse_expr(r, depth+2, max_depth)
        elif name in ("EX_AddMulticastDelegate", "EX_RemoveMulticastDelegate"):
            emit(depth, f"[0x{off:04x}] {name}")
            emit(depth+1, "delegate:"); parse_expr(r, depth+2, max_depth)
            emit(depth+1, "value:"); parse_expr(r, depth+2, max_depth)
        elif name == "EX_ClearMulticastDelegate":
            emit(depth, f"[0x{off:04x}] {name}")
            parse_expr(r, depth+1, max_depth)
        elif name == "EX_CallMulticastDelegate":
            ptr = r.u64()
            emit(depth, f"[0x{off:04x}] {name} func=0x{ptr:x}")
            emit(depth+1, "delegate:"); parse_expr(r, depth+2, max_depth)
            n = 0
            while r.d[r.i] != 0x16:
                emit(depth+1, f"param[{n}]:")
                parse_expr(r, depth+2, max_depth)
                n += 1
                if n > 64: break
            r.u8(); emit(depth+1, "EX_EndFunctionParms")
        elif name == "EX_LocalOutVariable":
            ptr = r.u64()
            emit(depth, f"[0x{off:04x}] {name} prop=0x{ptr:x}")
        elif name == "EX_InstrumentationEvent":
            sub = r.u8()
            emit(depth, f"[0x{off:04x}] {name} subtype={sub}")
        elif name == "EX_FieldPathConst":
            emit(depth, f"[0x{off:04x}] {name}")
            parse_expr(r, depth+1, max_depth)
        elif name == "EX_Cast":
            # legacy numeric conversion: uint8 ConversionType + nested Expression
            convtype = r.u8()
            emit(depth, f"[0x{off:04x}] {name} conv_type={convtype}")
            parse_expr(r, depth+1, max_depth)
        elif name == "EX_BitFieldConst":
            v = r.u8(); emit(depth, f"[0x{off:04x}] {name} {v}")
        else:
            emit(depth, f"[0x{off:04x}] {name} <UNHANDLED OPCODE 0x{op:02x}, stopping decode>")
            raise SystemExit(f"Unhandled/unknown opcode 0x{op:02x} at file offset 0x{off:04x} - stopping to avoid garbage cascade")
    except IndexError:
        emit(depth, f"[0x{off:04x}] {name} <ran off end of buffer parsing operand>")
        raise

def main():
    path = sys.argv[1] if len(sys.argv) > 1 else None
    start = int(sys.argv[2], 0) if len(sys.argv) > 2 else 0
    if not path:
        print("usage: kismet_disasm.py <file.bin> [start_offset]")
        return
    with open(path, "rb") as f:
        data = f.read()
    data = data[start:]
    r = Reader(data)
    print(f"; {path}  ({len(data)} bytes from file offset 0x{start:x})")
    try:
        while not r.eof():
            parse_expr(r, 0)
    except SystemExit as e:
        print(f"; STOPPED: {e}")
    except IndexError:
        print("; STOPPED: ran off end of buffer")
    print("\n".join(out))

if __name__ == "__main__":
    main()

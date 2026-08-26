#!/usr/bin/env python3
"""
Batch disassemble + resolve every .bin dump in a directory, running
entirely locally against THIS machine's live game process (no SSH). Must be
run while the game session that produced the .bin files is still alive —
resolve_ptr/resolve_fprop read raw pointers that are only valid within the
SAME process instance (ASLR), and FName ci/num indices are only meaningful
against that same process's live name table.

For each .bin: disassemble via kismet_disasm.py, collect every FName
ci/num pair and every raw pointer operand (prop=/property=/member_prop=/
inner_prop= go to resolve_fprop; class=/func=/obj=/struct= go to
resolve_ptr). All unique refs across the whole batch are resolved in three
big flag-file round trips (not one per function) to keep this fast, then
each disassembly gets a "Resolved names" appendix.
"""
import os
import re
import subprocess
import sys
import time

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
DISASM = os.path.join(SCRIPT_DIR, "kismet_disasm.py")
FLAG_DIR = os.path.join(os.environ["APPDATA"], "SurrounDeadBridge")
DEBUG_LOG = os.path.join(FLAG_DIR, "debug.log")

FNAME_RE = re.compile(r"FName\(ci=(-?\d+),\s*disp=-?\d+,\s*num=(-?\d+)\)")
PTR_RE = re.compile(r"\b(?:class|func|obj|struct)=0x([0-9a-fA-F]+)")
FPROP_RE = re.compile(r"\b(?:prop|property|member_prop|inner_prop)=0x([0-9a-fA-F]+)")


def disassemble(bin_path):
    r = subprocess.run([sys.executable, DISASM, bin_path], capture_output=True, text=True, timeout=30)
    return r.stdout


def read_new_log_text(since_offset):
    if not os.path.exists(DEBUG_LOG):
        return "", since_offset
    # The game process writes this file continuously; an occasional
    # transient sharing violation (PermissionError) is expected under heavy
    # write load rather than exceptional, so retry briefly instead of
    # aborting the whole batch on one collision.
    for attempt in range(5):
        try:
            with open(DEBUG_LOG, "rb") as f:
                f.seek(since_offset)
                data = f.read()
            new_offset = since_offset + len(data)
            return data.decode("utf-8", errors="replace"), new_offset
        except PermissionError:
            if attempt == 4:
                return "", since_offset
            time.sleep(0.1)


def write_flag(name, content):
    path = os.path.join(FLAG_DIR, name)
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write(content)


def batch_resolve_fname(pairs):
    if not pairs:
        return {}
    start_offset = os.path.getsize(DEBUG_LOG) if os.path.exists(DEBUG_LOG) else 0
    content = "".join(f"{ci} {num}\n" for ci, num in pairs)
    write_flag("resolve_fname.flag", content)
    results = {}
    deadline = time.time() + max(15, len(pairs) * 0.01)
    pat = re.compile(r'resolve_fname: ci=(-?\d+) num=(-?\d+) -> "(.*)"')
    seen = 0
    while time.time() < deadline and seen < len(pairs):
        time.sleep(0.2)
        text, start_offset = read_new_log_text(start_offset)
        for m in pat.finditer(text):
            results[(int(m.group(1)), int(m.group(2)))] = m.group(3)
            seen += 1
    return results


def batch_resolve_ptr(addrs):
    if not addrs:
        return {}
    start_offset = os.path.getsize(DEBUG_LOG) if os.path.exists(DEBUG_LOG) else 0
    content = "".join(f"{a}\n" for a in addrs)
    write_flag("resolve_ptr.flag", content)
    results = {}
    deadline = time.time() + max(15, len(addrs) * 0.01)
    pat = re.compile(r"resolve_ptr: 0x([0-9a-fA-F]+) -> (.*)")
    seen = 0
    while time.time() < deadline and seen < len(addrs):
        time.sleep(0.2)
        text, start_offset = read_new_log_text(start_offset)
        for m in pat.finditer(text):
            results[m.group(1).lower()] = m.group(2).strip()
            seen += 1
    return results


def batch_resolve_fprop(addrs):
    if not addrs:
        return {}
    start_offset = os.path.getsize(DEBUG_LOG) if os.path.exists(DEBUG_LOG) else 0
    content = "".join(f"{a}\n" for a in addrs)
    write_flag("resolve_fprop.flag", content)
    results = {}
    deadline = time.time() + max(15, len(addrs) * 0.01)
    pat = re.compile(r'resolve_fprop: 0x([0-9a-fA-F]+) -> "(.*)"')
    seen = 0
    while time.time() < deadline and seen < len(addrs):
        time.sleep(0.2)
        text, start_offset = read_new_log_text(start_offset)
        for m in pat.finditer(text):
            results[m.group(1).lower()] = m.group(2).strip()
            seen += 1
    return results


def main(src_dir, out_dir):
    os.makedirs(out_dir, exist_ok=True)
    bins = sorted(f for f in os.listdir(src_dir) if f.endswith(".bin"))
    print(f"found {len(bins)} .bin files in {src_dir}")

    disasms = {}
    fname_pairs = set()
    ptr_addrs = set()
    fprop_addrs = set()

    for i, fn in enumerate(bins, 1):
        path = os.path.join(src_dir, fn)
        text = disassemble(path)
        disasms[fn] = text
        for m in FNAME_RE.finditer(text):
            fname_pairs.add((int(m.group(1)), int(m.group(2))))
        for m in PTR_RE.finditer(text):
            ptr_addrs.add(m.group(1).lower())
        for m in FPROP_RE.finditer(text):
            fprop_addrs.add(m.group(1).lower())
        if i % 100 == 0:
            print(f"  disassembled {i}/{len(bins)}")

    print(f"disassembly done. unique refs: {len(fname_pairs)} FNames, {len(ptr_addrs)} ptrs, {len(fprop_addrs)} fprops")

    print("resolving FNames...")
    fname_map = batch_resolve_fname(sorted(fname_pairs))
    print(f"  resolved {len(fname_map)}/{len(fname_pairs)}")

    print("resolving pointers...")
    ptr_map = batch_resolve_ptr(sorted(ptr_addrs))
    print(f"  resolved {len(ptr_map)}/{len(ptr_addrs)}")

    print("resolving fprops...")
    fprop_map = batch_resolve_fprop(sorted(fprop_addrs))
    print(f"  resolved {len(fprop_map)}/{len(fprop_addrs)}")

    for fn, text in disasms.items():
        out_path = os.path.join(out_dir, fn.replace(".bin", ".decoded.txt"))
        with open(out_path, "w", encoding="utf-8") as f:
            f.write(text)
            f.write("\n\n; ---- Resolved names ----\n")
            local_fnames = sorted({(int(m.group(1)), int(m.group(2))) for m in FNAME_RE.finditer(text)})
            for ci, num in local_fnames:
                name = fname_map.get((ci, num), "<unresolved>")
                f.write(f'; FName(ci={ci}, num={num}) -> "{name}"\n')
            local_ptrs = sorted({m.group(1).lower() for m in PTR_RE.finditer(text)})
            for a in local_ptrs:
                f.write(f"; ptr=0x{a} -> {ptr_map.get(a, '<unresolved>')}\n")
            local_fprops = sorted({m.group(1).lower() for m in FPROP_RE.finditer(text)})
            for a in local_fprops:
                f.write(f'; fprop=0x{a} -> "{fprop_map.get(a, "<unresolved>")}"\n')

    print(f"wrote {len(disasms)} decoded files to {out_dir}")


if __name__ == "__main__":
    src = sys.argv[1] if len(sys.argv) > 1 else r"C:\temp\GIT\SDO\research\bytecode\pc1_decode_out"
    dst = sys.argv[2] if len(sys.argv) > 2 else r"C:\temp\GIT\SDO\research\bytecode\pc1_decoded"
    main(src, dst)

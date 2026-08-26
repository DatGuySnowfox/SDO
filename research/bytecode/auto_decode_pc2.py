#!/usr/bin/env python3
"""
Automated, unattended bytecode decode pipeline against PC2 (sdo-client2).

For each (class_name, function_name) target:
  1. Write a local flag file, scp it to a plain (no-spaces) temp path on PC2,
     then move it into place at %APPDATA%\\SurrounDeadBridge\\bytecode_dump.flag
     (sidesteps bash->ssh->powershell quoting hell for names with spaces,
     e.g. "Update Equipped Item Durability" — a real function name in this
     game's own header dump).
  2. Poll PC2's debug.log (via ssh) for the "wrote N bytes to X.bin" line.
  3. scp the resulting .bin down to a local results directory.
  4. Run kismet_disasm.py on it locally.
  5. Extract every "ci=NNNN num=N" reference from the disassembly, batch them
     into a resolve_fname.flag (same write/scp/move pattern), poll for
     results, and produce a final annotated .txt with resolved names appended.
  6. Log progress to stdout and move to the next target — errors on one
     target don't stop the run, they're recorded and skipped.

Designed to run for a long time unattended (potentially hours across
hundreds of targets) — this is why it's a script and not a per-function
agent call: the dump/copy/disassemble/resolve loop is entirely mechanical.
"""
import subprocess
import time
import re
import os
import sys

# 2026-08-17: plain "bash" from Python's subprocess resolves to WSL's bash
# (via the Windows bash.exe redirector at C:\Windows\System32\bash.exe), NOT
# Git-Bash — a completely different environment (WSL's own HOME, its own
# /mnt/c/... mount convention, no access to the Windows-side ~/.ssh/config
# that defines the sdo-client2 alias). Must invoke Git-Bash's real bash.exe
# by its full path to get the same environment the Bash tool itself uses.
GIT_BASH = r"C:\Program Files\Git\bin\bash.exe"

REMOTE_HOST = "sdo-client2"
REMOTE_APPDATA_FLAG_DIR = r"$env:APPDATA\SurrounDeadBridge"
LOCAL_TMP = r"C:\Users\mccau\AppData\Local\Temp\claude\C--Users-mccau\7b5f3f2f-bcdb-4d8e-afbd-151c82c05958\scratchpad"
LOCAL_OUT_DIR = r"C:\temp\GIT\SDO\research\bytecode\playercharacter_decoded"
DISASM_SCRIPT = r"C:\temp\GIT\SDO\research\bytecode\kismet_disasm.py"
DEBUG_LOG_REMOTE = r"$env:APPDATA\SurrounDeadBridge\debug.log"

os.makedirs(LOCAL_OUT_DIR, exist_ok=True)
os.makedirs(LOCAL_TMP, exist_ok=True)


def run(cmd, timeout=30):
    """Run a command through Git-Bash (see GIT_BASH's own comment for why
    the explicit path matters), return (returncode, stdout, stderr)."""
    p = subprocess.run([GIT_BASH, "-c", cmd], capture_output=True, text=True, timeout=timeout)
    return p.returncode, p.stdout, p.stderr


def ssh_ps(ps_command, timeout=30):
    """Run a PowerShell command on PC2 via ssh, wrapped safely."""
    # Write the PS command to a local file and scp it up, then execute it via
    # -File instead of -Command, to avoid ANY inline quoting of ps_command
    # across bash -> ssh -> powershell (the exact trap that caused repeated
    # friction earlier this session).
    local_ps1 = os.path.join(LOCAL_TMP, "remote_cmd.ps1")
    with open(local_ps1, "w", encoding="utf-8") as f:
        f.write(ps_command)
    rc, out, err = run(f'scp "{local_ps1.replace(chr(92), "/")}" {REMOTE_HOST}:C:/sdo_remote_cmd.ps1', timeout=15)
    if rc != 0:
        return rc, out, err
    return run(f'ssh {REMOTE_HOST} "powershell -NoProfile -ExecutionPolicy Bypass -File C:/sdo_remote_cmd.ps1"', timeout=timeout)


def write_remote_flag(flag_name, content):
    """Write `content` to a plain local file, scp to a no-spaces temp path on
    PC2, then move it into the real flag directory."""
    local_flag = os.path.join(LOCAL_TMP, flag_name)
    with open(local_flag, "w", encoding="utf-8", newline="\n") as f:
        f.write(content)
    rc, out, err = run(f'scp "{local_flag.replace(chr(92), "/")}" {REMOTE_HOST}:C:/sdo_{flag_name}', timeout=15)
    if rc != 0:
        return False, f"scp failed: {err}"
    rc, out, err = ssh_ps(
        f'Move-Item -Force -Path "C:\\sdo_{flag_name}" -Destination "{REMOTE_APPDATA_FLAG_DIR}\\{flag_name}"'
    )
    if rc != 0:
        return False, f"remote move failed: {err}"
    return True, ""


def get_remote_log_line_count():
    rc, out, err = ssh_ps(f'(Get-Content -Path "{DEBUG_LOG_REMOTE}").Count')
    if rc != 0:
        return None
    try:
        return int(out.strip().splitlines()[-1])
    except (ValueError, IndexError):
        return None


def get_remote_log_tail(n):
    rc, out, err = ssh_ps(f'Get-Content -Tail {n} -Path "{DEBUG_LOG_REMOTE}"')
    if rc != 0:
        return ""
    return out


def dump_function(class_name, func_name):
    """Trigger a bytecode dump for class_name::func_name, wait for it, scp
    the .bin down. Returns (local_bin_path_or_None, message)."""
    ok, msg = write_remote_flag("bytecode_dump.flag", f"{class_name}\n{func_name}\n")
    if not ok:
        return None, msg

    safe_func = re.sub(r"[^A-Za-z0-9_]", "_", func_name)
    bin_name = f"{class_name}_{safe_func}.bin"
    remote_bin = f"{REMOTE_APPDATA_FLAG_DIR}\\{bin_name}"

    # Each poll iteration is itself a ~1.5-2s ssh round trip, on top of the
    # two round trips write_remote_flag() already spent (scp + move) — a
    # short deadline here blows through before ever catching the completion
    # line, confirmed live 2026-08-17 (the dump itself completed in ~100ms
    # server-side every time; the timeout was purely local polling overhead).
    deadline = time.time() + 25
    wrote_line = None
    while time.time() < deadline:
        tail = get_remote_log_tail(30)
        if f"wrote " in tail and bin_name in tail:
            for line in tail.splitlines():
                if "wrote " in line and bin_name in line:
                    wrote_line = line
            if wrote_line:
                break
        if "NOT FOUND" in tail or "not found" in tail:
            for line in tail.splitlines():
                if (class_name in line or func_name in line) and ("NOT FOUND" in line or "not found" in line):
                    return None, f"function not found on live class: {line.strip()}"

    if not wrote_line:
        return None, "timed out waiting for dump confirmation"

    local_bin = os.path.join(LOCAL_OUT_DIR, bin_name)
    rc, out, err = run(f'scp {REMOTE_HOST}:"{remote_bin}" "{local_bin.replace(chr(92), "/")}"', timeout=15)
    if rc != 0:
        return None, f"scp of .bin failed: {err}"
    return local_bin, wrote_line.strip()


def resolve_cis(ci_num_pairs):
    """Batch-resolve a list of (ci, num) via resolve_fname.flag. Returns
    dict[(ci,num)] -> name string."""
    if not ci_num_pairs:
        return {}
    content = "".join(f"{ci} {num}\n" for ci, num in ci_num_pairs)
    before_count = get_remote_log_line_count()
    ok, msg = write_remote_flag("resolve_fname.flag", content)
    if not ok:
        print(f"    resolve_fname write failed: {msg}")
        return {}

    time.sleep(min(1.0 + len(ci_num_pairs) * 0.05, 6.0))
    tail = get_remote_log_tail(max(50, len(ci_num_pairs) * 2 + 10))
    results = {}
    for line in tail.splitlines():
        m = re.search(r'resolve_fname: ci=(-?\d+) num=(-?\d+) -> "(.*)"', line)
        if m:
            results[(int(m.group(1)), int(m.group(2)))] = m.group(3)
    return results


def disassemble(local_bin):
    rc, out, err = run(f'python "{DISASM_SCRIPT.replace(chr(92), "/")}" "{local_bin.replace(chr(92), "/")}"', timeout=30)
    return out if rc == 0 else f"[disasm failed: {err}]"


def process_target(class_name, func_name, index, total):
    print(f"[{index}/{total}] {class_name}::{func_name} ...", flush=True)
    local_bin, msg = dump_function(class_name, func_name)
    if not local_bin:
        print(f"    SKIP: {msg}", flush=True)
        return "skip", msg

    disasm = disassemble(local_bin)

    ci_pairs = set()
    for m in re.finditer(r"ci=(-?\d+), disp=\d+, num=(-?\d+)\)", disasm):
        ci_pairs.add((int(m.group(1)), int(m.group(2))))
    for m in re.finditer(r"FName\(ci=(-?\d+), num=(-?\d+)", disasm):
        ci_pairs.add((int(m.group(1)), int(m.group(2))))

    resolved = resolve_cis(list(ci_pairs))

    out_path = os.path.join(LOCAL_OUT_DIR, f"{class_name}_{func_name}.decoded.txt")
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(f"; {class_name}::{func_name}\n; dump: {msg}\n\n")
        f.write(disasm)
        if resolved:
            f.write("\n\n; ---- Resolved names ----\n")
            for (ci, num), name in sorted(resolved.items()):
                f.write(f"; ci={ci} num={num} -> \"{name}\"\n")

    print(f"    OK -> {out_path} ({len(resolved)} names resolved)", flush=True)
    return "ok", out_path


def main(targets_file):
    with open(targets_file, "r", encoding="utf-8") as f:
        targets = []
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            cls, func = line.split("\t")
            targets.append((cls, func))

    total = len(targets)
    results = {"ok": 0, "skip": 0}
    log_path = os.path.join(LOCAL_OUT_DIR, "_run_log.txt")
    with open(log_path, "a", encoding="utf-8") as logf:
        logf.write(f"\n=== run started, {total} targets ===\n")
        for i, (cls, func) in enumerate(targets, 1):
            try:
                status, detail = process_target(cls, func, i, total)
            except Exception as e:
                status, detail = "skip", f"exception: {e}"
                print(f"    SKIP (exception): {e}", flush=True)
            results[status] = results.get(status, 0) + 1
            logf.write(f"[{i}/{total}] {cls}::{func} -> {status}: {detail}\n")
            logf.flush()

    print(f"\nDone. {results.get('ok',0)} decoded, {results.get('skip',0)} skipped. Log: {log_path}", flush=True)


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else r"C:\temp\GIT\SDO\research\bytecode\targets_playercharacter.txt")

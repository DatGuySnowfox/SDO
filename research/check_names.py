#!/usr/bin/env python3
"""Check every name-based lookup in src/ against the 5.6 header dump.

Why this exists: `GetValuePtrByPropertyNameInChain` returns null for a name that
does not exist, and the callers all skip on null. A renamed property therefore
produces no crash, no log line and no compiler error - the write simply stops
happening. The UE 5.3 -> 5.6 port shipped with eleven of these, including the
whole crouch/ADS/falling/aim path, because the player AnimBP was renamed
`Player_AnimBP_C` -> `AnimBP_PlayerCharacter` and its variables with it. See
research/04_ida_investigation_log.md, Session 61.

Run it after any engine upgrade. "It compiles and does not crash" says nothing
about whether these lookups still resolve.

    python research/check_names.py            # names with no owner at all
    python research/check_names.py --all      # every name, with owning classes

Two caveats on the output:

  * A name existing on *some* class is not the same as existing on the class it
    is used on. That is why the owner column is printed next to the receiver
    variable - reading it is the point, and it is what caught
    GetSkeletalMeshComponent (a property on ASkeletalMeshActor, never a getter).
  * The dump only contains classes that were loaded when it was taken, so a
    main-menu-only class shows as missing while being perfectly fine. Absence is
    a prompt to look, not a verdict.
"""
import argparse
import collections
import io
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HDR = os.path.join(ROOT, 'research', 'CXXHeaderDump')
SRC = os.path.join(ROOT, 'src')

# Header lines look like one of:
#     FGameplayTag ActiveWeapon;                    // 0x0BB0 (size: 0x8)
#     void GetActiveWeapon(class AActor*& Actor);
PROP_RE = re.compile(r'^\s{4}(.+?)\s+([A-Za-z_][A-Za-z0-9_?& ]*?)(\[\d+\])?;\s*//\s*0x')
FUNC_RE = re.compile(r'^\s{4}(?:[\w:<>*&\s]+?)\s([A-Za-z_][A-Za-z0-9_? ]*)\s*\(')
CLS_RE = re.compile(r'^(?:class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)')

# Built without backslash escapes in a raw string on purpose: this file gets
# edited through shell heredocs often enough that doubled backslashes are a
# recurring source of silently broken patterns.
Q = chr(34)
BS = chr(92)
LIT = ('(?:STR' + BS + '(' + BS + 's*' + Q + '([^' + Q + ']*)' + Q + BS + 's*' + BS + ')'
       '|L' + Q + '([^' + Q + ']*)' + Q + ')')

LOOKUPS = [
    ('prop', 'GetValuePtrByPropertyNameInChain' + BS + '(' + BS + 's*' + LIT),
    ('func', 'GetFunctionByNameInChain' + BS + '(' + BS + 's*' + LIT),
    ('prop', BS + 'bprop_obj' + BS + 's*' + BS + '([^,]+,' + BS + 's*' + LIT),
    ('prop', BS + 'bprop_ptr' + BS + 's*<[^>]+>' + BS + 's*' + BS + '([^,]+,' + BS + 's*' + LIT),
    ('prop', BS + 'bobj_prop' + BS + 's*' + BS + '([^,]+,' + BS + 's*' + LIT),
]
LOOKUPS = [(kind, re.compile(pat)) for kind, pat in LOOKUPS]

RECV = re.compile(r'([A-Za-z_][A-Za-z0-9_\.>-]*?)\s*->\s*Get(?:ValuePtrByPropertyName|FunctionByName)InChain')
RECV_FN = re.compile(r'\b(?:prop_obj|obj_prop|prop_ptr\s*<[^>]+>)\s*\(\s*([^,]+),')


def build_index():
    """name -> sorted list of declaring classes, for properties and functions."""
    props = collections.defaultdict(set)
    funcs = collections.defaultdict(set)
    files = 0
    for fn in sorted(os.listdir(HDR)):
        if not fn.endswith('.hpp'):
            continue
        files += 1
        current = fn[:-4]
        for line in io.open(os.path.join(HDR, fn), encoding='utf-8', errors='replace'):
            m = CLS_RE.match(line)
            if m:
                current = m.group(1)
                continue
            m = PROP_RE.match(line)
            if m:
                props[m.group(2).strip()].add(current)
                continue
            m = FUNC_RE.match(line)
            if m:
                funcs[m.group(1).strip()].add(current)
    return files, props, funcs


def collect_lookups():
    """(kind, name) -> {receiver variable: [file:line, ...]}"""
    hits = collections.defaultdict(lambda: collections.defaultdict(list))
    for fn in sorted(os.listdir(SRC)):
        if not (fn.endswith('.cpp') or fn.endswith('.hpp')):
            continue
        path = os.path.join(SRC, fn)
        for num, line in enumerate(io.open(path, encoding='utf-8', errors='replace'), 1):
            if line.lstrip().startswith('//'):
                continue
            for kind, rx in LOOKUPS:
                for m in rx.finditer(line):
                    name = m.group(1) or m.group(2)
                    if not name:
                        continue
                    r = RECV.search(line) or RECV_FN.search(line)
                    recv = (r.group(1).strip() if r else '?')[:26]
                    hits[(kind, name)][recv].append('%s:%d' % (fn, num))
    return hits


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--all', action='store_true',
                    help='print every name with its owning classes, not only the unresolved ones')
    args = ap.parse_args()

    files, props, funcs = build_index()
    hits = collect_lookups()
    print('indexed %d headers: %d property names, %d function names'
          % (files, len(props), len(funcs)))
    print('found %d distinct name lookups in src/' % len(hits))
    print()

    missing = 0
    print('%-5s %-34s %-26s %s' % ('kind', 'name', 'receiver(s)', 'declared on'))
    print('-' * 118)
    for kind, name in sorted(hits):
        table = props if kind == 'prop' else funcs
        owners = sorted(table.get(name, []))
        if not owners:
            missing += 1
            shown = '*** no owner in the 5.6 dump ***'
        elif not args.all:
            continue
        else:
            shown = ','.join(owners[:3])
            if len(owners) > 3:
                shown += ' +%d' % (len(owners) - 3)
        recvs = ','.join(sorted(hits[(kind, name)]))[:26]
        print('%-5s %-34s %-26s %s' % (kind, name, recvs, shown))

    print()
    print('%d name(s) with no owner in the dump' % missing)
    return 1 if missing else 0


if __name__ == '__main__':
    raise SystemExit(main())

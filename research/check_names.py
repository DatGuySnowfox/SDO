#!/usr/bin/env python3
"""Check the names this mod hands to the engine against the 5.6 header dump.

Why this exists: `GetValuePtrByPropertyNameInChain` returns null for a name that
does not exist, and every caller skips on null. A renamed property therefore
produces no crash, no log line and no compiler error - the write simply stops
happening. The UE 5.3 -> 5.6 port shipped with eleven of these, including the
whole crouch/ADS/falling/aim path, because the player AnimBP was renamed
`Player_AnimBP_C` -> `AnimBP_PlayerCharacter` and its variables with it. See
research/04_ida_investigation_log.md, Session 61.

Run it after any engine upgrade. "It compiles and does not crash" says nothing
about whether these lookups still resolve.

    python research/check_names.py              # both passes, problems only
    python research/check_names.py --all        # every name, with owning classes
    python research/check_names.py --pass sites # call sites only
    python research/check_names.py --pass lits  # wide literals only

Two passes, because either alone misses things:

  sites  Every literal passed directly to a name-based lookup. Precise: it knows
         whether a name is wanted as a property or a function, and prints the
         receiver variable so a name that exists on the wrong class is visible.
         Blind to any name that reaches the lookup through a variable.

  lits   Every wide-string literal in the source, classified against everything
         the dump knows. Catches the ones `sites` cannot see - a table of names
         iterated by a loop, for instance, which is exactly how the stale
         `AnimGraphNode_Fabrik_6` survived the first audit - at the cost of
         flagging strings that were never meant to be engine names. Read the
         unknowns and judge; most are obvious.

Caveats that apply to both:

  * A name existing on *some* class is not the same as existing on the class it
    is used on. That is why the owner column is printed next to the receiver -
    it is what caught `GetSkeletalMeshComponent`, a property on
    ASkeletalMeshActor that was being looked up as a getter.
  * The dump only holds classes that were loaded when it was taken, so a
    main-menu-only class shows as missing while being perfectly fine. Absence is
    a prompt to look, not a verdict.
"""
import argparse
import collections
import io
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HDR = os.path.join(ROOT, 'research', 'CXXHeaderDump')
SRC = os.path.join(ROOT, 'src')
TAGS = os.path.join(ROOT, 'research', 'Exports', 'SurrounDead', 'Content',
                    'JigSInventory', 'Jigsaw', 'DT_JigTags.json')

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

ANY_LIT = re.compile(LIT)
RECV = re.compile(r'([A-Za-z_][A-Za-z0-9_.>-]*?)\s*->\s*Get(?:ValuePtrByPropertyName|FunctionByName)InChain')
RECV_FN = re.compile(r'\b(?:prop_obj|obj_prop|prop_ptr\s*<[^>]+>)\s*\(\s*([^,]+),')

# Wide literals that are legitimately not engine names. Output::send's format
# strings are wide too, and they always carry one of the markers below; none of
# them is ever a name the engine resolves.
IGNORE_EXACT = {'', ' ', '.', '\\', '/', ') +', 'SDO'}
IGNORE_SUBSTR = ('\\\\', '.flag', '.log', '.bin', '.txt', '.cfg', '.json', '.dll',
                 '.prev', 'SDO_', 'APPDATA', 'Default__',
                 'SDO: ', '{:', '{}', '\\n', ' = ', ': ', ' - ')


def build_index():
    props = collections.defaultdict(set)
    funcs = collections.defaultdict(set)
    classes = set()
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
                classes.add(current)
                continue
            m = PROP_RE.match(line)
            if m:
                props[m.group(2).strip()].add(current)
                continue
            m = FUNC_RE.match(line)
            if m:
                funcs[m.group(1).strip()].add(current)
    return files, props, funcs, classes


def load_tags():
    tags = set()
    if os.path.exists(TAGS):
        text = io.open(TAGS, encoding='utf-8', errors='replace').read()
        tags = set(re.findall(r'"((?:Jig|Icons)\.[A-Za-z0-9_.]+)"', text))
    return tags


def iter_source():
    for fn in sorted(os.listdir(SRC)):
        if fn.endswith('.cpp') or fn.endswith('.hpp'):
            path = os.path.join(SRC, fn)
            for num, line in enumerate(io.open(path, encoding='utf-8', errors='replace'), 1):
                yield fn, num, line


def pass_sites(props, funcs, show_all):
    hits = collections.defaultdict(lambda: collections.defaultdict(list))
    for fn, num, line in iter_source():
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

    print('== pass 1: call sites ==   %d distinct name lookups' % len(hits))
    print('%-5s %-34s %-26s %s' % ('kind', 'name', 'receiver(s)', 'declared on'))
    print('-' * 116)
    missing = 0
    for kind, name in sorted(hits):
        table = props if kind == 'prop' else funcs
        owners = sorted(table.get(name, []))
        if not owners:
            missing += 1
            shown = '*** no owner in the 5.6 dump ***'
        elif not show_all:
            continue
        else:
            shown = ','.join(owners[:3]) + (' +%d' % (len(owners) - 3) if len(owners) > 3 else '')
        recvs = ','.join(sorted(hits[(kind, name)]))[:26]
        print('%-5s %-34s %-26s %s' % (kind, name, recvs, shown))
    print('-> %d unresolved' % missing)
    return missing


def pass_literals(props, funcs, classes, tags, show_all):
    lits = collections.defaultdict(list)
    for fn, num, line in iter_source():
        if line.lstrip().startswith('//'):
            continue
        for m in ANY_LIT.finditer(line):
            name = m.group(1) or m.group(2)
            if name is None:
                continue
            if name in IGNORE_EXACT or any(t in name for t in IGNORE_SUBSTR):
                continue
            lits[name].append('%s:%d' % (fn, num))

    def classify(name):
        if name in props:
            return 'property'
        if name in funcs:
            return 'function'
        if name in classes or ('U' + name) in classes or ('A' + name) in classes \
                or (name + '_C') in classes or ('U' + name + '_C') in classes \
                or ('A' + name + '_C') in classes:
            return 'class'
        if name in tags:
            return 'tag'
        if '.' in name or '/' in name:
            return 'path/tag?'
        return 'UNKNOWN'

    print()
    print('== pass 2: every wide literal ==   %d distinct' % len(lits))
    print('%-46s %-11s %s' % ('literal', 'kind', 'first sites'))
    print('-' * 116)
    unknown = 0
    for name in sorted(lits):
        kind = classify(name)
        if kind == 'UNKNOWN':
            unknown += 1
        elif not show_all:
            continue
        print('%-46s %-11s %s' % (name[:46], kind, ','.join(lits[name][:3])))
    print('-> %d unclassified (read these; not all are bugs)' % unknown)
    return unknown


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--all', action='store_true', help='show every name, not only problems')
    ap.add_argument('--pass', dest='which', choices=('sites', 'lits', 'both'), default='both')
    args = ap.parse_args()

    files, props, funcs, classes = build_index()
    tags = load_tags()
    print('indexed %d headers: %d properties, %d functions, %d classes, %d tags'
          % (files, len(props), len(funcs), len(classes), len(tags)))
    print()

    bad = 0
    if args.which in ('sites', 'both'):
        bad += pass_sites(props, funcs, args.all)
    if args.which in ('lits', 'both'):
        bad += pass_literals(props, funcs, classes, tags, args.all)
    return 1 if bad else 0


if __name__ == '__main__':
    raise SystemExit(main())

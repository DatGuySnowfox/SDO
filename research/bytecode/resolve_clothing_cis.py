#!/usr/bin/env python3
"""Resolve the handful of FName ComparisonIndex values the clothing decode turns on.

The full pipeline resolves names per target, but re-running three targets just to
resolve six numbers is wasteful and needs the dumps redone. This asks the live
client for exactly the values that matter.

Requires PC2 to be in game: resolve_fname.flag is consumed by the mod's own tick.

  1696234  called from MC_AttachClothing with (Parts, IsPlayerMale?, BodyPart,
           UpdateAllBodyParts?) - expected to be BodyPartVisibility
   137933  called on the clothing component with (Mesh, False), and 14 times
           inside UpdateBodyParts - expected to be SetSkinnedAssetAndUpdate
   127923  10 occurrences in UpdateBodyParts, unidentified
  1534050  EX_NameConst in UpdateBodyParts - expected to be a body part name
  1534053  likewise
  1534056  likewise
  1716933  called on an instance variable inside the MC_AttachClothing body
  1710744  six-argument call in the same region
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import auto_decode_pc2 as pipe

TARGETS = [1704958, 1572105, 1715440, 1571986, 1572021, 1574440, 1716825, 1534053, 1534056, 137933, 1710744]

def main():
    pairs = [(ci, 0) for ci in TARGETS]
    print("asking the live client to resolve %d names ..." % len(pairs), flush=True)
    results = pipe.resolve_cis(pairs)
    if not results:
        print("nothing resolved. Is PC2 in game? The flag is consumed by the mod's tick.")
        return 1
    print()
    for ci in TARGETS:
        name = results.get((ci, 0))
        print("  ci=%-9d %s" % (ci, name if name else "<unresolved>"))
    return 0

if __name__ == "__main__":
    raise SystemExit(main())

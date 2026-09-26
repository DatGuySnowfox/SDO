#!/usr/bin/env python3
"""Read, and optionally rewrite, the vitals inside player_progress.data.

The blob is a fixed-layout binary, big-endian, per the byte map in
server/src/lib/protocol.js (kept in lockstep with src/protocol.cpp):

    0       uint8   format version (1)
    1 - 4   uint32  revision
    5 - 8   float32 health
    9 - 12  float32 hunger
    13 - 16 float32 thirst
    17 - 20 float32 stamina
    21 - 24 float32 radiation

Only those five floats are touched. Everything after byte 24 - level, xp,
position, the whole container list, the extended stats trailer - is copied
through untouched, so inventory and position cannot be disturbed by this.

Run with no --write to print current values and change nothing.
"""
import sqlite3, struct, sys, argparse

FIELDS = [("health", 5), ("hunger", 9), ("thirst", 13),
          ("stamina", 17), ("radiation", 21)]


def read_vitals(blob):
    if len(blob) < 25:
        return None, "payload too short (%d bytes)" % len(blob)
    ver = blob[0]
    rev = struct.unpack_from(">I", blob, 1)[0]
    vals = {name: struct.unpack_from(">f", blob, off)[0] for name, off in FIELDS}
    return (ver, rev, vals), None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("db")
    ap.add_argument("--write", action="store_true",
                    help="actually apply the new values")
    ap.add_argument("--health", type=float)
    ap.add_argument("--hunger", type=float)
    ap.add_argument("--thirst", type=float)
    ap.add_argument("--stamina", type=float)
    ap.add_argument("--radiation", type=float)
    ap.add_argument("--player", action="append", default=[],
                    help="restrict to these playerIds (repeatable); default is all rows")
    a = ap.parse_args()

    con = sqlite3.connect(a.db)
    rows = con.execute(
        "SELECT playerId, revision, savedAt, data FROM player_progress").fetchall()
    print("player_progress rows: %d\n" % len(rows))

    new = {n: getattr(a, n) for n, _ in FIELDS if getattr(a, n) is not None}

    for pid, rev, savedAt, blob in rows:
        if a.player and pid not in a.player:
            print("player %s  SKIPPED (not selected)" % pid)
            continue
        parsed, err = read_vitals(blob)
        if err:
            print("%s: %s" % (pid, err))
            continue
        ver, brev, vals = parsed
        print("player %s  rev=%d (blob rev=%d) bytes=%d ver=%d" %
              (pid, rev, brev, len(blob), ver))
        for n, _ in FIELDS:
            mark = "  -> %.3f" % new[n] if n in new else ""
            print("    %-10s %.3f%s" % (n, vals[n], mark))

        if a.write and new:
            b = bytearray(blob)
            for n, off in FIELDS:
                if n in new:
                    struct.pack_into(">f", b, off, new[n])
            con.execute("UPDATE player_progress SET data=? WHERE playerId=?",
                        (bytes(b), pid))
            after, _ = read_vitals(bytes(b))
            print("    written, reads back: " +
                  ", ".join("%s=%.3f" % (n, after[2][n]) for n, _ in FIELDS))
        print()

    if a.write and new:
        con.commit()
        print("committed")
    else:
        print("dry run, nothing written")
    con.close()


if __name__ == "__main__":
    main()

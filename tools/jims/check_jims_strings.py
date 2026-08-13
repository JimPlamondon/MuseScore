#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
# MuseScore-Studio-CLA-applies
"""JiMS string-table verifier (Milestone 1, owner Q22 answer 2026-08-13).

Every user-visible JiMS name must live only in
src/engraving/jims/jimsstrings.h so a future trademark rename is one-file
work. This check fails when any of the tracked names appears as a quoted
literal elsewhere in the fork's src/ tree. Exit 0 = clean.
"""
import os
import re
import sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
TABLE = os.path.join(ROOT, "src", "engraving", "jims", "jimsstrings.h")
NAMES = ["JiMStaff", "JiMS staff", "crescent clef", "scale dots", "tonic indicator"]

failures = []
for dirpath, dirnames, filenames in os.walk(os.path.join(ROOT, "src")):
    dirnames[:] = [d for d in dirnames if d not in (".git", "thirdparty")]
    for name in filenames:
        if not name.endswith((".cpp", ".h", ".mm", ".qml", ".ui")):
            continue
        path = os.path.join(dirpath, name)
        if os.path.abspath(path) == os.path.abspath(TABLE):
            continue
        try:
            text = open(path, encoding="utf-8", errors="ignore").read()
        except OSError:
            continue
        for jims_name in NAMES:
            for match in re.finditer(rf'"[^"\n]*{re.escape(jims_name)}[^"\n]*"', text):
                line = text.count("\n", 0, match.start()) + 1
                failures.append(f"{os.path.relpath(path, ROOT)}:{line}: literal {jims_name!r} outside the string table")

if failures:
    print("\n".join(failures))
    print(f"{len(failures)} JiMS string-table violation(s)")
    sys.exit(1)
print("JiMS string table check passed")

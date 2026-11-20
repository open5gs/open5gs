#!/bin/sh
#
# Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
#
# This file is part of Open5GS.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# The JSON CDR mirrors the BER CDR: same record, same field names.
#
# This is the test that can actually say so, because it compares the two
# through something that shares no code with either.  chf-cdr-emit writes
# both encodings of the same sessions; open5gs-chf-cdr-decode, generated
# from the TS 32.298 module rather than from the CHF's C, decodes the
# .ber; and the two JSON documents must then be equal.
#
# Usage (meson passes all three):
#   test-cdr-decode.sh <chf-cdr-emit> <open5gs-chf-cdr-decode> <python3>

set -e

EMIT=${1:?emitter binary required}
DECODE=${2:?decoder script required}
PYTHON=${3:-python3}

WORK=$(mktemp -d 2>/dev/null || mktemp -d -t chf-cdr)
trap 'rm -rf "$WORK"' EXIT

echo "# emitting CDRs into $WORK"
"$EMIT" "$WORK" > "$WORK/paths.txt"

json_count=0
fail=0

while read -r kind path; do
    [ "$kind" = "json" ] || continue
    json_count=$((json_count + 1))

    ber=$(echo "$path" | sed 's/\.json$/.ber/')

    if [ ! -f "$path" ]; then
        echo "FAIL: missing JSON CDR $path"
        fail=1
        continue
    fi
    if [ ! -f "$ber" ]; then
        echo "FAIL: missing BER CDR $ber"
        fail=1
        continue
    fi

    # Decode the BER and compare the two as parsed documents, so key
    # order and int-vs-float do not count as differences.
    if "$PYTHON" - "$path" "$ber" "$DECODE" "$PYTHON" <<'PYEOF'
import json
import subprocess
import sys

json_path, ber_path, decode, python = sys.argv[1:5]

with open(json_path) as f:
    from_json = json.loads(f.read().strip())

out = subprocess.run([python, decode, ber_path],
                     capture_output=True, text=True)
if out.returncode != 0:
    sys.stderr.write("decoder failed: %s\n" % out.stderr)
    sys.exit(1)

lines = [ln for ln in out.stdout.strip().split("\n") if ln]
if len(lines) != 1:
    sys.stderr.write("expected 1 decoded record, got %d\n" % len(lines))
    sys.exit(1)

from_ber = json.loads(lines[0])

if from_json != from_ber:
    sys.stderr.write("JSON and BER renderings differ\n")
    a = json.dumps(from_json, indent=2, sort_keys=True).split("\n")
    b = json.dumps(from_ber, indent=2, sort_keys=True).split("\n")
    import difflib
    for d in difflib.unified_diff(a, b, "from .json", "from .ber",
                                  lineterm=""):
        sys.stderr.write(d + "\n")
    sys.exit(1)
PYEOF
    then
        echo "ok: $(basename "$path") matches $(basename "$ber")"
    else
        echo "FAIL: $(basename "$path") does not match $(basename "$ber")"
        fail=1
    fi
done < "$WORK/paths.txt"

if [ "$json_count" -eq 0 ]; then
    echo "FAIL: the emitter wrote no CDRs"
    exit 1
fi

# An aggregated BER file holds many records back to back; the decoder has
# to read every one of them, since that is what makes aggregation usable.
cat_all=$WORK/all.ber
: > "$cat_all"
while read -r kind path; do
    [ "$kind" = "ber" ] && cat "$path" >> "$cat_all"
done < "$WORK/paths.txt"

got=$("$PYTHON" "$DECODE" --count "$cat_all" | sed 's/.*: //')
if [ "$got" != "$json_count" ]; then
    echo "FAIL: concatenated BER holds $got records, expected $json_count"
    fail=1
else
    echo "ok: $json_count concatenated BER records all decode"
fi

if [ "$fail" -ne 0 ]; then
    exit 1
fi

echo "PASS: $json_count CDRs, JSON and BER agree"

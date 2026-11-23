#!/bin/bash
# =============================================================================
# test-dedicated-flow.sh - traffic on a dedicated QoS flow reaches the CDR
#
# UERANSIM cannot test this.  The UPF ignores a dedicated flow's downlink PDR
# until the gNB answers the N2 modify, and UERANSIM's gNB does not, so its traffic
# stays on the default flow whatever the PCC rules say.  Upstream's vonr
# qos-flow-test simulates its own gNB and does answer.  The harness starts a CHF,
# each session sends 2 pings on the default flow and 2 on the dedicated one.  They
# are 28 bytes each, so the CDR must read 112 each way.  56 means the dedicated
# flow went uncounted.
#
# Usage, as root from the build directory, with MongoDB running:
#   bash ../tests/chf/test-dedicated-flow.sh
#
# Like every upstream test it inserts and removes its own subscriber.  Do not point
# it at a database holding IMSI 999700000021309.
# =============================================================================
set -u

WORK=$(mktemp -d)
trap 'rm -rf "${WORK}"' EXIT
mkdir -p "${WORK}/cdr"

# The harness starts the CHF from the test config, which keeps no CDRs.  This
# test reads them, so it runs on a copy that writes them under ${WORK}.
python3 - configs/vonr.yaml "${WORK}/vonr.yaml" "${WORK}/cdr" <<'PY'
import re, sys

source, target, cdr = sys.argv[1:]
text, count = re.subn(
    r"(?m)^(chf:\n(?:[ #].*\n)*?    cdr:\n)      enable: false\n",
    lambda m: m.group(1) +
    "      enable: true\n      path: %s\n      format: json\n" % cdr,
    open(source).read())
if count != 1:
    sys.exit("FAIL: no chf section to change in " + source)
open(target, "w").write(text)
PY
[ -s "${WORK}/vonr.yaml" ] || exit 1

ip link set ogstun up 2>/dev/null

./tests/vonr/vonr -c "${WORK}/vonr.yaml" qos-flow-test > "${WORK}/test.out" 2>&1
TEST_RC=$?

if [ "${TEST_RC}" != "0" ]; then
    echo "FAIL: vonr qos-flow-test exited ${TEST_RC}"
    exit 1
fi

python3 - "${WORK}/cdr" <<'PY'
import json, os, sys

def find(obj, key):
    if isinstance(obj, dict):
        if key in obj:
            return obj[key]
        obj = list(obj.values())
    if isinstance(obj, list):
        for value in obj:
            found = find(value, key)
            if found is not None:
                return found
    return None

volumes = []
for root, _, files in os.walk(sys.argv[1]):
    for name in files:
        if name.endswith(".json"):
            cdr = json.load(open(os.path.join(root, name)))
            volumes.append((find(cdr, "dataVolumeUplink"),
                            find(cdr, "dataVolumeDownlink")))

if len(volumes) != 2:
    print("FAIL: %d CDRs, expected 2: the CHF may not have registered in time"
          % len(volumes))
    sys.exit(1)

for ul, dl in volumes:
    if (ul, dl) != (112, 112):
        print("FAIL: CDR reads ul=%s dl=%s, expected 112 each way" % (ul, dl))
        sys.exit(1)

print("PASS: 2 CDRs, each with the dedicated flow's traffic (ul=112 dl=112)")
PY

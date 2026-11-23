#!/bin/bash
# =============================================================================
# test-shared-quota.sh - pools: one bucket that several SIMs draw on
#
# A pool is a chf.subscribers document with a members array of SUPIs and no
# supi of its own.  This drives a real CHF against a database of its own, with
# the documents written directly as the WebUI would write them, and checks what
# the CHF does with them:
#
#   1. it publishes charging.pool.max_members in chf.settings
#   2. members draw on the pool and share its counter; a SIM past the limit,
#      and one outside the pool, get the default quota
#   3. at equal priority a SIM's own bucket is spent before its pool
#   4. a member taken out of the pool is ended at its next report, and what
#      it reported is still counted to the pool
#   5. a SIM added to a pool while its session is up on the default quota is
#      moved onto the pool at its next report
#   6. a SIM can be in one pool only: the unique index the CHF creates
#
# Usage, from the build directory:
#   bash ../tests/chf/test-shared-quota.sh
#
# Needs: MongoDB at DB_URI_BASE (default mongodb://127.0.0.1:27017), curl with
# HTTP/2, python3, and node with the WebUI's npm install, whose MongoDB driver
# does the database steps.  It creates, and drops, a database named
# chf_pool_test_<pid>.  Ports on 127.0.0.1: NRF_PORT (17793), CHF_PORT (17794).
# No root.  KEEP_WORK=1 keeps the logs.
# =============================================================================
set -u

REPO=$(cd "$(dirname "$0")/../.." && pwd)
DB_URI_BASE=${DB_URI_BASE:-mongodb://127.0.0.1:27017}
DB_URI="${DB_URI_BASE}/chf_pool_test_$$"
NRF_PORT=${NRF_PORT:-17793}
CHF_PORT=${CHF_PORT:-17794}
MONGODB_MODULE=${MONGODB_MODULE:-${REPO}/webui/node_modules/mongodb}
MB=1048576
A=imsi-999700000091001; B=imsi-999700000091002; C=imsi-999700000091003
D=imsi-999700000091004; E=imsi-999700000091005
POOL=pool-0123456789ab

PASS=0; FAIL=0
pass() { echo "PASS $1"; PASS=$((PASS+1)); }
fail() { echo "FAIL $1"; FAIL=$((FAIL+1)); }
check() { if [ "$2" = "$3" ]; then pass "$1"; else fail "$1 (got [$2], expected [$3])"; fi; }

WORK=$(mktemp -d)
PIDS=""
cleanup() {
    for p in ${PIDS}; do kill -TERM "${p}" 2>/dev/null; done
    sleep 1
    for p in ${PIDS}; do kill -KILL "${p}" 2>/dev/null; done
    [ -f "${WORK}/db.js" ] && node "${WORK}/db.js" drop >/dev/null 2>&1
    if [ -n "${KEEP_WORK:-}" ]; then echo "logs kept in ${WORK}"; else rm -rf "${WORK}"; fi
}
trap cleanup EXIT

for b in ./src/nrf/open5gs-nrfd ./src/chf/open5gs-chfd; do
    if [ ! -x "${b}" ]; then
        echo "SKIP: ${b} not found; run this from the build directory"
        exit 77
    fi
done
if ! command -v node >/dev/null 2>&1 || [ ! -d "${MONGODB_MODULE}" ]; then
    echo "SKIP: needs node and ${MONGODB_MODULE} (npm install in webui/)"
    exit 77
fi

# -----------------------------------------------------------------------------
# The database steps, through the WebUI's MongoDB driver.
# -----------------------------------------------------------------------------
cat > "${WORK}/db.js" <<'JS'
'use strict';
const { MongoClient } = require(process.env.MONGODB_MODULE);
const [op, ...args] = process.argv.slice(2);
const plan = mb => ({ totalQuotaBytes: mb * 1048576, validityPeriod: 'monthly',
  validitySeconds: 2592000, autoRenew: true, finalUnitAction: 'TERMINATE' });
(async () => {
  const client = new MongoClient(process.env.DB_URI,
      { serverSelectionTimeoutMS: 5000 });
  await client.connect();
  const db = client.db();
  const subs = db.collection('chf.subscribers');
  switch (op) {
  case 'ping':
    await db.command({ ping: 1 });
    console.log('ok');
    break;
  case 'pool':               // id mb member...
    await subs.insertOne({ bucketId: args[0], members: args.slice(2),
      priority: 0, quota: plan(Number(args[1])) });
    console.log('ok');
    break;
  case 'own':                // supi mb priority
    await subs.insertOne({ supi: args[0], bucketId: args[0],
      priority: Number(args[2]), quota: plan(Number(args[1])) });
    console.log('ok');
    break;
  case 'remove':             // bucketId
    await subs.deleteOne({ bucketId: args[0] });
    console.log('ok');
    break;
  case 'members':            // id member...
    await subs.updateOne({ bucketId: args[0] },
        { $set: { members: args.slice(1) } });
    console.log('ok');
    break;
  case 'used': {             // id -> consumed MB
    const d = await subs.findOne({ bucketId: args[0] });
    console.log(d ? ((d.usage || {}).consumedBytes || 0) / 1048576 : 'gone');
    break;
  }
  case 'setting': {
    const d = await db.collection('chf.settings').findOne({ _id: 'pool' });
    console.log(d ? d.maxMembers : 'none');
    break;
  }
  case 'try-pool': {         // id member...: 'ok' or the error code
    try {
      await subs.insertOne({ bucketId: args[0], members: args.slice(1),
        quota: plan(10) });
      console.log('ok');
    } catch (e) { console.log(e.code); }
    break;
  }
  case 'drop':
    await db.dropDatabase();
    console.log('ok');
    break;
  default:
    throw new Error(`unknown op ${op}`);
  }
  await client.close();
})().catch(e => { console.error(e.message); process.exit(1); });
JS
export MONGODB_MODULE DB_URI
DB() { node "${WORK}/db.js" "$@"; }

if [ "$(DB ping 2>/dev/null)" != "ok" ]; then
    echo "SKIP: no MongoDB at ${DB_URI_BASE}"
    exit 77
fi

# -----------------------------------------------------------------------------
# An NRF and a CHF that allows 3 members per pool
# -----------------------------------------------------------------------------
cat > "${WORK}/nrf.yaml" <<EOF
logger:
  file:
    path: ${WORK}/nrf.log
global:
  max:
    ue: 64
nrf:
  sbi:
    server:
      - address: 127.0.0.1
        port: ${NRF_PORT}
EOF
cat > "${WORK}/chf.yaml" <<EOF
db_uri: ${DB_URI}
logger:
  file:
    path: ${WORK}/chf.log
global:
  max:
    ue: 64
chf:
  sbi:
    server:
      - address: 127.0.0.1
        port: ${CHF_PORT}
    client:
      nrf:
        - uri: http://127.0.0.1:${NRF_PORT}
  charging:
    cdr:
      enable: false
    notify:
      enable: false
    pool:
      max_members: 3
EOF

./src/nrf/open5gs-nrfd -c "${WORK}/nrf.yaml" > "${WORK}/nrf.out" 2>&1 &
PIDS="${PIDS} $!"
sleep 1
./src/chf/open5gs-chfd -c "${WORK}/chf.yaml" > "${WORK}/chf.out" 2>&1 &
CHF_PID=$!
PIDS="${PIDS} ${CHF_PID}"
for i in $(seq 1 20); do
    grep -aq "CHF DB: collections ready" "${WORK}/chf.log" 2>/dev/null && break
    sleep 0.5
done
sleep 1

URL="http://127.0.0.1:${CHF_PORT}/nchf-convergedcharging/v3/chargingdata"
SEQ=0
# request <url> <supi> <used bytes or -> : leaves the body in ${WORK}/b
request() {
    SEQ=$((SEQ+1))
    local used=""
    [ "$3" != "-" ] && used="\"usedUnitContainer\":[{\"localSequenceNumber\":${SEQ},\"uplinkVolume\":0,\"downlinkVolume\":$3,\"time\":1}],"
    curl -s --max-time 5 --http2-prior-knowledge -D "${WORK}/h" -o "${WORK}/b" \
        -w '%{http_code}' -H 'Content-Type: application/json' -d "{
          \"subscriberIdentifier\": \"$2\",
          \"nfConsumerIdentification\": {\"nFName\": \"test\", \"nodeFunctionality\": \"SMF\"},
          \"invocationTimeStamp\": \"$(date -u +%Y-%m-%dT%H:%M:%SZ)\",
          \"invocationSequenceNumber\": ${SEQ},
          \"pDUSessionChargingInformation\": {\"chargingId\": ${SEQ}, \"userInformation\": {},
            \"pduSessionInformation\": {\"networkSlicingInfo\": {\"sNSSAI\": {\"sst\": 1}},
              \"pduSessionID\": 5, \"dnnId\": \"internet\", \"chargingCharacteristics\": \"0800\"}},
          \"multipleUnitUsage\": [{\"ratingGroup\": 1, ${used}\"requestedUnit\": {\"totalVolume\": 1000000}}]
        }" "$1"
}
create() {     # supi -> prints the ChargingDataRef
    request "${URL}" "$1" - >/dev/null
    grep -i '^location:' "${WORK}/h" | tr -d '\r' | sed -E 's#.*/chargingdata/##'
}
update() { request "${URL}/$1/update" "$2" "$3"; }
# What the last answer said: "<granted bytes> <final unit action or ->"
answer() {
    python3 - "${WORK}/b" <<'PY'
import json, sys
try:
    d = json.load(open(sys.argv[1]))
except Exception:
    print("unparseable"); raise SystemExit
mui = (d.get("multipleUnitInformation") or [{}])[0]
g = (mui.get("grantedUnit") or {}).get("totalVolume")
f = (mui.get("finalUnitIndication") or {}).get("finalUnitAction", "-")
print(g, f)
PY
}
logged() { grep -ac "$1" "${WORK}/chf.log"; }

# -----------------------------------------------------------------------------
# 1. The limit is published where the WebUI reads it
# -----------------------------------------------------------------------------
check "1. charging.pool.max_members is published in chf.settings" "$(DB setting)" 3

# -----------------------------------------------------------------------------
# 2. Members share the pool; past the limit, or outside it, the default quota
# -----------------------------------------------------------------------------
DB pool "${POOL}" 100 "${A}" "${B}" "${C}" "${D}" >/dev/null
REF_A=$(create "${A}")
check "2. a member's session is charged to the pool" "$(logged "\[${A}\] Pool \[${POOL}\] charged")" 1
update "${REF_A}" "${A}" $((10*MB)) >/dev/null
REF_B=$(create "${B}")
check "2. a second member's session is charged to the same pool" "$(logged "\[${B}\] Pool \[${POOL}\] charged")" 1
update "${REF_B}" "${B}" $((5*MB)) >/dev/null
check "2. both members' usage lands on the pool's one counter" "$(DB used "${POOL}")" 15
REF_D=$(create "${D}")
check "2. the fourth member, past max_members 3, is not charged to the pool" \
    "$(logged "\[${D}\] Member 4 of pool \[${POOL}\], past charging.pool.max_members")" 1
check "2. ... and gets the default quota" "$(logged "\[${D}\] Not provisioned in chf.subscribers")" 1
REF_E=$(create "${E}")
check "2. a SIM outside the pool gets the default quota" "$(logged "\[${E}\] Not provisioned in chf.subscribers")" 1

# -----------------------------------------------------------------------------
# 3. A SIM's own bucket before its pool, at equal priority
# -----------------------------------------------------------------------------
DB own "${C}" 20 0 >/dev/null
REF_C=$(create "${C}")
check "3. at equal priority the SIM's own bucket is charged, not the pool" \
    "$(logged "\[${C}\] Pool \[${POOL}\] charged")" 0
update "${REF_C}" "${C}" $((2*MB)) >/dev/null
check "3. ... and its usage goes to its own bucket" "$(DB used "${C}"):$(DB used "${POOL}")" "2:15"
DB remove "${C}" >/dev/null

# -----------------------------------------------------------------------------
# 4. A member taken out is ended at its next report
# -----------------------------------------------------------------------------
DB members "${POOL}" "${A}" "${C}" "${E}" >/dev/null
update "${REF_B}" "${B}" $((3*MB)) >/dev/null
check "4. the removed member's next report is answered with a terminate final unit" \
    "$(answer)" "0 TERMINATE"
check "4. the CHF says why" "$(logged "\[${B}\] No longer a member of pool \[${POOL}\]")" 1
check "4. what it reported was granted from the pool, and is counted to it" "$(DB used "${POOL}")" 18
update "${REF_A}" "${A}" $((1*MB)) >/dev/null
check "4. a member that stayed carries on, granted from the pool" "$(answer | cut -d' ' -f2)" "-"

# -----------------------------------------------------------------------------
# 5. Added while up on the default quota: moved onto the pool
# -----------------------------------------------------------------------------
update "${REF_E}" "${E}" $((1*MB)) >/dev/null
check "5. a SIM added to the pool moves its live session onto it" \
    "$(logged "\[${E}\] Joined pool \[${POOL}\] mid-session")" 1
update "${REF_E}" "${E}" $((4*MB)) >/dev/null
check "5. ... and its later usage is counted to the pool" "$(DB used "${POOL}")" 23

# -----------------------------------------------------------------------------
# 6. One pool per SIM
# -----------------------------------------------------------------------------
check "6. a SIM already in a pool cannot be put in a second one" \
    "$(DB try-pool pool-second00000 "${A}" "${D}")" 11000
check "6. a SIM in no pool can" "$(DB try-pool pool-second00000 "${D}")" ok

kill -0 "${CHF_PID}" 2>/dev/null && pass "the CHF is still running" || fail "the CHF stopped"
errs=$(grep -ac "ERROR\|FATAL" "${WORK}/chf.log")
[ "${errs}" = "0" ] && pass "no errors in the CHF's log" \
    || { fail "${errs} error lines"; grep -a "ERROR\|FATAL" "${WORK}/chf.log" | head -5; }

echo
echo "Results: PASS=${PASS}  FAIL=${FAIL}"
[ "${FAIL}" = "0" ]

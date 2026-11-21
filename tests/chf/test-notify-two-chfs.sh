#!/bin/bash
# =============================================================================
# test-notify-two-chfs.sh - two CHFs sharing one ChargingNotify queue
#
# A top-up, a plan change or a deprovision through the TMF API leaves a row in
# chf.notifications, and the CHF turns it into a ChargingNotify to the live
# sessions it holds.  Several CHFs can share one database - one per slice, or
# per SUPI range - and a row does not say which of them holds the sessions.
# Every CHF has to see every row, once.  Until 2026-11-14 the first CHF to poll
# deleted every row it read, and the sessions on the other CHF never heard.
#
# Two CHFs and an NRF run on loopback against a database of their own.  The
# SUPI imsi-...1 has a session on each CHF, as a subscriber with sessions on
# two slices would, and imsi-...2 has one on the second CHF.  The sessions'
# notifyUri points at a port nobody listens on: what is checked is which
# sessions each CHF sent a notification to, not what an SMF did with it.
#
# Usage, from the build directory:
#   bash ../tests/chf/test-notify-two-chfs.sh
#
# Needs: MongoDB at DB_URI_BASE (default mongodb://127.0.0.1:27017), curl with
# HTTP/2, and node with the WebUI's npm install, whose MongoDB driver does the
# database steps.  It creates, and drops, a database named
# chf_notify_test_<pid>, so it can run beside a core using the same MongoDB.
# The ports are on 127.0.0.1 and can be moved with NRF_PORT, CHF_A_PORT and
# CHF_B_PORT.  It needs no root.  KEEP_WORK=1 keeps the logs.
# =============================================================================
set -u

REPO=$(cd "$(dirname "$0")/../.." && pwd)
DB_URI_BASE=${DB_URI_BASE:-mongodb://127.0.0.1:27017}
DB_NAME="chf_notify_test_$$"
DB_URI="${DB_URI_BASE}/${DB_NAME}"
NRF_PORT=${NRF_PORT:-17790}
CHF_A_PORT=${CHF_A_PORT:-17791}
CHF_B_PORT=${CHF_B_PORT:-17792}
DEAD_PORT=${DEAD_PORT:-17799}           # the notifyUri; nothing listens here
MONGODB_MODULE=${MONGODB_MODULE:-${REPO}/webui/node_modules/mongodb}
SUPI_X="imsi-999700000090001"
SUPI_Y="imsi-999700000090002"

PASS=0; FAIL=0
pass() { echo "PASS $1"; PASS=$((PASS+1)); }
fail() { echo "FAIL $1"; FAIL=$((FAIL+1)); }

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
const { MongoClient, ObjectId } = require(process.env.MONGODB_MODULE);
const [op, ...args] = process.argv.slice(2);
const key = id => (/^[0-9a-f]{24}$/.test(id) ? new ObjectId(id) : id);
(async () => {
  const client = new MongoClient(process.env.DB_URI,
      { serverSelectionTimeoutMS: 5000 });
  await client.connect();
  const q = client.db().collection('chf.notifications');
  switch (op) {
  case 'ping':
    await client.db().command({ ping: 1 });
    console.log('ok');
    break;
  case 'insert': {           // supi type [bucketId]
    const row = { id: new ObjectId().toHexString(), supi: args[0],
      type: args[1], reason: 'test', createdAt: new Date() };
    if (args[2]) row.bucketId = args[2];
    console.log((await q.insertOne(row)).insertedId.toHexString());
    break;
  }
  case 'insert-raw':         // a malformed row, as JSON
    await q.insertOne(JSON.parse(args[0]));
    console.log('ok');
    break;
  case 'row': {              // id -> "gone", or how many saw it
    const r = await q.findOne({ _id: key(args[0]) });
    console.log(!r ? 'gone' : `seenBy=${(r.seenBy || []).sort().join(',')} ` +
        `seenAt=${r.seenAt instanceof Date ? 'set' : 'unset'}`);
    break;
  }
  case 'age':                // id seconds: make it look first seen that long ago
    await q.updateOne({ _id: key(args[0]) },
        { $set: { seenAt: new Date(Date.now() - Number(args[1]) * 1000) } });
    console.log('ok');
    break;
  case 'drop':
    await client.db().dropDatabase();
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
# An NRF and two CHFs
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

chf_yaml() {   # name port
    cat > "${WORK}/chf-$1.yaml" <<EOF
db_uri: ${DB_URI}
logger:
  file:
    path: ${WORK}/chf-$1.log
global:
  max:
    ue: 64
chf:
  sbi:
    server:
      - address: 127.0.0.1
        port: $2
    client:
      nrf:
        - uri: http://127.0.0.1:${NRF_PORT}
  charging:
    cdr:
      enable: false
    notify:
      enable: true
      poll_seconds: 1
EOF
}
chf_yaml a "${CHF_A_PORT}"
chf_yaml b "${CHF_B_PORT}"

./src/nrf/open5gs-nrfd -c "${WORK}/nrf.yaml" > "${WORK}/nrf.out" 2>&1 &
PIDS="${PIDS} $!"
sleep 1

start_chf() {  # name -> sets CHF_<NAME>_PID and CHF_<NAME>_ID
    local name=$1 log="${WORK}/chf-$1.log" pid id i from line upper
    # A restart appends to the same log: look only at what this run writes.
    from=$(( $(cat "${log}" 2>/dev/null | wc -l) + 1 ))
    ./src/chf/open5gs-chfd -c "${WORK}/chf-${name}.yaml" \
        > "${WORK}/chf-${name}.out" 2>&1 &
    pid=$!
    PIDS="${PIDS} ${pid}"
    line=""
    for i in $(seq 1 20); do
        line=$(tail -n +"${from}" "${log}" 2>/dev/null \
            | grep -a "ChargingNotify: polling" | tail -1)
        [ -n "${line}" ] && break
        sleep 0.5
    done
    sleep 0.5    # the SBI server opens just after the poll is set up
    # The name it reads the queue under.  A CHF from before 2026-11-14 has
    # none, and then only the checks on names and marks fail.
    id=$(printf '%s\n' "${line}" | sed -nE 's/.* as \[([0-9a-f-]+)\].*/\1/p')
    upper=$(echo "${name}" | tr a-z A-Z)
    printf -v "CHF_${upper}_PID" '%s' "${pid}"
    printf -v "CHF_${upper}_ID" '%s' "${id}"
    printf -v "CHF_${upper}_UP" '%s' "${line:+yes}"
}
start_chf a
start_chf b
if [ -z "${CHF_A_UP}" ] || [ -z "${CHF_B_UP}" ]; then
    echo "FAIL: a CHF did not start its notification poll"
    tail -5 "${WORK}/chf-a.out" "${WORK}/chf-b.out"
    exit 1
fi
[ -n "${CHF_A_ID}" ] && [ "${CHF_A_ID}" != "${CHF_B_ID}" ] \
    && pass "the two CHFs read the queue under different names" \
    || fail "the two CHFs read the queue under different names ([${CHF_A_ID}] [${CHF_B_ID}])"

SEQ=0
create() {     # port supi -> prints the ChargingDataRef, or nothing
    SEQ=$((SEQ+1))
    curl -s --max-time 5 --http2-prior-knowledge -D "${WORK}/h" -o /dev/null \
        -H 'Content-Type: application/json' -d "{
          \"subscriberIdentifier\": \"$2\",
          \"nfConsumerIdentification\": {\"nFName\": \"test\", \"nodeFunctionality\": \"SMF\"},
          \"invocationTimeStamp\": \"$(date -u +%Y-%m-%dT%H:%M:%SZ)\",
          \"invocationSequenceNumber\": ${SEQ},
          \"notifyUri\": \"http://127.0.0.1:${DEAD_PORT}/notify\"
        }" "http://127.0.0.1:$1/nchf-convergedcharging/v3/chargingdata"
    grep -i '^location:' "${WORK}/h" | tr -d '\r' | sed -E 's#.*/chargingdata/##'
}

# How many notifications of a kind one CHF sent to one session.
sent() {       # name kind ref
    grep -ac "ChargingNotify $2 sent \[ref:$3 " "${WORK}/chf-$1.log"
}
expect_sent() { # name kind ref count what
    local n
    n=$(sent "$1" "$2" "$3")
    [ "${n}" = "$4" ] && pass "$5" || fail "$5 (sent ${n}, expected $4)"
}
seen_by_both() {
    local want
    want=$(printf '%s\n' "${CHF_A_ID}" "${CHF_B_ID}" | sort | paste -sd, -)
    [ "$(DB row "$1")" = "seenBy=${want} seenAt=set" ]
}
wait_polls() { sleep 3; }   # poll_seconds is 1

# -----------------------------------------------------------------------------
# 1. Each CHF reaches its own sessions
# -----------------------------------------------------------------------------
REF_AX=$(create "${CHF_A_PORT}" "${SUPI_X}")
REF_BX=$(create "${CHF_B_PORT}" "${SUPI_X}")
REF_BY=$(create "${CHF_B_PORT}" "${SUPI_Y}")
if [ -z "${REF_AX}" ] || [ -z "${REF_BX}" ] || [ -z "${REF_BY}" ]; then
    echo "FAIL: a Create was refused"
    exit 1
fi

ROW_X=$(DB insert "${SUPI_X}" REAUTHORIZATION)
ROW_Y=$(DB insert "${SUPI_Y}" REAUTHORIZATION)
wait_polls

expect_sent a REAUTHORIZATION "${REF_AX}" 1 "1. the first CHF reaches the SUPI's session it holds"
expect_sent b REAUTHORIZATION "${REF_BX}" 1 "1. the second CHF reaches the same SUPI's session it holds"
expect_sent b REAUTHORIZATION "${REF_BY}" 1 "1. the second CHF reaches the other SUPI's session"
seen_by_both "${ROW_X}" && pass "1. both CHFs marked both rows, and the rows are still there" \
    || fail "1. rows after one poll each: $(DB row "${ROW_X}"); $(DB row "${ROW_Y}")"

# -----------------------------------------------------------------------------
# 2. ... once
# -----------------------------------------------------------------------------
wait_polls
expect_sent a REAUTHORIZATION "${REF_AX}" 1 "2. no CHF delivers a row twice"
expect_sent b REAUTHORIZATION "${REF_BX}" 1 "2. no CHF delivers a row twice, on the second CHF either"

# -----------------------------------------------------------------------------
# 3. A restarted CHF does not act on rows queued before it started
# -----------------------------------------------------------------------------
kill -TERM "${CHF_B_PID}"; wait "${CHF_B_PID}" 2>/dev/null
OLD_B_ID=${CHF_B_ID}
ROW_OLD=$(DB insert "${SUPI_Y}" ABORT_CHARGING)
sleep 2
start_chf b
[ -n "${CHF_B_ID}" ] && [ "${CHF_B_ID}" != "${OLD_B_ID}" ] \
    && pass "3. the restarted CHF reads under a new name" \
    || fail "3. the restarted CHF reads under its old name, or none"
REF_BY2=$(create "${CHF_B_PORT}" "${SUPI_Y}")
if [ -z "${REF_BY2}" ]; then
    echo "FAIL: the restarted CHF refused a Create"
    exit 1
fi
wait_polls
expect_sent b ABORT_CHARGING "${REF_BY2}" 0 "3. a session set up after the restart is not aborted by a row from before it"
case "$(DB row "${ROW_OLD}")" in
    *"${CHF_B_ID}"*) pass "3. the restarted CHF marked that row seen at start-up" ;;
    *) fail "3. the restarted CHF did not mark the old row: $(DB row "${ROW_OLD}")" ;;
esac
ROW_NEW=$(DB insert "${SUPI_Y}" REAUTHORIZATION)
wait_polls
expect_sent b REAUTHORIZATION "${REF_BY2}" 1 "3. a row queued after the restart reaches it"

# -----------------------------------------------------------------------------
# 4. Rows go once they are old enough, and not before
# -----------------------------------------------------------------------------
DB age "${ROW_X}" 3600 >/dev/null
wait_polls
[ "$(DB row "${ROW_X}")" = "gone" ] && pass "4. a row first seen an hour ago is deleted" \
    || fail "4. an hour-old row is still there: $(DB row "${ROW_X}")"
[ "$(DB row "${ROW_NEW}")" != "gone" ] && pass "4. a row first seen seconds ago is kept" \
    || fail "4. a fresh row was deleted"

# -----------------------------------------------------------------------------
# 5. A malformed row is read once, not on every poll
# -----------------------------------------------------------------------------
DB insert-raw '{"_id":"by-hand-1","type":"REAUTHORIZATION"}' >/dev/null
wait_polls; wait_polls
n_a=$(grep -ac "row with no supi" "${WORK}/chf-a.log")
n_b=$(grep -ac "row with no supi" "${WORK}/chf-b.log")
[ "${n_a}" = "1" ] && [ "${n_b}" = "1" ] \
    && pass "5. a row with no supi and a string _id is logged once by each CHF" \
    || fail "5. a malformed row was logged ${n_a} and ${n_b} times"
seen_by_both "by-hand-1" && pass "5. and is marked, so it is aged out like any other" \
    || fail "5. the malformed row: $(DB row by-hand-1)"

# Both still running, and nothing worse than the expected warnings logged.
for name in a b; do
    pid_var="CHF_$(echo "${name}" | tr a-z A-Z)_PID"
    kill -0 "${!pid_var}" 2>/dev/null && pass "the ${name} CHF is still running" \
        || fail "the ${name} CHF stopped"
done
errs=$(cat "${WORK}"/chf-*.log | grep -a "ERROR\|FATAL" \
    | grep -avc "row with no supi")
[ "${errs}" = "0" ] && pass "no other errors in either CHF's log" \
    || { fail "${errs} other error lines"; grep -a "ERROR\|FATAL" "${WORK}"/chf-*.log \
         | grep -av "row with no supi" | head -5; }

echo
echo "Results: PASS=${PASS}  FAIL=${FAIL}"
[ "${FAIL}" = "0" ]

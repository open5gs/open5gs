# S10 inter-MME mobility

S10 connects MMEs for mobility and context transfer. The protocol layer includes
GTPv2-C mobility messages, the EPS MM Context codec and transaction stages. The
MME runtime implements Identification, inter-MME TAU and connected S1 handover
with a retained SGW. It includes fresh AKA fallback for idle mobility, direct
and indirect forwarding, PDCP status transfer, cancellation and SON configuration
transfer. The remaining integration work is listed below.

## Standards baseline

- [TS 23.401 V19.4.0](https://www.etsi.org/deliver/etsi_ts/123400_123499/123401/19.04.00_60/ts_123401v190400p.pdf):
  architecture (4.2.3), Attach (5.3.2.1), TAU with MME change (5.3.3.1),
  S1 handover with MME relocation (5.5.1.2.2).
- [TS 29.274 V19.6.0](https://www.etsi.org/deliver/etsi_ts/129200_129299/129274/19.06.00_60/ts_129274v190600p.pdf):
  message numbers (6.1), mobility messages (7.3), MM Context (8.38), PDN
  Connection (8.39), Complete Request Message (8.46), GUTI (8.47), containers
  (8.48) and target identification (8.51). This is the Word input already
  versioned under `lib/gtp/v2/support/r19.6.0/`.
- [TS 33.401 V19.2.0](https://www.etsi.org/deliver/etsi_ts/133400_133499/133401/19.02.00_60/ts_133401v190200p.pdf):
  authentication-vector handling (6.1), EPS security contexts (6.4), concurrent
  security procedures (7.2.10), idle mobility (7.2.7) and handover security (7.2.8).
- [TS 23.007 V19.2.0](https://www.etsi.org/deliver/etsi_ts/123000_123099/123007/19.02.00_60/ts_123007v190200p.pdf):
  MME restoration (14), GTP restart handling (18) and network-triggered service
  restoration (25).

## Protocol implementation

The generator includes the following TS 29.274 mobility messages:

| Procedure | Message types | TS 29.274 clauses |
| --- | --- | --- |
| Identification | 128 / 129 | 7.3.8 / 7.3.9 |
| Context transfer | 130 / 131 / 132 | 7.3.5–7.3.7 |
| Forward relocation | 133 / 134 | 7.3.1 / 7.3.2 |
| Relocation completion | 135 / 136 | 7.3.3 / 7.3.4 |
| Access-context forwarding | 137 / 138 | 7.3.10 / 7.3.11 |
| Relocation cancellation | 139 / 140 | 7.3.16 / 7.3.17 |
| Configuration transfer | 141 | 7.3.18 |

PDN Connection groups use `OGS_MAX_NUM_OF_SESS` entries. Nested bearer groups,
relocation bearer lists and Context Acknowledge bearer lists use
`OGS_BEARER_PER_UE` entries. The structure arrays and TLV repetition descriptors
are generated together. Distinct IE instances with the same display name receive
distinct C member names.

`ogs_gtp2_parse_eps_mm_context()` and `ogs_gtp2_build_eps_mm_context()` implement
security mode 4 of Figure 8.38-5. They preserve KASME, NAS counts, algorithms,
KSI, unused EPS vectors, DRX, NH/NCC, subscribed/used AMBR, capabilities,
equipment identity, access restrictions and the old security context. The codec
checks all lengths before consuming data and assigns the decoded result only
after a complete successful parse. Its octet-string fields borrow the input
buffer. Callers retain that buffer until they have copied or consumed the views.

The extension tail follows the release-ordered length fields and preserves
future extension octets. Bitrates in this structure are wire values in kbit/s;
NAS counts are 24-bit values. The receiving MME still applies its algorithm,
identity, access-control and security-context policy when importing a context.

Context Request/Response/Acknowledge uses a three-leg transaction. The ACK is
matched against the old MME's remote transaction list. Other request/response
pairs use the two-leg transaction path. Configuration Transfer Tunnel is a
one-way message, forwarded between configured MMEs without creating a
request/response transaction.

## MME integration checklist

The intended EPS scope covers both idle mobility and connected-mode S1 handover.
Each procedure needs source and target behavior:

- [x] Configure S10 peer endpoints and GUMMEI routes, sharing transactions for
  routes to the same endpoint. Dispatch only provisioned peers on GTP-C.
- [x] Attach Identification Request/Response for a foreign native GUTI, complete
  NAS request validation, conditional security-context export and fresh AKA.
- [x] TAU with a retained SGW: transfer MM and PDN/bearer contexts, validate the
  complete request, acknowledge, update S11 ownership and HSS registration,
  and allocate/confirm the target GUTI. Authentication fallback retries the
  request with IMSI and the standard MS Validated indication.
- [x] Route connected-mode handover by target TAI/eNB identity.
- [x] Retain the old context while transfer is pending. Handle acknowledgements,
  HSS cancellation, retransmissions and expiry without deleting gateway sessions
  that now belong to the target MME.
- [x] Connected handover with a retained SGW: exchange Forward Relocation, allocate target radio
  resources, forward status containers, complete relocation and release source
  resources. Cover cancellation, target rejection and expiry paths.
- [ ] Handle SGW relocation and indirect forwarding when the selected SGW changes.
- [ ] Preserve NAS replay protection, key derivation and NH/NCC progression across
  mobility and concurrent security procedures.
- [ ] Exercise two-MME Attach/TAU and handover with several APNs and dedicated
  bearers, checking user-plane traffic and cleanup in both directions.

S10 transfer depends on the source context being available. Continuous session
replication, database replication and host failover are separate components.
They should not be implemented by copying process pointers or by treating the
subscriber database as an MME session store.

## Runtime configuration

S10 shares `mme.gtpc.server` with S11 and uses UDP/2123. Each endpoint must be
provisioned on the receiving MME. Configure the remote GUMMEI under
`mme.gtpc.client.mme`:

```yaml
mme:
  gtpc:
    server:
      - address: 192.0.2.10
    client:
      mme:
        - address: 192.0.2.20
          port: 2123
          tai:
            - plmn_id: {mcc: '001', mnc: '01'}
              tac: 2
          gummei:
            plmn_id: {mcc: '001', mnc: '01'}
            mme_gid: 2
            mme_code: 2
```

The other MME needs the reverse route. Two-digit and three-digit MNCs are
separate identities. Duplicate GUMMEI routes, invalid numeric ranges and
unknown peer configuration keys are rejected. Several GUMMEIs can reference
the same endpoint and use its shared transaction lists. The retained SGW must
also be configured as an S11 peer on the target MME.

A source context remains available while transfer is pending. A successful
Context Acknowledge and subsequent HSS Cancel Location permit local cleanup
after the source holding timer (10 seconds). Cleanup does not delete gateway
sessions that have moved to the target. A subscriber returning before expiry
can replace this retired local context. Missing/rejected exchanges retain the
source session; the target falls back to NAS authentication where applicable.

The target updates the SGW's serving-MME address and S11 TEID before the HSS
registration step. A bearerless Modify Bearer Request handles the control-plane
change before access bearer establishment. The SGW rejects a delayed Delete
Session Request from the previous MME after ownership changes.

EPS PDN bearer groups have a dedicated descriptor: SGW user-plane F-TEID is
instance 0 and PGW user-plane F-TEID is instance 1, as in mobility tables
7.3.1 and 7.3.6. The Create Session bearer descriptor has a different instance
mapping. The runtime exports the allocated PAA, rather than the subscription's
requested address, and preserves the mobile-identity encoding and masked IMEISV.

## Verification

`tests/s10` starts two independent MME processes with a common HSS/SGW. It
checks bidirectional protected TAU, unprotected TAU with Identity/AKA/Security
Mode fallback, target GUTI allocation, unchanged UE address and a returned
IPv4 ICMP echo after each move. `configs/s10-source.yaml` and
`configs/s10-target.yaml` are the integration fixtures.

Connected cases exercise both direct forwarding and indirect forwarding through
the retained SGW. Each transfers default and dedicated bearers in both directions,
checks source-path traffic during preparation, forwards downlink data to the
advertised target endpoint, transports PDCP status, registers through post-handover
TAU and verifies data after cancellation. SON configuration is tunneled in both
directions. The source SGW retains the allocating MME's forwarding binding so
cleanup replies reach the source even after the serving S11 endpoint changes.
Cleanup failures never delete PDNs transferred to the target.

Rejected Context Responses end as two-leg transactions: TS 29.274 7.3.7 requires
an Acknowledge only for an accepted Context Response. Unit coverage checks that
no ACK-response timer starts for a rejection while duplicate requests can still
receive the cached negative response.

`tests/unit/mme-s10-test.c` checks peer configuration and GUMMEI routing, MAC
failure without modifying the source context, accepted COUNT progression and
replay rejection. `tests/unit/gtp-s10-test.c` covers independent wire vectors, all fourteen message
dispatch paths, repeated PDNs/bearers, EPS context optional fields, truncated
contexts, invalid lengths and transaction stages. The existing unit suites run
alongside these cases:

```sh
meson setup build
ninja -C build tests/unit/unit
build/tests/unit/unit gtp-s10-test
build/tests/unit/unit
```

Regenerate the wire definitions with the existing generator:

```sh
cd lib/gtp/v2/support
python3 gtp-tlv.py -f r19.6.0/29274-j60.docx -o ..
```

Related design discussion: [open5gs/open5gs#4821](https://github.com/open5gs/open5gs/discussions/4821).

## Connected mobility security and wire formats

The handover foundation keeps the AS key role separately from the current NAS
KASME/eKSI. Initial Context Setup and SecurityKey-bearing UE Context Modification
requests capture the key snapshot sent to the radio. Only a matching S1 context
and procedure response confirms that snapshot; a failure retains the previous
AS context. Release of an old S1 leg does not clear the key of the serving leg.
The S1 and X2 handover paths advance the AS NH chain using its own KASME.

Forward Relocation serialization includes the previous AS context when it differs
from the active NAS context, as required by TS 33.401 7.2.10. Target import preserves
the received NH/NCC pair; it does not apply the idle TAU key derivation again.
The source waits for pending NAS security and AS installation procedures before
sending its snapshot, then freezes downlink NAS until failure or cancellation.
A successful transfer stops the source NAS timers and prevents obsolete S11
session deletion or access-path deactivation.

Target Identification supports macro, home, short macro and long macro eNB IDs.
Selected PLMN ID has a dedicated codec for the two- and three-digit MNC encoding
in TS 29.274 8.50. The Forward Relocation Response has a separate grouped bearer
schema for forwarding F-TEID instances 0 through 5 (table 7.3.2-2). It must not use
the Create Session bearer descriptor. Optional TAI routes are parsed and looked
up under each configured MME peer.

Independent wire vectors exercise these encodings. Key-role tests cover NAS
changes while an AS installation is pending, stale and mismatched responses,
failure, NH progression, old-context export and release of obsolete radio legs.
A production Forward Relocation builder test verifies opaque RAN container and
security-context serialization, with only the SGW pool lookup substituted.

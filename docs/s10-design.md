# S10 inter-MME mobility

S10 connects MMEs for mobility and context transfer. This contribution adds the
GTPv2-C message definitions, EPS MM Context codec and transaction stages used by
that interface. The MME procedure integration is tracked below.

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
one-way message, with codec support rather than a request/response transaction.

## MME integration checklist

The intended EPS scope covers both idle mobility and connected-mode S1 handover.
Each procedure needs source and target behavior:

- [ ] Configure and validate S10 peers, route GUMMEI/target identities, and
  dispatch only configured peer traffic on the GTP-C transport.
- [ ] Attach: send Identification Request for a foreign GUTI; verify the
  complete Attach Request before exporting a security context; continue local
  identification/authentication when context retrieval cannot complete.
- [ ] TAU: transfer the MM context and every PDN/bearer context; validate the
  complete TAU Request; acknowledge transfer; update HSS registration and SGW
  control-plane ownership; allocate and confirm the target GUTI.
- [ ] Retain the old context while transfer is pending. Handle acknowledgements,
  HSS cancellation, retransmissions and expiry without deleting gateway sessions
  that now belong to the target MME.
- [ ] Connected handover: exchange Forward Relocation, allocate target radio
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

## Verification

`tests/unit/gtp-s10-test.c` covers independent wire vectors, all fourteen message
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

# Connect and configure two MMEs over S10

This guide describes the S10 implementation in this branch, including mobility
with a retained SGW and with SGW relocation. Install the same branch on both
MMEs and, for the tested SGW-relocation setup, on the SGW-C/SGW-U and SMF/UPF
components. This feature is not implied to be available in an existing packaged
Open5GS release. See [S10 design and validation](s10-design.md) for the supported
procedures, standards mapping and interoperability limits.

Start from a working LTE/EPC installation with provisioned subscribers, APNs,
HSS and PCRF connectivity, a UE address pool, and working data-network routing.
The snippets below are changes to the installed configuration files, not complete
replacements: keep the existing logging, Diameter, security, session and DNS
settings. Merge entries into the existing YAML mappings; do not duplicate an
`mme`, `gtpc`, `server` or `client` key.

## Example topology

All addresses below are documentation addresses. Replace them with addresses
assigned to the corresponding hosts and reachable by their peers. A process
cannot bind an address that is not assigned locally. Use routable addresses
instead of loopback addresses when the components run on different hosts.

| Component | Address | Role |
| --- | --- | --- |
| MME-A | 192.0.2.10 | Site A, TAC 1, GUMMEI group 2/code 1 |
| SGW-C-A / SGW-U-A | 192.0.2.11 / 192.0.2.12 | Site A gateway |
| MME-B | 192.0.2.20 | Site B, TAC 2, GUMMEI group 2/code 2 |
| SGW-C-B / SGW-U-B | 192.0.2.21 / 192.0.2.22 | Site B gateway |
| SMF (PGW-C) / UPF (PGW-U) | 192.0.2.30 / 192.0.2.31 | Common PDN anchor |
| HSS | 192.0.2.40 | Common subscriber and serving-MME registration |
| eNB-A / eNB-B | 192.0.2.101 / 192.0.2.102 | Radio sites A/B |

Both sites use PLMN `001/01` in this example. Keep MCC/MNC strings quoted to
preserve their leading zeroes and the distinction between two- and three-digit
MNCs. Use a PLMN appropriate for your lab and subscribers.

```text
eNB-A -- S1AP -- MME-A -------- S10 -------- MME-B -- S1AP -- eNB-B
                  | S11                       | S11
                SGW-C-A                     SGW-C-B
                  | PFCP                      | PFCP
eNB-A -- S1-U -- SGW-U-A                     SGW-U-B -- S1-U -- eNB-B
                    \                         /
                     +------ S5/S8-U --------+---- PGW-U -- data network

SGW-C-A and SGW-C-B -- S5/S8-C -- PGW-C
MME-A and MME-B     -- S6a     -- HSS
PGW-C              -- PFCP    -- PGW-U
```

There is no separate S10 listener or `s10.enabled` switch. S10 and S11 share
the MME's GTP-C listener; provisioning the MME peer enables its S10 route.

## Configure MME-A and MME-B

Merge the following into **MME-A's `mme.yaml`**:

```yaml
mme:
  s1ap:
    server:
      - address: 192.0.2.10
  gtpc:
    server:
      - address: 192.0.2.10
    client:
      mme:
        - address: 192.0.2.20
          port: 2123
          gummei:
            plmn_id: {mcc: '001', mnc: '01'}
            mme_gid: 2
            mme_code: 2
          tai:
            - plmn_id: {mcc: '001', mnc: '01'}
              tac: 2
      sgwc:
        - address: 192.0.2.11
      smf:
        - address: 192.0.2.30
  gummei:
    - plmn_id: {mcc: '001', mnc: '01'}
      mme_gid: 2
      mme_code: 1
  tai:
    - plmn_id: {mcc: '001', mnc: '01'}
      tac: 1
  mme_name: mme-a
```

Merge the reverse configuration into **MME-B's `mme.yaml`**:

```yaml
mme:
  s1ap:
    server:
      - address: 192.0.2.20
  gtpc:
    server:
      - address: 192.0.2.20
    client:
      mme:
        - address: 192.0.2.10
          port: 2123
          gummei:
            plmn_id: {mcc: '001', mnc: '01'}
            mme_gid: 2
            mme_code: 1
          tai:
            - plmn_id: {mcc: '001', mnc: '01'}
              tac: 1
      sgwc:
        - address: 192.0.2.21
      smf:
        - address: 192.0.2.30
  gummei:
    - plmn_id: {mcc: '001', mnc: '01'}
      mme_gid: 2
      mme_code: 2
  tai:
    - plmn_id: {mcc: '001', mnc: '01'}
      tac: 2
  mme_name: mme-b
```

The peer's `gummei` must match the remote MME's served GUMMEI; it is used to
locate the old MME from a UE's GUTI. The peer's `tai` describes tracking areas
served by that peer and routes outgoing inter-MME S1 handover. It must agree
with the remote MME and its eNBs. Configure both directions; an outgoing route
alone does not provision the receiving peer. Avoid ambiguous peer TAI routes
in this example by using distinct tracking areas.

### Select retained SGW or SGW relocation

The example selects SGW-A at MME-A and SGW-B at MME-B, so a move between the
sites selects a different SGW. Selection uses the existing
`mme.gtpc.client.sgwc` configuration; there is no SGW-relocation boolean.
For multiple gateways, existing `tac` or `e_cell_id` selection filters can be
used. If `mme.dns` is enabled, its gateway selection can take precedence over
the static peers, so ensure its records select the intended gateways as well.

To retain one SGW across both MMEs, point both `sgwc` lists to the same SGW-C,
for example `192.0.2.11`, and keep that SGW's user plane reachable from both
eNBs. The retained SGW must be provisioned on the target MME. When relocation
selects a new SGW, the old SGW does not need to be added to the target MME's
local S11 peers merely to import its S10 context.

## Connect each SGW-C to its SGW-U

On **SGW-C-A**, merge into `sgwc.yaml`:

```yaml
sgwc:
  gtpc:
    server:
      - address: 192.0.2.11
  pfcp:
    server:
      - address: 192.0.2.11
    client:
      sgwu:
        - address: 192.0.2.12
```

On **SGW-U-A**, merge into `sgwu.yaml`:

```yaml
sgwu:
  pfcp:
    server:
      - address: 192.0.2.12
  gtpu:
    server:
      - address: 192.0.2.12
```

For **SGW-C-B**, use `192.0.2.21` for both server addresses and `192.0.2.22`
for the `sgwu` peer. For **SGW-U-B**, use `192.0.2.22` for both server
addresses. The SGW-C initiates PFCP association to its configured SGW-U.
An unavailable SGW-U prevents successful resource reservation during relocation.

## Keep the PGW and HSS reachable from both sites

Both MMEs initially select the common PGW-C at `192.0.2.30` through their
`smf` entries. Bind `smf.gtpc.server` to that address. Keep the existing
SMF-to-UPF PFCP configuration and bind/advertise the UPF's GTP-U endpoint at
`192.0.2.31`. Both SGW-Cs must reach the PGW-C, and both SGW-Us must reach the
PGW-U. Keep the existing APN definitions, PCRF connection, UE subnet, UPF TUN,
forwarding, return routes and any data-network NAT required by your EPC.
S10 does not configure Internet access by itself.

During relocation, each PDN keeps its **existing PGW and allocated UE address**.
The target MME's default PGW choice does not move an existing PDN to another
PGW. All transferred APNs must remain supported by the target path; for several
PGW anchors, allow reachability to every anchor that may be transferred.

Each MME also needs its own Diameter identity and connection to the common HSS.
For installations using `mme.freeDiameter` as a configuration-file path, use
separate files for the two MMEs. Change the existing identity, realm, listener
and HSS peer settings, preserving the installed extensions and TLS policy:

```text
# MME-A freeDiameter settings
Identity = "mme-a.lab.example";
Realm = "lab.example";
ListenOn = "192.0.2.10";
ConnectPeer = "hss.lab.example" { ConnectTo = "192.0.2.40"; };
```

MME-B uses identity `mme-b.lab.example` and `ListenOn = "192.0.2.20"`.
Configure the HSS with identity `hss.lab.example`, the same realm and accepted
peers for both MME identities. Ensure these identities resolve consistently
through your lab DNS or host mappings and that the certificates match the
chosen identities when TLS is used. HSS-initiated Cancel Location must be able
to reach the old MME after a serving-MME change. Sharing a subscriber database
alone does not establish S6a connectivity or replicate MME session state.

## Connect the radios and open the network paths

Configure eNB-A with MME-A's S1AP address, PLMN `001/01` and TAC 1; configure
eNB-B with MME-B's S1AP address, the same PLMN and TAC 2. Give the eNBs distinct
global eNB identities. Each target eNB must complete S1 Setup with its target
MME before connected handover can reserve radio resources.

For S1 handover, configure the radio implementation's neighbor cells,
measurements and handover trigger so the source eNB sends **Handover Required**
with the correct target global eNB identity and TAI. S10 peer configuration
does not configure the radios or initiate RF mobility. Idle TAU can be tested
separately by moving an attached UE to the other tracking area.

| Path | Transport and usual port | Required reachability |
| --- | --- | --- |
| S10 | UDP 2123 | MME-A and MME-B, both directions |
| S11 | UDP 2123 | Each MME and its selected SGW-C |
| S5/S8-C | UDP 2123 | Both SGW-Cs and each retained PGW-C anchor |
| PFCP | UDP 8805 | Each SGW-C/SGW-U pair and PGW-C/PGW-U pair |
| S1AP | SCTP 36412 | Each eNB and its serving MME |
| GTP-U | UDP 2152 | eNB/SGW-U, SGW-U/PGW-U, and forwarding endpoints |
| S6a | TCP/SCTP 3868, or configured TLS transport/port | Both MMEs and HSS |

For direct forwarding, the eNB forwarding endpoints must reach one another.
For indirect forwarding with SGW change, also allow the source and target
SGW-U forwarding endpoints to reach one another. Bind addresses and addresses
advertised in F-TEIDs must be reachable by the corresponding remote peers;
opening a firewall port does not repair an unroutable advertised address.

## Start and verify

Apply these changes in a lab using binaries built from this branch. Start the
existing database, HSS, PCRF and common PGW-C/PGW-U, then both SGW-C/SGW-U
pairs, both MMEs and finally the radios. Check Diameter and PFCP associations
before attaching a UE. Use your installation's service manager, or run a daemon
explicitly with its configuration file, for example:

```sh
open5gs-mmed -c /etc/open5gs/mme.yaml
```

Run one MME on each host with that host's file. Do not start a second manual
copy alongside an already running service bound to the same address and port.
S10 uses connectionless GTP-C; do not expect a persistent TCP-style S10
association. A UDP port scan alone does not prove that a mobility exchange works.

1. Attach on site A and record the UE address and working traffic to a host
   on the PDN/data network.
2. Move the idle UE to TAC 2. Capture Context Request/Response/Acknowledge
   between the MMEs and verify the target GUTI and resumed traffic with the
   same UE address. With SGW relocation, check the new SGW session and the
   PGW Modify Bearer exchange.
3. Trigger an inter-MME S1 handover while traffic is active. Check Forward
   Relocation, target radio preparation, Handover Notify and relocation
   completion. With SGW change, the PGW binding changes during execution,
   after target resource preparation.
4. Wait beyond the source holding interval (10 seconds) and confirm traffic
   still works after old SGW resource cleanup. Repeat in the reverse direction.
5. Test each APN and dedicated bearer in use. In a lab, also exercise target
   rejection, cancellation and unavailable-target cases before operational use.

Useful captures and logs:

```sh
sudo tcpdump -ni any 'udp port 2123'
sudo tcpdump -ni any 'udp port 8805 or udp port 2152'
journalctl -u open5gs-mmed -f
```

Use Wireshark's GTPv2 dissection to inspect peer addresses, message causes,
TEIDs and advertised user-plane endpoints. An absent Context Request usually
points to the old-GUMMEI route or an unavailable source context. An absent
Forward Relocation Request points to the target-TAI route or radio trigger.
Target radio rejection requires checking the target eNB identity, S1 Setup
and served TAI. Successful control-plane exchanges with failed traffic require
checking PFCP, GTP-U routing, MTU and the PGW/data-network return path.

## Automated regression lab

The repository includes a separate two-MME test harness. Its
`configs/s10-sgw-source.yaml.in` and `configs/s10-sgw-target.yaml.in` fixtures
launch test components with loopback addresses; they are not deployment files
to copy unchanged onto separate machines.

In an isolated Linux test environment with the build dependencies, MongoDB,
required loopback addresses and an `ogstun` TUN device configured for the test
UE subnets (`10.45.0.1/16` and `2001:db8:cafe::1/48`), build and run:

```sh
meson setup build
ninja -C build
meson test -C build --suite s10 -v -t 3
```

The suite covers retained SGW, changed SGW, target SGW failure and timeout.
Do not run it against a live EPC sharing its addresses, ports or test database.
The test harness supplies simulated radio signaling; real-radio interoperability
and peers from other vendors require separate integration tests.

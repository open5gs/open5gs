# PFCP Packet Matching Fuzzer

`pfcp_packet_match_fuzz` passes raw IPv4 or IPv6 packets directly to
`ogs_pfcp_pdr_rule_find_by_packet()` in `lib/pfcp/rule-match.c`. This is a
distinct packet-decoding and rule-matching target, not a PFCP message parser
or roundtrip target. It uses the shared `fuzzing.h` core initialization without
creating servers, sessions, PFCP contexts, or pooled PDRs/rules.

Each input owns one zero-initialized PDR and four stack-allocated rules. The
source/destination prefixes and host-order port ranges are fixed:

| Protocol | Source prefix | Destination prefix | Source ports | Destination ports |
| --- | --- | --- | --- | --- |
| IPv4 TCP | 192.0.2.0/24 | 198.51.100.0/24 | 10000-20000 | 443 |
| IPv4 UDP | 192.0.2.0/24 | 198.51.100.0/24 | 1024-65535 | 53 |
| IPv6 TCP | 2001:db8:1::/64 | 2001:db8:2::/64 | 10000-20000 | 443 |
| IPv6 ICMP | 2001:db8:1::/64 | 2001:db8:2::/64 | Not checked | Not checked |

Inputs of 1-2048 bytes are copied unchanged into a packet buffer, which is
freed after matching. Empty inputs are rejected because the matcher asserts
that packet length is nonzero. No lengths, checksums, protocols, or addresses
are repaired, preserving malformed-header and no-match paths. Rules never
depend on input data and do not require pool cleanup.

The existing `pfcp_packet_match_fuzz_seed_corpus.zip` contains 24 raw packets:
direct TCP/UDP/ICMPv6 matches; source-port, destination-port, prefix, and
protocol misses; IPv4 options; IPv6 hop-by-hop, destination, routing, and
atomic-fragment headers; and malformed/truncated headers or an unsupported
IP version. Packet bytes start with the IP header, with no harness selector.

Limits: the fixed list does not exercise rule compilation, wildcard-protocol
rules, arbitrary rule configuration, session lookup, or full IPv6 jumbograms
(which exceed the input bound). Matching results are not an oracle; sanitizer
findings in parsing and matching are the primary objective. Static checks do
not establish runtime correctness or coverage.

The local Meson registration names the executable `pfcp_packet_match_fuzz`.
OSS-Fuzz integration must separately ensure its Open5GS build script builds
and copies this executable to `$OUT` and copies the archive as
`$OUT/pfcp_packet_match_fuzz_seed_corpus.zip`. Any explicit target allowlist
must include the executable. This worktree does not modify OSS-Fuzz or add
default options/dictionaries. Build and runtime validation are left to the
user through OSS-Fuzz infrastructure.
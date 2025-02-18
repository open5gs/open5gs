/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined(OGS_PFCP_INSIDE) && !defined(OGS_PFCP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PFCP_TYPES_H
#define OGS_PFCP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_PFCP_VERSION                                    1

typedef uint16_t ogs_pfcp_pdr_id_t;
typedef uint32_t ogs_pfcp_far_id_t;
typedef uint32_t ogs_pfcp_urr_id_t;
typedef uint32_t ogs_pfcp_qer_id_t;
typedef uint8_t  ogs_pfcp_bar_id_t;

#define OGS_PFCP_CAUSE_REQUEST_ACCEPTED                     1
#define OGS_PFCP_CAUSE_MORE_USAGE_REPORT_TO_SEND            2
#define OGS_PFCP_CAUSE_REQUEST_REJECTED                     64
#define OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND            65
#define OGS_PFCP_CAUSE_MANDATORY_IE_MISSING                 66
#define OGS_PFCP_CAUSE_CONDITIONAL_IE_MISSING               67
#define OGS_PFCP_CAUSE_INVALID_LENGTH                       68
#define OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT               69
#define OGS_PFCP_CAUSE_INVALID_FORWARDING_POLICY            70
#define OGS_PFCP_CAUSE_INVALID_F_TEID_ALLOCATION_OPTION     71
#define OGS_PFCP_CAUSE_NO_ESTABLISHED_PFCP_ASSOCIATION      72
#define OGS_PFCP_CAUSE_RULE_CREATION_MODIFICATION_FAILURE   73
#define OGS_PFCP_CAUSE_PFCP_ENTITY_IN_CONGESTION            74
#define OGS_PFCP_CAUSE_NO_RESOURCES_AVAILABLE               75
#define OGS_PFCP_CAUSE_SERVICE_NOT_SUPPORTED                76
#define OGS_PFCP_CAUSE_SYSTEM_FAILURE                       77
#define OGS_PFCP_CAUSE_REDIRECTION_REQUESTED                78
#define OGS_PFCP_CAUSE_ALL_DYNAMIC_ADDRESS_ARE_OCCUPIED     79

const char *ogs_pfcp_cause_get_name(uint8_t cause);

/*
 * 8.2.11 Precedence
 *
 * The Precedence value shall be encoded as an Unsigned32 binary integer value. The lower precedence values
 * indicate higher precedence of the PDR, and the higher precedence values
 * indicate lower precedence of the PDR when matching a packet.
 */
typedef uint32_t ogs_pfcp_precedence_t;

/*
 * 8.2.2 Source Interface
 * NOTE 1: The "Access" and "Core" values denote an uplink and downlink
 * traffic direction respectively.
 * NOTE 2: For indirect data forwarding, the Source Interface in the PDR and
 * the Destination Interface in the FAR shall both be set to "Access",
 * in the forwarding SGW(s). The Interface value does not infer any
 * traffic direction, in PDRs and FARs set up for indirect data
 * forwarding, i.e. with both the Source and Destination Interfaces set
 * to Access.
 *
 * 8.2.24 Destination Interface
 * NOTE 1: The "Access" and "Core" values denote a downlink and uplink
 * traffic direction respectively.
 * NOTE 2: LI Function may denote an SX3LIF or an LMISF. See clause 5.7.
 * NOTE 3: For indirect data forwarding, the Source Interface in the PDR and
 * the Destination Interface in the FAR shall both be set to "Access",
 * in the forwarding SGW(s). The Interface value does not infer any
 * traffic direction, in PDRs and FARs set up for indirect data
 * forwarding, i.e. with both the Source and Destination Interfaces set
 * to Access.
 * NOTE 4: For a HTTP redirection, the Source Interface in the PDR to match
 * the uplink packets to be redirected and the Destination Interface in
 * the FAR to enable the HTTP redirection shall both be set to "Access".
 */
#define OGS_PFCP_INTERFACE_ACCESS                           0
#define OGS_PFCP_INTERFACE_CORE                             1
#define OGS_PFCP_INTERFACE_SGI_N6_LAN                       2
#define OGS_PFCP_INTERFACE_CP_FUNCTION                      3
#define OGS_PFCP_INTERFACE_LI_FUNCTION                      4
#define OGS_PFCP_INTERFACE_UNKNOWN                          0xff
typedef uint8_t  ogs_pfcp_interface_t;

/* 8.2.25 UP Function Features */

typedef struct ogs_pfcp_up_function_features_s {
    union {
        struct {
/*
 * 5/8 TREU Sxb, Sxc, N4
 *   Traffic Redirection Enforcement in the UP function is supported
 *   by the UP function.
 * 5/7 HEEU Sxb, Sxc, N4
 *   Header Enrichment of Uplink traffic is supported by the UP function.
 * 5/6 PFDM Sxb, Sxc, N4
 *   The PFD Management procedure is supported by the UP function.
 * 5/5 FTUP Sxa, Sxb, N4
 *   F-TEID allocation / release in the UP function is supported
 *   by the UP function.
 * 5/4 TRST Sxb, Sxc, N4
 *   Traffic Steering is supported by the UP function.
 * 5/3 DLBD Sxa, N4
 *   The buffering parameter 'DL Buffering Duration' is supported
 *   by the UP function.
 * 5/2 DDND Sxa, N4
 *   The buffering parameter 'Downlink Data Notification Delay' is supported
 *   by the UP function.
 * 5/1 BUCP Sxa, N4
 *   Downlink Data Buffering in CP function is supported by the UP function.
 */
ED8(uint8_t treu:1;,
    uint8_t heeu:1;,
    uint8_t pfdm:1;,
    uint8_t ftup:1;,
    uint8_t trst:1;,
    uint8_t dldb:1;,
    uint8_t ddnd:1;,
    uint8_t bucp:1;)
        };
        uint8_t octet5;
    };
    union {
        struct {
/*
 * 6/8 EPFAR Sxa, Sxb, Sxc, N4
 *   The UP function supports the Enhanced PFCP Association Release feature
 *   (see clause 5.18).
 * 6/7 PFDE Sxb, N4
 *   The UP function supports a PFD Contents including a property
 *   with multiple values.
 * 6/6 FRRT Sxb, N4
 *   The UP function supports Framed Routing
 *   (see IETF RFC 2865 [37] and IETF RFC 3162 [38]).
 * 6/5 TRACE Sxa, Sxb, Sxc, N4
 *   The UP function supports Trace (see clause 5.15).
 * 6/4 QUOAC Sxb, Sxc, N4
 *   The UP function supports being provisioned with the Quota Action
 *   to apply when reaching quotas.
 * 6/3 UDBC Sxb, Sxc, N4
 *   Support of UL/DL Buffering Control
 * 6/2 PDIU Sxa, Sxb, Sxc, N4
 *   Support of PDI optimised signalling in UP function (see clause 5.2.1A.2).
 * 6/1 EMPU Sxa, Sxb, N4
 *   Sending of End Marker packets supported by the UP function.
 */
ED8(uint8_t epfar:1;,
    uint8_t pfde:1;,
    uint8_t frrt:1;,
    uint8_t trace:1;,
    uint8_t quoac:1;,
    uint8_t udbc:1;,
    uint8_t pdiu:1;,
    uint8_t empu:1;)
        };
        uint8_t octet6;
    };
    union {
        struct {
/*
 * 7/8 GCOM N4
 *   UPF support of 5G VN Group Communication.(See clause 5.23)
 * 7/7 BUNDL Sxa, Sxb, Sxc, N4
 *   PFCP messages bunding (see clause 6.5) is supported by the UP function.
 * 7/6 MTE N4
 *   UPF supports multiple instances of Traffic Endpoint IDs in a PDI.
 * 7/5 MNOP Sxa, Sxb, Sxc, N4
 *   The UP function supports measurement of number of packets
 *   which is instructed with the flag 'Measurement of Number of Packets'
 *   in a URR.See also clause 5.2.2.2.1.
 * 7/4 SSET N4
 *   UPF support of PFCP sessions successively controlled
 *   by different SMFs of a same SMF Set (see clause 5.22).
 * 7/3 UEIP Sxb, N4
 *   The UP function supports allocating UE IP addresses or prefixes
 *   (see clause 5.21).
 * 7/2 ADPDP Sxa, Sxb, Sxc, N4
 *   The UP function supports the Activation and Deactivation
 *   of Pre-defined PDRs (see clause 5.19).
 * 7/1 DPDRA Sxb, Sxc, N4
 *   The UP function supports Deferred PDR Activation or Deactivation.
 */
ED8(uint8_t gcom:1;,
    uint8_t bundl:1;,
    uint8_t mte:1;,
    uint8_t mnop:1;,
    uint8_t sset:1;,
    uint8_t ueip:1;,
    uint8_t adpdp:1;,
    uint8_t dpdra:1;)
        };
        uint8_t octet7;
    };
    union {
        struct {
/*
 * 8/8 MPTCP N4
 *   UPF support of MPTCP Proxy functionality (see clause 5.20)
 * 8/7 TSCU N4
 *   Time Sensitive Communication is supported by the UPF (see clause 5.26).
 * 8/6 IP6PL N4
 *   UPF supports:
 *     - UE IPv6 address(es) allocation with IPv6 prefix length other than
 *       default /64 (including allocating /128 individual IPv6 addresses),
 *       as specified in clause 4.6.2.2 of of 3GPP TS 23.316 [57]; and
 *     - multiple UE IPv6 addresses allocation using multiple instances
 *       of the UE IP Address IE in a same PDI or Traffic Endpoint,
 *       or using multiple PDIs or Traffic Endpoints
 *       with a different UE IP Address as specified in clause 5.21.1.
 * 8/5 IPTV N4
 *   UPF support of IPTV service (see clause 5.25)
 * 8/4 NORP Sxa, Sxb, Sxc, N4
 *   UP function support of Number of Reports as specified in clause 5.2.2.2.
 * 8/3 VTIME Sxb,N4
 *   UP function support of quota validity time feature.
 * 8/2 RTTL N4
 *   UPF supports redundant transmission at transport layer.
 * 8/1 MPAS N4
 *   UPF support for multiple PFCP associations to the SMFs in an SMF set
 *   (see clause 5.22.3).
 */
ED8(uint8_t mptcp:1;,
    uint8_t tscu:1;,
    uint8_t ip6pl:1;,
    uint8_t iptv:1;,
    uint8_t norp:1;,
    uint8_t vtime:1;,
    uint8_t rttl:1;,
    uint8_t mpas:1;)
        };
        uint8_t octet8;
    };
    union {
        struct {
/*
 * 9/8 RDS Sxb, N4
 *   UP function support of Reliable Data Service (see clause 5.29).
 * 9/7 DDDS N4
 *   UPF support of reporting the first buffered / discarded downlink data
 *   for downlink data delivery status notification.
 * 9/6 ETHAR N4
 *   UPF support of Ethernet PDU Session Anchor Relocation (see clause 5.13.6).
 * 9/5 CIOT Sxb, N4
 *   UP function support of CIoT feature,
 *   e.g.small data packet rate enforcement.(see 5.4.15)
 * 9/4 MT-EDT Sxa
 *   SGW-U support of reporting the size of DL Data Packets.
 *   (see clause 5.2.4.1).
 * 9/3 GPQM N4
 *   UPF support of per GTP-U Path QoS monitoring (see clause 5.24.5).
 * 9/2 QFQM N4
 *   UPF support of per QoS flow per UE QoS monitoring (see clause 5.24.4).
 * 9/1 ATSSS-LL N4
 *   UPF support of ATSSS-LLL steering functionality (see clause 5.20)
 */
ED8(uint8_t rds:1;,
    uint8_t ddds:1;,
    uint8_t ethar:1;,
    uint8_t ciot:1;,
    uint8_t mt_edt:1;,
    uint8_t gpqm:1;,
    uint8_t qfqm:1;,
    uint8_t atsss_ll:1;)
        };
        uint8_t octet9;
    };
    union {
        struct {
/*
 * 10/8 DNSTS N4
 *   UP function support DNS Traffic Steering based on
 *   FQDN in the DNS Query message (see
 *   clause 5.33.4)
 * 10/7 IPREP N4
 *   UP function supports IP Address and Port number
 *   replacement (see clause 5.33.3).
 * 10/6 RESPS Sxb, N4
 *   UP function supports Restoration of PFCP Sessions
 *   associated with one or more PGW-C/SMF FQ-
 *   CSID(s), Group Id(s) or CP IP address(es) (see
 *   clause 5.22.4)
 * 10/5 UPBER N4
 *   UP function supports the uplink packets buffering
 *   during EAS relocation.
 * 10/4 L2TP Sxb, N4
 *   UP function supports the L2TP feature as described
 *   in clause 5.31.
 * 10/3 NSPOC Sxa, Sxb, N4
 *   UP function supports notifying start of Pause of
 *   Charging via user plane.
 * 10/2 QUASF Sxb, Sxc, N4
 *   The UP function supports being provisioned in a
 *   URR with an Exempted Application ID for Quota
 *   Action or an Exempted SDF Filter for Quota Action
 *   which is to be used when the quota is exhausted.
 *   See also clauses 5.2.2.2.1 and 5.2.2.3.1.
 * 10/1 RTTWP N4
 *   UPF support of RTT measurements towards the UE Without PMF.
 */
ED8(uint8_t dnsts:1;,
    uint8_t iprep:1;,
    uint8_t resps:1;,
    uint8_t upber:1;,
    uint8_t l2tp:1;,
    uint8_t nspoc:1;,
    uint8_t quasf:1;,
    uint8_t rttwp:1;)
        };
        uint8_t octet10;
    };
    union {
        struct {

/*
 * 11/6 UPIDP N4
 *   UP function supports User Plane Inactivity Detection
 *   and reporting per PDR feature as specified in
 *   clause 5.11.3.
 * 11/5 RATP Sxb, N4
 *   UP function supports Redirection Address Types set
 *   to "Port", "IPv4 address and Port", "IPv6 address
 *   and Port", or "IPv4 and IPv6 addresses and Port".
 * 11/4 EPPPI N4
 *   UP function supports Enhanced Provisioning of
 *   Paging Policy Indicator feature as specified in
 *   clause 5.36.2.
 * 11/3 PSUPRM N4, N4mb
 *   UP function supports Per Slice UP Resource
 *   Management (see clause 5.35).3GPP TS 29.244 version 17.7.1 Release 17
 * 11/2 MBSN4 N4
 *   UPF supports sending MBS multicast session data
 *   to associated PDU sessions using 5GC individual
 *   delivery.
 * 11/1 DRQOS N4
 *   UP function supports Direct Reporting of QoS
 *   monitoring events to Local NEF or AF (see
 *   clause 5.33.5).
 */
ED7(uint8_t spare:2;,
    uint8_t upidp:1;,
    uint8_t ratp:1;,
    uint8_t epppi:1;,
    uint8_t psuprm:1;,
    uint8_t mbsn4:1;,
    uint8_t drqos:1;)
        };
        uint8_t octet11;
    };
} __attribute__ ((packed)) ogs_pfcp_up_function_features_t;

/*
 * 8.2.26 Apply Action
 *
 * The octet 5 shall be encoded as follows:
 *
 * Bit 1 – DROP (Drop): when set to 1, this indicates a request
 * to drop the packets.
 * Bit 2 – FORW (Forward): when set to 1, this indicates a request
 * to forward the packets.
 * Bit 3 – BUFF (Buffer): when set to 1, this indicates a request
 * to buffer the packets.
 * Bit 4 – NOCP (Notify the CP function): when set to 1,
 * this indicates a request to notify the CP function about the
 * arrival of a first downlink packet being buffered.
 * Bit 5 – DUPL (Duplicate): when set to 1, this indicates a request
 * to duplicate the packets.
 * Bit 6 – IPMA (IP Multicast Accept): when set to "1", this indicates
 * a request to accept UE requests to join an IP multicast group.
 * Bit 7 – IPMD (IP Multicast Deny): when set to "1", this indicates
 * a request to deny UE requests to join an IP multicast group.
 * Bit 8 – DFRT (Duplicate for Redundant Transmission): when set to "1",
 * this indicates a request to duplicate the packets
 * for redundant transmission (see clause 5.24.2).
 *
 * The octet 6 shall be encoded as follows:
 *
 * Bit 1 – EDRT (Eliminate Duplicate Packets for Redundant Transmission):
 * when set to "1", this indicates a request to eliminate duplicate packets
 * used for redundant transmission (see clause 5.24.2).
 * Bit 2 – BDPN (Buffered Downlink Packet Notification): when set to "1",
 * this indicates a request to notify the CP function about the first buffered
 * DL packet for downlink data delivery status notification.
 * Bit 3 – DDPN (Discarded Downlink Packet Notification): when set to "1",
 * this indicates a request to notify the CP function about the first discarded
 * DL packet for downlink data delivery status notification if the DL Buffering
 * Duration or DL Buffering Suggested Packet Count is exceeded or
 * it is discarded directly. See clause 5.2.3.1.
 *
 * Bit 4 - FSSM (Forward packets to lower layer SSM): when set to "1",
 * this indicates a request to the MB-UPF to forward MBS session data
 * towards a low layer SSM address allocated by the MB-UPF
 * using multicast transport.
 * Bit 5 – MBSU (Forward and replicate MBS data using Unicast transport):
 * when set to "1", this indicates a request to forward and replicate
 * MBS session data towards multiple remote GTP-U peers using unicast transport.
 * Bit 6 to 8 – Spare, for future use and seto to "0".

 *
 * One and only one of the DROP, FORW, BUFF, IPMA and IPMD flags shall be
 * set to "1".
 *
 * The NOCP flag and BDPN flag may only be set if the BUFF flag is set.
 * The DUPL flag may be set with any of the DROP, FORW, BUFF and NOCP flags.
 * The DFRN flag may only be set if the FORW flag is set.
 * The EDRT flag may be set if the FORW flag is set.
 * The DDPN flag may be set with any of the DROP and BUFF flags.
 *
 * Both the MBSU flag and the FSSM flag may be set
 * (to require the MB-UPF to forward MBS session data
 * using both multicast and unicast transports).
 */
#define OGS_PFCP_APPLY_ACTION_DROP                          (1<<8)
#define OGS_PFCP_APPLY_ACTION_FORW                          (1<<9)
#define OGS_PFCP_APPLY_ACTION_BUFF                          (1<<10)
#define OGS_PFCP_APPLY_ACTION_NOCP                          (1<<11)
#define OGS_PFCP_APPLY_ACTION_DUPL                          (1<<12)
#define OGS_PFCP_APPLY_ACTION_IPMA                          (1<<13)
#define OGS_PFCP_APPLY_ACTION_IPMD                          (1<<14)
#define OGS_PFCP_APPLY_ACTION_DFRT                          (1<<15)
#define OGS_PFCP_APPLY_ACTION_EDRT                          (1<<0)
#define OGS_PFCP_APPLY_ACTION_BDPN                          (1<<1)
#define OGS_PFCP_APPLY_ACTION_DDPN                          (1<<2)
#define OGS_PFCP_APPLY_ACTION_FSSM                          (1<<3)
#define OGS_PFCP_APPLY_ACTION_MBSU                          (1<<4)
typedef uint16_t  ogs_pfcp_apply_action_t;

/* 8.2.58 CP Function Features */
typedef struct ogs_pfcp_cp_function_features_s {
    union {
        struct {
/*
 * 5/8 UIAUR Sxb, N4
 *   CP function supports the UE IP Address Usage Reporting feature,
 *   i.e. receiving and handling of UE IP Address Usage Information IE
 *   (see clause 5.21.3.2).
 * 5/7 ARDR Sxb, N4
 *   CP function supports Additional Usage Reports
 *   in the PFCP Session Deletion Response (see clause 5.2.2.3.1).
 * 5/6 MPAS N4
 *   SMF support for multiple PFCP associations from an SMF set to a single UPF
 *   (see clause 5.22.3).
 * 5/5 BUNDL Sxa, Sxb, Sxc, N4
 *   PFCP messages bunding (see clause 6.5) is supported by the CP function.
 * 5/4 SSET N4
 *   SMF support of PFCP sessions successively controlled by different SMFs
 *   of a same SMF Set (see clause 5.22).
 * 5/3 EPFAR Sxa, Sxb, Sxc, N4
 *   The CP function supports the Enhanced PFCP Association Release feature
 *   (see clause 5.18).
 * 5/2 OVRL Sxa, Sxb, Sxc, N4
 *   Overload Control is supported by the CP function.
 * 5/1 LOAD Sxa, Sxb, Sxc, N4
 *   Load Control is supported by the CP function.
 */
ED8(uint8_t uiaur:1;,
    uint8_t apdr:1;,
    uint8_t mpas:1;,
    uint8_t bundl:1;,
    uint8_t sset:1;,
    uint8_t epfar:1;,
    uint8_t ovrl:1;,
    uint8_t load:1;)
        };
        uint8_t octet5;
    };
    union {
        struct {

/*
 * 6/2 RPGUR Sxa, Sxb, N4, N4mb
 *   CP function supports the Peer GTP-U Entity Restart
 *   Reporting as specified in clause 20.3.4a of
 *   3GPP TS 23.007 [24] and in clause 5.5 of 3GPP TS 23.527 [40].
 * 6/1 PSUCC Sxb, Sxc, N4, N4mb
 *   CP function supports PFCP session establishment
 *   or modification with Partial Success, i.e. with UP
 *   function reporting rules that cannot be activated.
 *   See clause 5.2.9.
 */
ED3(uint8_t spare:6;,
    uint8_t rpgur:1;,
    uint8_t psucc:1;)
        };
        uint8_t octet6;
    };
} __attribute__ ((packed)) ogs_pfcp_cp_function_features_t;


/*
 * 8.2.64 Outer Header Remaval
 *
 * NOTE 1: The SGW-U/I-UPF shall store GTP-U extension header(s) required
 * to be forwarded for this packet (as required by the comprehension rules
 * of Figure 5.2.1-2 of 3GPP TS 29.281 [3]) that are not requested
 * to be deleted by the GTP-U Extension Header Deletion field.
 * NOTE 2: The SGW-U/I-UPF shall store the GTP-U message type
 * for a GTP-U signalling message which is required to be forwarded,
 * e.g. for an End Marker message.
 * NOTE 3: This value may apply to DL packets received by a PGW-U
 * for non-IP PDN connections with SGi tunnelling based
 * on UDP/IP encapsulation (see clause 4.3.17.8.3.3.2 of 3GPP TS 23.401 [14]).
 * NOTE 4: The CP function shall use this value to instruct UP function
 * to remove the GTP-U/UDP/IP header regardless it is IPv4 or IPv6.
 * NOTE 5: This value may apply to DL packets received by a UPF over N6 for
 * Ethernet PDU sessions over (see clause 5.8.2.11.3 of 3GPP TS 23.501 [28]).
 * NOTE 6: This value may apply e.g. to DL packets received by a UPF
 * (PDU Session Anchor) over N6, when explicit N6 traffic routing information
 * is provided to the SMF (see clause 5.6.7 of 3GPP TS 23.501 [28]).
 *
 * The GTP-U Extension Header Deletion field (octet 6) shall be present
 * if it is required to delete GTP-U extension header(s) from incoming GTP-PDUs.
 * Octet 6 shall be absent if all GTP-U extension headers required
 * to be forwarded shall be stored as indicated in NOTE 1 of Table 8.2.64-1.
 *
 * The GTP-U Extension Header Deletion field, when present, shall be encoded
 * as specified in Table 8.2.64-2. It takes the form of a bitmask where each bit
 * provides instructions on the information to be deleted from the incoming
 * GTP-PDU packet. Spare bits shall be ignored by the receiver.
 */
typedef struct ogs_pfcp_outer_header_removal_s {
#define OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4         0
#define OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IPV6         1
#define OGS_PFCP_OUTER_HEADER_REMOVAL_UDP_IPV4              2
#define OGS_PFCP_OUTER_HEADER_REMOVAL_UDP_IPV6              3
#define OGS_PFCP_OUTER_HEADER_REMOVAL_IPV4                  4
#define OGS_PFCP_OUTER_HEADER_REMOVAL_IPV6                  5
#define OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IP           6
#define OGS_PFCP_OUTER_HEADER_REMOVAL_VLAN_STAG             7
#define OGS_PFCP_OUTER_HEADER_REMOVAL_SLAN_CTAG             8
    uint8_t description;

#define OGS_PFCP_PDU_SESSION_CONTAINER_TO_BE_DELETED        1
    uint8_t gtpu_extheader_deletion;
} ogs_pfcp_outer_header_removal_t;

/******************************************************************************
 * PFCP Node ID structure
 ******************************************************************************/
#define OGS_PFCP_NODE_ID_IPV4   0
#define OGS_PFCP_NODE_ID_IPV6   1
#define OGS_PFCP_NODE_ID_FQDN   2

/******************************************************************************
 * Add this line to define the UNKNOWN type. We use '3' since 0,1,2 are taken.
 ******************************************************************************/
#define OGS_PFCP_NODE_ID_UNKNOWN 0xf
typedef struct ogs_pfcp_node_id_s {
ED2(uint8_t     spare:4;,
    uint8_t     type:4;)
    union {
        uint32_t addr;
        uint8_t addr6[OGS_IPV6_LEN];
        char fqdn[OGS_MAX_FQDN_LEN];
    };
} __attribute__ ((packed)) ogs_pfcp_node_id_t;

typedef struct ogs_pfcp_f_seid_s {
ED3(uint8_t     spare:6;,
    uint8_t     ipv4:1;,
    uint8_t     ipv6:1;)
    uint64_t    seid;
    union {
        uint32_t addr;
        uint8_t addr6[OGS_IPV6_LEN];
        struct {
            uint32_t addr;
            uint8_t addr6[OGS_IPV6_LEN];
        } both;
    };
} __attribute__ ((packed)) ogs_pfcp_f_seid_t;

/*
 * 8.2.3 F-TEID
 *
 * The following flags are coded within Octet 5:
 *
 * - Bit 1 – V4: If this bit is set to "1" and the CH bit is not set,
 *   then the IPv4 address field shall be present,
 *   otherwise the IPv4 address field shall not be present.
 * - Bit 2 – V6: If this bit is set to "1" and the CH bit is not set,
 *   then the IPv6 address field shall be present,
 *   otherwise the IPv6 address field shall not be present.
 * - Bit 3 – CH (CHOOSE): If this bit is set to "1", then the TEID,
 *   IPv4 address and IPv6 address fields shall not be present and
 *   the UP function shall assign an F-TEID with an IP4 or an IPv6 address
 *   if the V4 or V6 bit is set respectively. This bit shall only be set
 *   by the CP function.
 * - Bit 4 – CHID (CHOOSE ID): If this bit is set to "1",
 *   then the UP function shall assign the same F-TEID to the PDRs requested
 *   to be created in a PFCP Session Establishment Request or
 *   PFCP Session Modification Request with the same CHOOSE ID value.
 *   This bit may only be set to "1" if the CH bit it set to "1".
 *   This bit shall only be set by the CP function.
 * - Bit 5 to 8: Spare, for future use and set to 0.
 *
 * At least one of the V4 and V6 flags shall be set to "1", and
 * both may be set to "1" for both scenarios:
 *
 * - when the CP function is allocating F-TEID, i.e. both IPv4 address field
 *   and IPv6 address field may be present;
 * - or when the UP function is requested to allocate the F-TEID,
 *   i.e. when CHOOSE bit is set to "1", and the IPv4 address and
 *   IPv6 address fields are not present.
 *
 * Octet 6 to 9 (TEID) shall be present and shall contain a GTP-U TEID,
 * if the CH bit in octet 5 is not set. When the TEID is present,
 * if both IPv4 and IPv6 addresses are present in the F-TEID IE,
 * then the TEID value shall be shared by both addresses.
 *
 * Octets "m to (m+3)" and/or "p to (p+15)"(IPv4 address / IPv6 address fields),
 * if present, it shall contain the respective IP address values.
 *
 * Octet q shall be present and shall contain a binary integer value
 * if the CHID bit in octet 5 is set to "1".
 */
typedef struct ogs_pfcp_f_teid_s {
ED5(uint8_t     spare1:4;,
    uint8_t     chid:1;,
    uint8_t     ch:1;,
    uint8_t     ipv6:1;,
    uint8_t     ipv4:1;)
    union {
        struct {
        ED4(uint8_t choose_id;,
            uint8_t spare2;,
            uint8_t spare3;,
            uint8_t spare4;)
        };
        struct {
            uint32_t teid;
            union {
                uint32_t addr;
                uint8_t addr6[OGS_IPV6_LEN];
                struct {
                    uint32_t addr;
                    uint8_t addr6[OGS_IPV6_LEN];
                } both;
            };
        };
    };
} __attribute__ ((packed)) ogs_pfcp_f_teid_t;

/*
 * 8.2.62 UE IP Address
 *
 * - Bit 1 – V6: If this bit is set to "1", then the IPv6 address field
 *   shall be present in the UE IP Address, otherwise the IPv6 address field
 *   shall not be present.
 * - Bit 2 – V4: If this bit is set to "1", then the IPv4 address field
 *   shall be present in the UE IP Address, otherwise the IPv4 address field
 *   shall not be present.
 * - Bit 3 – S/D: This bit is only applicable to the UE IP Address IE
 *   in the PDI IE. It shall be set to "0" and ignored by the receiver
 *   in IEs other than PDI IE. In the PDI IE, if this bit is set to "0",
 *   this indicates a Source IP address; if this bit is set to "1",
 *   this indicates a Destination IP address.
 * - Bit 4 – IPv6D: This bit is only applicable to the UE IP address IE
 *   in the PDI IE and whhen V6 bit is set to "1". If this bit is set to "1",
 *   then the IPv6 Prefix Delegation Bits field shall be present,
 *   otherwise the UP function shall consider IPv6 prefix is default /64.
 * - Bit 5 to 8 Spare, for future use and set to 0.
 *
 * Octets "m to (m+3)" or "p to (p+15)" (IPv4 address / IPv6 address fields),
 * if present, shall contain the address value.
 *
 * Octet r, if present, shall contain the number of bits is allocated
 * for IPv6 prefix delegation, e.g. if /60 prefix is used, the value
 * is set to "4". When using UE IP address IE in a PDI to match the packets,
 * the UP function shall only use the IPv6 prefix part and
 * ignore the interface identifier part.
 */
typedef struct ogs_pfcp_ue_ip_addr_s {
ED8(uint8_t     spare:1;,
    uint8_t     ip6pl:1;,
    uint8_t     chv6:1;,
    uint8_t     chv4:1;,
    uint8_t     ipv6d:1;,
#define OGS_PFCP_UE_IP_SRC     0
#define OGS_PFCP_UE_IP_DST     1
    uint8_t     sd:1;,
    uint8_t     ipv4:1;,
    uint8_t     ipv6:1;)
    union {
        uint32_t addr;
        uint8_t addr6[OGS_IPV6_LEN];
        struct {
            uint32_t addr;
            uint8_t addr6[OGS_IPV6_LEN];
        } both;
    };
} __attribute__ ((packed)) ogs_pfcp_ue_ip_addr_t;

/*
 * 8.2.56 Outer Header Creation
 *
 * NOTE 1: The SGW-U/I-UPF shall also create GTP-U extension header(s)
 * if any has been stored for this packet, during a previous outer header
 * removal (see clause 8.2.64).
 * NOTE 2: This value may apply to UL packets sent by a PGW-U
 * for non-IP PDN connections with SGi tunnelling based on UDP/IP encapsulation
 * (see clause 4.3.17.8.3.3.2 of 3GPP TS 23.401 [14]).
 * NOTE 3: The SGW-U/I-UPF shall set the GTP-U message type
 * to the value stored during the previous outer header removal.
 * NOTE 4: This value may apply to UL packets sent by a UPF
 * for Ethernet PDU sessions over N6
 * (see clause 5.8.2.11.6 of 3GPP TS 23.501 [28]).
 * NOTE 5: This value may apply e.g. to UL packets sent by a UPF
 * (PDU Session Anchor) over N6, when explicit N6 traffic routing information is provided to the SMF (see clause 5.6.7 of 3GPP TS 23.501 [28]).
 *
 * At least one bit of the Outer Header Creation Description field
 * shall be set to 1. Bits 5/1 and 5/2 may both be set to 1 if an F-TEID
 * with both an IPv4 and IPv6 addresses has been assigned by the GTP-U peer.
 * In this case, the UP function shall send the outgoing packet
 * towards the IPv4 or IPv6 address.
 *
 * The TEID field shall be present if the Outer Header Creation Description
 * requests the creation of a GTP-U header. Otherwise it shall not be present.
 * When present, it shall contain the destination GTP-U TEID to set
 * in the GTP-U header of the outgoing packet.
 *
 * The IPv4 Address field shall be present if the Outer Header Creation
 * Description requests the creation of an IPv4 header. Otherwise it shall
 * not be present. When present, it shall contain the destination IPv4 address
 * to set in the IPv4 header of the outgoing packet.
 *
 * The IPv6 Address field shall be present if the Outer Header Creation
 * Description requests the creation of an IPv6 header. Otherwise it shall
 * not be present. When present, it shall contain the destination IPv6 address
 * to set in the IPv6 header of the outgoing packet.
 *
 * The Port Number field shall be present if the Outer Header Creation
 * Description requests the creation of a UDP/IP header
 * (i.e. it is set to the value 4). Otherwise it shall not be present.
 * When present, it shall contain the destination Port Number to set
 * in the UDP header of the outgoing packet.
 *
 * The C-TAG field shall be present if the Outer Header Creation Description
 * requests the setting of the C-Tag in Ethernet packet. Otherwise it shall
 * not be present. When present, it shall contain the destination Customer-VLAN
 * tag to set in the Customer-VLAN tag header of the outgoing packet.
 *
 * The S-TAG field shall be present if the Outer Header Creation Description
 * requests the setting of the S-Tag in Ethernet packet. Otherwise it shall
 * not be present. When present, it shall contain the destination Service-VLAN
 * tag to set in the Service-VLAN tag header of the outgoing packet.
 */
typedef struct ogs_pfcp_outer_header_creation_s {
ED8(uint8_t     stag:1;,
    uint8_t     ctag:1;,
    uint8_t     ip6:1;,
    uint8_t     ip4:1;,
    uint8_t     udp6:1;,
    uint8_t     udp4:1;,
    uint8_t     gtpu6:1;,
    uint8_t     gtpu4:1;)
ED4(uint8_t     spare:5;,
    uint8_t     ssm_c_teid:1;,
    uint8_t     n6:1;,
    uint8_t     n19:1;)
    uint32_t    teid;
    union {
        uint32_t addr;
        uint8_t addr6[OGS_IPV6_LEN];
        struct {
            uint32_t addr;
            uint8_t addr6[OGS_IPV6_LEN];
        } both;
    };
} __attribute__ ((packed)) ogs_pfcp_outer_header_creation_t;

/*
 * 8.2.82 User Plane IP Resource Information
 *
 * The following flags are coded within Octet 5:
 * - Bit 1 – V4: If this bit is set to "1", then the IPv4 address field
 *   shall be present, otherwise the IPv4 address field shall not be present.
 * - Bit 2 – V6: If this bit is set to "1", then the IPv6 address field
 *   shall be present, otherwise the IPv6 address field shall not be present.
 * - Bit 3-5 – TEID Range Indication (TEIDRI): the value of this field
 *   indicates the number of bits in the most significant octet of a TEID
 *   that are used to partition the TEID range,
 *   e.g. if this field is set to "4", then the first 4 bits in the TEID
 *   are used to partition the TEID range.
 * - Bit 6 – Associated Network Instance (ASSONI): if this bit is set to "1",
 *   then the Network Instance field shall be present, otherwise the Network
 *   Instance field shall not be present.
 * - Bit 7 – Associated Source Interface (ASSOSI): if this bit is set to "1",
 *   then the Source Interface field shall be present,
 *   otherwise the Source Interface field shall not be present.
 * - Bit 8: Spare, for future use and set to 0.
 *
 * At least one of the V4 and V6 flags shall be set to "1",
 * and both may be set to "1".
 *
 * If both the ASSONI and ASSOSI flags are set to "0", this shall indicate
 * that the User Plane IP Resource Information provided can be used
 * by CP function for any Network Instance and any Source Interface
 * of GTP-U user plane in the UP function.  Octet 6 (TEID Range) shall be
 * present if the TEID Range Indication is not set to zero and
 * shall contain a value of the bits which are used to partition the TEID range.
 * E.g. if the TEID Range Indication is set to "4", then Octet 6 shall be
 * one of values between 0 and 15. When TEID Range Indication is set to zero,
 * the Octet 6 shall not be present, the TEID is not partitioned,
 * i.e. all TEID values are available for use by the CP function.
 *
 * Octets "m to (m+3)" and/or "p to (p+15)" (IPv4 address / IPv6 address fields)
 * , if present, shall contain the respective IP address values.
 *
 * Octets "k to l", if present, shall contain a Network Instance value
 * as encoded in octet "5 to n+4" of the Figure 8.2.4-1 in clause 8.2.4,
 * identifying a Network Instance with which the IP address or TEID Range
 * is associated.
 *
 * Octet r, if present, shall contain a Source Interface value as encoded
 * in octet 5 of the Figure 8.2.2-1 in clause 8.2.2,
 * identifying the Source Interface with which the IP address or TEID Range
 * is associated.
 */
int16_t ogs_pfcp_build_user_plane_ip_resource_info(
        ogs_tlv_octet_t *octet,
        ogs_user_plane_ip_resource_info_t *info,
        void *data, int data_len);
int16_t ogs_pfcp_parse_user_plane_ip_resource_info(
        ogs_user_plane_ip_resource_info_t *info,
        ogs_tlv_octet_t *octet);

/*
 * 8.2.5 SDF Filter
 *
 * The SDF Filter IE type shall be encoded as shown in Figure 8.2.5-1.
 * It contains an SDF Filter, i.e. a single IP flow packet filter.
 *
 * The following flags are coded within Octet 5:
 * - Bit 1 – FD (Flow Description): If this bit is set to "1",
 *   then the Length of Flow Description and the Flow Description fields
 *   shall be present, otherwise they shall not be present.
 * - Bit 2 – TTC (ToS Traffic Class): If this bit is set to "1",
 *   then the ToS Traffic Class field shall be present,
 *   otherwise the ToS Traffic Class field shall not be present.
 * - Bit 3 – SPI (Security Parameter Index): If this bit is set to "1",
 *   then the Security Parameter Index field shall be present,
 *   otherwise the Security Parameter Index field shall not be present.
 * - Bit 4 – FL (Flow Label): If this bit is set to "1",
 *   then the Flow Label field shall be present, otherwise the Flow Label field
 *   shall not be present.
 * - Bit 5 – BID (Bidirectional SDF Filter): If this bit is set to "1",
 *   then the SDF Filter ID shall be present, otherwise the SDF Filter ID
 *   shall not be present.
 * - Bit 6 to 8: Spare, for future use and set to 0.
 *
 * The Flow Description field, when present, shall be encoded as an OctetString
 * as specified in clause 5.4.2 of 3GPP TS 29.212 [8].
 *
 * The ToS Traffic Class field, when present, shall be encoded as an OctetString
 * on two octets as specified in clause 5.3.15 of 3GPP TS 29.212 [8].
 *
 * The Security Parameter Index field, when present, shall be encoded as
 * an OctetString on four octets and shall contain the IPsec security parameter
 * index (which is a 32-bit field), as specified in clause 5.3.51
 * of 3GPP TS 29.212 [8].
 *
 * The Flow Label field, when present, shall be encoded as an OctetString
 * on 3 octets as specified in clause 5.3.52 of 3GPP TS 29.212 [8] and
 * shall contain an IPv6 flow label (which is a 20-bit field).
 *
 * The bits 8 to 5 of the octet "v" shall be spare and set to zero, and
 * the remaining 20 bits shall contain the IPv6 flow label.
 *
 * An SDF Filter may:
 *
 * - be a pattern for matching the IP 5 tuple (source IP address or
 *   IPv6 network prefix, destination IP address or IPv6 network prefix,
 *   source port number, destination port number, protocol ID of the protocol
 *   above IP). In the pattern:
 *   - a value left unspecified in a filter matches any value of
 *     the corresponding information in a packet;
 *   - an IP address may be combined with a prefix mask;
 *   - port numbers may be specified as port ranges;
 *   - the pattern can be extended by the Type of Service (TOS) (IPv4) /
 *     Traffic class (IPv6) and Mask;
 *
 * - consist of the destination IP address and optional mask, protocol ID
 *   of the protocol above IP, the Type of Service (TOS) (IPv4) /
 *   Traffic class (IPv6) and Mask and the IPsec Security Parameter Index (SPI);
 *
 * - consist of the destination IP address and optional mask,
 *   the Type of Service (TOS) (IPv4) / Traffic class (IPv6) and Mask and
 *   the Flow Label (IPv6).
 *
 * NOTE 1: The details about the IPsec Security Parameter Index (SPI),
 *         the Type of Service (TOS) (IPv4) / Traffic class (IPv6) and Mask and
 *         the Flow Label (IPv6) are defined in 3GPP TS 23.060 [19] clause 15.3.
 *
 * - extend the packet inspection beyond the possibilities described above and
 *   look further into the packet. Such service data flow filters need
 *   to be predefined in the PGW-U, as specified in clause 5.11
 *   of 3GPP TS 23.214 [2].
 *
 * NOTE 2: Such filters may be used to support filtering with respect
 *         to a service data flow based on the transport and application
 *         protocols used above IP, e.g. for HTTP and WAP. Filtering
 *         for further application protocols and services can also be supported.
 *
 * The SDF Filter ID, when present, shall be encoded as
 * an Unsigned32 binary integer value. It shall uniquely identify
 * an SDF Filter among all the SDF Filters provisioned for a given PFCP Session.
 * The source/destination IP address and port information, in a bidirectional
 * SDF Filter, shall be set as for downlink IP flows. The SDF filter
 * for the opposite direction has the same parameters, but having
 * the source and destination address/port parameters swapped. When being
 * provisioned with a bidirectional SDF filter in a PDR,
 * the UP function shall apply the SDF filter as specified in clause 5.2.1A.2A.
 */

typedef struct ogs_pfcp_sdf_filter_s {
    union {
        struct {
ED6(uint8_t     spare1:3;,
    uint8_t     bid:1;,
    uint8_t     fl:1;,
    uint8_t     spi:1;,
    uint8_t     ttc:1;,
    uint8_t     fd:1;)
        };
        uint8_t flags;
    };

    uint8_t     spare2;
    uint16_t    flow_description_len;
    char        *flow_description;
    uint16_t    tos_traffic_class;
    uint32_t    security_parameter_index;
    uint32_t    flow_label;
    uint32_t    sdf_filter_id;
} __attribute__ ((packed)) ogs_pfcp_sdf_filter_t;

int16_t ogs_pfcp_build_sdf_filter(
        ogs_tlv_octet_t *octet, ogs_pfcp_sdf_filter_t *filter,
        void *data, int data_len);
int16_t ogs_pfcp_parse_sdf_filter(
        ogs_pfcp_sdf_filter_t *filter, ogs_tlv_octet_t *octet);

/*
 * 8.2.8 MBR
 *
 * The UL/DL MBR fields shall be encoded as kilobits per second
 * (1 kbps = 1000 bps) in binary value. The UL/DL MBR fields may require
 * converting values in bits per second to kilobits per second
 * when the UL/DL MBR values are received from an interface other than
 * GTPv2 interface. If such conversions result in fractions,
 * then the value of UL/DL MBR fields shall be rounded upwards.
 * The range of UL/DL MBR is specified in 3GPP TS 36.413 [10].
 *
 * NOTE: The encoding is aligned on the encoding specified
 * in 3GPP TS 29.274 [9].
 */

#define OGS_PFCP_BITRATE_LEN 10
typedef struct ogs_pfcp_bitrate_s {
    uint64_t    uplink;
    uint64_t    downlink;
} __attribute__ ((packed)) ogs_pfcp_bitrate_t;

int16_t ogs_pfcp_build_bitrate(ogs_tlv_octet_t *octet,
        ogs_pfcp_bitrate_t *bitrate, void *data, int data_len);
int16_t ogs_pfcp_parse_bitrate(
        ogs_pfcp_bitrate_t *bitrate, ogs_tlv_octet_t *octet);

#define OGS_PFCP_GATE_OPEN 0
#define OGS_PFCP_GATE_CLOSE 1
typedef struct ogs_pfcp_gate_status_s {
    union {
        struct {
ED3(uint8_t     spare:4;,
    uint8_t     uplink:2;,
    uint8_t     downlink:2;)
        };
        uint8_t value;
    };
} __attribute__ ((packed)) ogs_pfcp_gate_status_t;

/* 8.2.19 Reporting Triggers
 */
typedef struct ogs_pfcp_reporting_triggers_s {
    union {
        struct {
ED8(uint8_t linked_usage_reporting:1;,
    uint8_t dropped_dl_traffic_threshold:1;,
    uint8_t stop_of_traffic:1;,
    uint8_t start_of_traffic:1;,
    uint8_t quota_holding_time:1;,
    uint8_t time_threshold:1;,
    uint8_t volume_threshold:1;,
    uint8_t periodic_reporting:1;)
        };
        uint8_t reptri_5;
    };
    union {
        struct {
ED8(uint8_t quota_validity_time:1;,
    uint8_t ip_multicast_join_leave:1;,
    uint8_t event_quota:1;,
    uint8_t event_threshold:1;,
    uint8_t mac_addresses_reporting:1;,
    uint8_t envelope_closure:1;,
    uint8_t time_quota:1;,
    uint8_t volume_quota:1;)
        };
        uint8_t reptri_6;
    };
    union {
        struct {
ED3(uint8_t spare:6;,
    uint8_t user_plane_inactivity_timer:1;,
    uint8_t report_the_end_marker_reception:1;)
        };
        uint8_t reptri_7;
    };
} __attribute__ ((packed)) ogs_pfcp_reporting_triggers_t;

/*
 * 8.2.21 Report Type
 *
 * Octet 5 shall be encoded as follows:
 *
 * - Bit 1 – DLDR (Downlink Data Report): when set to 1,
 *           this indicates Downlink Data Report
 * - Bit 2 – USAR (Usage Report): when set to 1, this indicates a Usage Report
 * - Bit 3 – ERIR (Error Indication Report): when set to 1,
 *           this indicates an Error Indication Report.
 * - Bit 4 – UPIR (User Plane Inactivity Report): when set to 1,
 *           this indicates a User Plane Inactivity Report.
 * - Bit 5 – TMIR (TSC Management Information Report): when set to "1",
 *           this indicates a TSC Management Information Report.
 * - Bit 6 – Session Report (SESR): when set to "1",
 *           this indicates a Session Report.
 * - Bit 7 – UISR (UP Initiated Session Request): when set to "1",
 *           this indicates it is a UP function initiated request
 *           for a reason which is indicated by the PFCPSRReq-Flags,
 *           for the PFCP session.
 * - Bit 8 – Spare, for future use and set to "0".
 *
 * At least one bit shall be set to 1. Several bits may be set to 1.
 */
typedef struct ogs_pfcp_report_type_s {
    union {
        struct {
ED8(uint8_t     spare:1;,
    uint8_t     up_initiated_session_request:1;,
    uint8_t     session_report:1;,
    uint8_t     tsc_management_information_report:1;,
    uint8_t     user_plane_inactivity_report:1;,
    uint8_t     error_indication_report:1;,
    uint8_t     usage_report:1;,
    uint8_t     downlink_data_report:1;)
        };
        uint8_t value;
    };
} __attribute__ ((packed)) ogs_pfcp_report_type_t;

/*
 * 8.2.27 Downlink Data Service Information
 */
typedef struct ogs_pfcp_downlink_data_service_information_s {
    struct {
ED3(uint8_t     spare:6;,
    uint8_t     qfii:1;,
    uint8_t     ppi:1;)
    };
    union {
        uint8_t paging_policy_indication_value;
        uint8_t qfi;
        struct {
            uint8_t paging_policy_indication_value;
            uint8_t qfi;
        } both;
    };
} __attribute__ ((packed)) ogs_pfcp_downlink_data_service_information_t;

/*
 * 8.2.31 PFCPSMReq-Flags
 *
 * The following bits within Octet 5 shall indicate:
 * - Bit 1 – DROBU (Drop Buffered Packets): if this bit is set to 1,
 *   it indicates that the UP function shall drop all the packets currently
 *   buffered for the PFCP session, if any, prior to further applying
 *   the action specified in the Apply Action value of the FARs.
 * - Bit 2 – SNDEM (Send End Marker Packets): if this bit is set to 1,
 *   it indicates that the UP function shall construct and send End Marker
 *   packets towards the old F-TEID of the downstream node when switching
 *   to the new F- TEID.
 * - Bit 3 – QAURR (Query All URRs): if this bit is set to 1, it indicates
 *   that the UP function shall return immediate usage report(s)
 *   for all the URRs previously provisioned for this PFCP session.
 * - Bit 4 - SUMPC (Stop of Usage Measurement to Pause Charging):
 *   if this bit is set to "1", it indicates that the UP function
 *   shall stop the usage measurement for all URRs
 *   with the "ASPOC" flag set to "1".
 * - Bit 5 - RUMUC (Resume of Usage Measurement to Un-pause of Charging):
 *   if this bit is set to "1", it indicates that the UP function
 *   shall resume the usage measurement for all URRs
 *   with the "ASPOC" flag set to "1".
 * - Bit 6 - DETEID (Delete All DL N3mb and/or N19mb F-TEIDs):
 *   if this bit is set to "1", it indicates that the MB-UPF
 *   shall delete all NG-RAN N3mb DL F-TEIDs
 *   and all UPF N19mb DL F-TEIDs which were provisioned
 *   in Add MBS Unicast Parameters IEs for the MBS session
 *   (see clause 5.34.2.4).
 * - Bit 7 to 8 – Spare, for future use, shall be set to "0" by the sender
 *   and discarded by the receiver.
 */
typedef struct ogs_pfcp_smreq_flags_s {
    union {
        struct {
ED7(uint8_t     spare:2;,
    uint8_t     delete_all_dl_n3mb_and_or_n19mb_f_teids:1;,
    uint8_t     resume_of_usage_measurement_to_un_pause_of_charging:1;,
    uint8_t     stop_of_usage_measurement_to_pause_charging:1;,
    uint8_t     query_all_urrs:1;,
    uint8_t     send_end_marker_packets:1;,
    uint8_t     drop_buffered_packets:1;)
        };
        uint8_t value;
    };
} __attribute__ ((packed)) ogs_pfcp_smreq_flags_t;

/*
 * 8.2.40 Measurement Method
 *
 * Octet 5 shall be encoded as follows:
 * - Bit 1 – DURAT (Duration): when set to "1",
 *   this indicates a request for measuring the duration of the traffic.
 * - Bit 2 – VOLUM (Volume): when set to "1",
 *   this indicates a request for measuring the volume of the traffic.
 * - Bit 3 – EVENT (Event): when set to "1",
 *   this indicates a request for measuring the events.
 * - Bit 4 to 8: Spare, for future use and set to "0".
 *
 * At least one bit shall be set to "1". Several bits may be set to "1".
 */
#define OGS_PFCP_MEASUREMENT_METHOD_DURATION    1
#define OGS_PFCP_MEASUREMENT_METHOD_VOLUME      2
#define OGS_PFCP_MEASUREMENT_METHOD_EVENT       4
typedef uint8_t ogs_pfcp_measurement_method_t;

/*
 * 8.2.41 Usage Report Trigger
 *
 * The Usage Report Trigger IE shall be encoded as shown in Figure 8.2.41-1.
 * It indicates the trigger of the usage report.
 */
typedef struct ogs_pfcp_usage_report_trigger_s {
    union {
        struct {
ED8(uint8_t immediate_report:1;;,
    uint8_t dropped_dl_traffic_threshold:1;,
    uint8_t stop_of_traffic:1;,
    uint8_t start_of_traffic:1;,
    uint8_t quota_holding_time:1;,
    uint8_t time_threshold:1;,
    uint8_t volume_threshold:1;,
    uint8_t periodic_reporting:1;)
        };
        uint8_t reptri_5;
    };
    union {
        struct {
ED8(uint8_t event_threshold:1;,
    uint8_t mac_addresses_reporting:1;,
    uint8_t envelope_closure:1;,
    uint8_t monitoring_time:1;,
    uint8_t termination_report:1;,
    uint8_t linked_usage_reporting:1;,
    uint8_t time_quota:1;,
    uint8_t volume_quota:1;)
        };
        uint8_t reptri_6;
    };
    union {
        struct {
ED7(uint8_t spare:2;,
    uint8_t user_plane_inactivity_timer:1;,
    uint8_t report_the_end_marker_reception:1;,
    uint8_t quota_validity_time:1;,
    uint8_t ip_multicast_join_leave:1;,
    uint8_t termination_by_up_function_report:1;,
    uint8_t event_quota:1;)
        };
        uint8_t reptri_7;
    };
} __attribute__ ((packed)) ogs_pfcp_usage_report_trigger_t;

void ogs_pfcp_parse_usage_report_trigger(
        ogs_pfcp_usage_report_trigger_t *rep_trig,
        ogs_pfcp_tlv_usage_report_trigger_t *tlv);

/*
 * 8.2.42 Measurement Period
 *
 * The Measurement Period IE contains the period, in seconds,
 * for generating periodic usage reports or the periodic QoS monitoring reports.
 * It shall be encoded as shown in Figure 8.2.42-1.
 *
 * The Measurement Period field shall be encoded
 * as an Unsigned32 binary integer value.
 */
typedef uint32_t ogs_pfcp_measurement_period_t;

/*
 * 8.2.13 Volume Threshold
 */
typedef struct ogs_pfcp_volume_threshold_s {
    union {
        struct {
ED4(uint8_t spare:5;,
    uint8_t dlvol:1;,
    uint8_t ulvol:1;,
    uint8_t tovol:1;)
        };
    uint8_t flags;
    };

    uint64_t total_volume;
    uint64_t uplink_volume;
    uint64_t downlink_volume;

} __attribute__ ((packed)) ogs_pfcp_volume_threshold_t;

int16_t ogs_pfcp_build_volume(ogs_tlv_octet_t *octet,
        ogs_pfcp_volume_threshold_t *volume, void *data, int data_len);
int16_t ogs_pfcp_parse_volume(
        ogs_pfcp_volume_threshold_t *volume, ogs_tlv_octet_t *octet);

/*
 * 8.2.50 Volume Quota
 */
typedef ogs_pfcp_volume_threshold_t ogs_pfcp_volume_quota_t;

/*
 * 8.2.113 Event Threshold
 *
 * The Event Threshold IE contains the Number of events after
 * which the measurement report is to be generated by the UP function.
 *
 * It shall be encoded as shown in Figure 8.2.113-1.
 */
typedef uint32_t ogs_pfcp_event_threshold_t;

/*
 * 8.2.112 Event Quota
 *
 * The Event Quota IE type shall be encoded as shown in Figure 8.2.112-1.
 * It contains the event quota to be monitored by the UP function.
 */
typedef uint32_t ogs_pfcp_event_quota_t;

/*
 * 8.2.14 Time Threshold
 *
 * The Time Threshold IE contains the traffic duration threshold
 * to be monitored by the UP function. It shall be encoded as shown
 * in Figure 8.2.14-1.
 */
typedef uint32_t ogs_pfcp_time_threshold_t;

/*
 * 8.2.51 Time Quota
 *
 * The Time Quota IE type shall be encoded as shown in Figure 8.2.51-1.
 * It contains the time quota to be monitored by the UP function.
 */
typedef uint32_t ogs_pfcp_time_quota_t;

/*
 * 8.2.48 Quota Holding Time
 *
 * The Quota Holding Time IE type shall be encoded as shown in Figure 8.2.48-1.
 * It contains the quota holding time in seconds.
 *
 * The Quota Holding Time value shall be encoded as
 * an Unsigned32 binary integer value.
 */
typedef uint32_t ogs_pfcp_quota_holding_time_t;

/*
 * 8.2.132 Quota Validity Time
 *
 * The Quota Validity Time IE type shall be encoded as shown
 * in Figure 8.2.132-1. It contains the quota validity time in seconds.
 *
 * The Quota Validity Time value shall be encoded as
 * an Unsigned32 binary integer value.
 */
typedef uint32_t ogs_pfcp_quota_validity_time_t;

/*
 * 8.2.49 Dropped DL Traffic Threshold
 *
 * The Dropped DL Traffic Threshold IE type shall be encoded as shown
 * in Figure 8.2.49-1. It contains the dropped DL traffic volume thresholds
 * to be monitored by the UP function.
 *
 * The following flags are coded within Octet 5:
 * - Bit 1 – DLPA: If this bit is set to "1",
 *   then the Downlink Packets field shall be present,
 *   otherwise the Downlink Packets field shall not be present.
 * - Bit 2 – DLBY: If this bit is set to "1",
 *   then the Number of Bytes of Downlink Data field shall be present,
 *   otherwise the Number of Bytes of Downlink Data field shall not be present.
 * - Bit 3 to 8: Spare, for future use and set to "0".
 *
 *
 * The Downlink Packets fields shall be encoded as an Unsigned64 binary
 * integer value. It shall contain a number of downlink packets.
 *
 * The Number of Bytes of Downlink Data fields shall be encoded
 * as an Unsigned64 binary integer value. It shall contain the number
 * of bytes of the downlink data.
 */
typedef struct ogs_pfcp_dropped_dl_traffic_threshold_s {
    union {
        struct {
ED3(uint8_t spare:6;,
    uint8_t dlby:1;,
    uint8_t dlpa:1;)
        };
        uint8_t flags;
    };
    uint64_t downlink_packets;
    uint64_t number_of_bytes_of_downlink_data;
} __attribute__ ((packed)) ogs_pfcp_dropped_dl_traffic_threshold_t;

int16_t ogs_pfcp_build_dropped_dl_traffic_threshold(
        ogs_tlv_octet_t *octet,
        ogs_pfcp_dropped_dl_traffic_threshold_t *threshold,
        void *data, int data_len);
int16_t ogs_pfcp_parse_dropped_dl_traffic_threshold(
        ogs_pfcp_dropped_dl_traffic_threshold_t *threshold,
        ogs_tlv_octet_t *octet);

/** 8.2.71 UR-SEQN
 *
 * The UR-SEQN (Usage Report Sequence Number) IE identifies the order
 * in which a usage report is generated for a
 * given URR. It shall be encoded as shown in Figure 8.2.71-1.
 *
 * The UR-SEQN value shall be encoded as an Unsigned32 binary integer value.
*/
typedef uint32_t ogs_pfcp_urr_ur_seqn_t;

/** 8.2.52 Start Time
 *
 * The Start Time IE indicates the time at which the UP function started
 * to collect the charging information. It shall be encoded as shown
 * in Figure 8.2.52-1.
 *
 * The Start Time field shall contain a UTC time. Octets 5 to 8 shall
 * be encoded in the same format as the first four octets
 * of the 64-bit timestamp format as defined in clause 6 of IETF RFC 5905 [12].
 *
 * NOTE: The encoding is defined as the time in seconds relative to 00:00:00
 * on 1 January 1900.
 */
typedef uint32_t ogs_pfcp_start_time_t;

/** 8.2.53 End Time
 * The End Time IE indicates the time at which the UP function ended
 * to collect the charging information. It shall be encoded as shown
 * in Figure 8.2.53-1.
 *
 * The End Time field shall contain a UTC time. Octets 5 to 8 shall be
 * encoded in the same format as the first four octets of the 64-bit timestamp
 * format as defined in clause 6 of IETF RFC 5905 [12].
 *
 * NOTE: The encoding is defined as the time in seconds relative to 00:00:00
 * on 1 January 1900.
*/
typedef uint32_t ogs_pfcp_end_time_t;

/** 8.2.45 Duration Measurement
 *
 * The Duration Measurement IE type shall be encoded as shown
 * in Figure 8.2.45-1. It contains the used time in seconds
 *
 * The Duration value shall be encoded as an Unsigned32 binary integer value.
*/
typedef uint32_t ogs_pfcp_duration_measurement_t;

/** 8.2.46 Time of First Packet
 *
 * The Time of First Packet IE indicates the time stamp for the first
 * IP packet transmitted for a given usage report. It shall be encoded
 * as shown in Figure 8.2.46-1.
 *
 * The End Time field shall contain a UTC time. Octets 5 to 8 shall
 * be encoded in the same format as the first four octets
 * of the 64-bit timestamp format as defined in clause 6 of IETF RFC 5905 [12].
 *
 * NOTE: The encoding is defined as the time in seconds relative to 00:00:00
 * on 1 January 1900.
*/
typedef uint32_t ogs_pfcp_time_of_first_packet_t;

/** 8.2.47 Time of Last Packet
 *
 * The Time of Last Packet IE indicates the time stamp for the last
 * IP packet transmitted for a given usage report. It shall be encoded
 * as shown in Figure 8.2.47-1.
 *
 * The End Time field shall contain a UTC time. Octets 5 to 8 shall
 * be encoded in the same format as the first four octets
 * of the 64-bit timestamp format as defined in clause 6 of IETF RFC 5905 [12].
 *
 * NOTE: The encoding is defined as the time in seconds relative to 00:00:00
 * on 1 January 1900.
*/
typedef uint32_t ogs_pfcp_time_of_last_packet_t;

/** 8.2.44 Volume Measurement
 *
 * The Volume Measurement IE contains the measured traffic volumes.
 * It shall be encoded as shown in Figure 8.2.44-1.
 *
 * The following flags are coded within Octet 5:
 *
 * - Bit 1 – TOVOL: If this bit is set to "1", then the Total Volume field
 *   shall be present, otherwise the Total Volume field shall not be present.
 * - Bit 2 – ULVOL: If this bit is set to "1", then the Uplink Volume field
 *   shall be present, otherwise the Uplink Volume field shall not be present.
 * - Bit 3 – DLVOL: If this bit is set to "1", then the Downlink Volume field
 *   shall be present, otherwise the Downlink Volume field shall not be present.
 * - Bit 4 – TONOP: If this bit is set to "1", then the Total Number of Packets
 *   field shall be present, otherwise the Total Number of Packets field
 *   shall not be present.
 * - Bit 5 – ULNOP: If this bit is set to "1", then the Uplink Number
 *   of Packets field shall be present, otherwise the Uplink Number
 *   of Packets field shall not be present.
 * - Bit 6 – DLNOP: If this bit is set to "1", then the Downlink Number
 *   of Packets field shall be present, otherwise the Downlink Number
 *   of Packets field shall not be present.
 * - Bit 7 to bit 8: Spare, for future use and set to "0".
 *
 * At least one bit shall be set to "1". Several bits may be set to "1".
 *
 * The Total Volume, Uplink Volume and Downlink Volume fields shall be encoded
 * as an Unsigned64 binary integer value. They shall contain the total,
 * uplink or downlink number of octets respectively
*/
typedef struct ogs_pfcp_volume_measurement_s {
    union {
        struct {
ED7(uint8_t spare:2;,
    uint8_t dlnop:1;,
    uint8_t ulnop:1;,
    uint8_t tonop:1;,
    uint8_t dlvol:1;,
    uint8_t ulvol:1;,
    uint8_t tovol:1;)
        };
    uint8_t flags;
    };

    uint64_t total_volume;
    uint64_t uplink_volume;
    uint64_t downlink_volume;
    uint64_t total_n_packets;
    uint64_t uplink_n_packets;
    uint64_t downlink_n_packets;
} __attribute__ ((packed)) ogs_pfcp_volume_measurement_t;

/*
 * 8.2.68 Measurement Information
 */
typedef struct ogs_pfcp_measurement_information_s {
    union {
        struct {
ED6(uint8_t spare:3;,
    uint8_t mnop:1;,
    uint8_t istm:1;,
    uint8_t radi:1;,
    uint8_t inam:1;,
    uint8_t mbqe:1;)
        };
    uint8_t octet5;
    };
} __attribute__ ((packed)) ogs_pfcp_measurement_information_t;

/*
 * 8.2.179 Data Status
 *
 * The following flags are coded within Octet 5:
 *
 * Bit 1 – DROP: when set to "1", this indicates first DL packet is
 * discarded by the UP function.
 * Bit 2 – BUFF: when set to "1", this indicates first DL packet is
 * received and buffered by the UP function.
 * Bit 3 to 8 Spare, for future use and set to "0".
 */
#define OGS_PFCP_DATA_STATUS_DROP                          (1<<0)
#define OGS_PFCP_DATA_STATUS_BUFF                          (1<<1)
typedef uint8_t  ogs_pfcp_data_status_t;

typedef struct ogs_pfcp_user_plane_report_s {
    ogs_pfcp_report_type_t type;
    struct {
        uint8_t pdr_id;
        uint8_t paging_policy_indication_value;
        uint8_t qfi;
    } downlink_data;
    struct {
        ogs_pfcp_urr_id_t id;
        ogs_pfcp_urr_ur_seqn_t seqn;
        ogs_pfcp_usage_report_trigger_t rep_trigger;
        ogs_pfcp_start_time_t start_time;
        ogs_pfcp_end_time_t end_time;
        ogs_pfcp_volume_measurement_t vol_measurement;
        ogs_pfcp_duration_measurement_t dur_measurement;
        ogs_pfcp_time_of_first_packet_t time_of_first_packet;
        ogs_pfcp_time_of_last_packet_t time_of_last_packet;
    } usage_report [OGS_MAX_NUM_OF_URR];
    unsigned int num_of_usage_report;
    struct {
        ogs_pfcp_f_teid_t remote_f_teid;
        int remote_f_teid_len;
    } error_indication;
} ogs_pfcp_user_plane_report_t;

int16_t ogs_pfcp_build_volume_measurement(ogs_tlv_octet_t *octet,
        ogs_pfcp_volume_measurement_t *volume, void *data, int data_len);
int16_t ogs_pfcp_parse_volume_measurement(
        ogs_pfcp_volume_measurement_t *volume, ogs_tlv_octet_t *octet);

/*
 * 8.2.101 User ID
 *
 * The User ID IE type shall be encoded as shown in Figure 8.2.101-1.
 *
 * The following flags are coded within Octet 5:
 *
 * - Bit 1 – IMSIF: If this bit is set to "1",
 *   then the Length of IMSI and IMSI fields shall be present,
 *   otherwise these fields shall not be present.
 * - Bit 2 – IMEIF: If this bit is set to "1",
 *   then the Length of IMEI and IMEI fields shall be present,
 *   otherwise these fields shall not be present.
 * - Bit 3 – MSISDNF: If this bit is set to "1",
 *   then the Length of MSISDN and MSISDN fields shall be present,
 *   otherwise these fields shall not be present.
 * - Bit 4 – NAIF: If this bit is set to "1",
 *   then the Length of NAI and NAI fields shall be present,
 *   otherwise these fields shall not be present.
 * - Bit 5 – SUPIF: If this bit is set to "1",
 *   then the Length of SUPI and SUPI fields shall be present,
 *   otherwise these fields shall not be present.
 * - Bit 6 – GPSIF: If this bit is set to "1",
 *   then the Length of GPSI and GPSI fields shall be present,
 *   otherwise these fields shall not be present.
 * - Bit 7 – PEIF: If this bit is set to "1",
 *   then the Length of PEI and PEI fields shall be present,
 *   otherwise these fields shall not be present.
 * - Bit 8: Spare, for future use and set to "0".
 *
 * One or more flags may be set to "1".
 *
 * For 5GS User Identities:
 * -The SUPI field shall only be used for carrying a Global Cable Identifier
 *  (GCI) or a Global Line Identifier (GLI). The IMSI and NAI, if received
 *  by the SMF in the SUPI, shall be included in the IMSI and NAI field respectively.
 * -The GPSI field shall only be used for carrying an External Identifier.
 *  The MSISDN, if received by the SMF in the SUPI, shall be included
 *  in the MSISDN field.
 * -The PEI field shall only be used for carrying an MAC address or
 *  an Extended Unique Identifier. The IMEI, if received by the SMF in the PEI,
 *  shall be included in the IMEI field.
 *
 * The coding of IMSI field, from octets 7 to 'a' shall be encoded
 * as the octets 5 to n+4 of the IMSI IE type
 * specified in clause 8.3 of 3GPP TS 29.274 [9].
 *
 * The coding of IMEI field, in octets 'b+1' to 'c' shall be encoded
 * as the octets 5 to n+4 of the MEI IE type
 * specified in clause 8.10 of 3GPP TS 29.274 [9].
 *
 * The coding of MSISDN field, in octets 'd+1' to 'e' shall be encoded
 * as the octets 5 to n+4 of the MSISDN IE type
 * specified in clause 8.11 of 3GPP TS 29.274 [9].
 *
 * The coding of the SUPI field, in octets 'h+1' to 'i' shall be encoded
 * as the Supi data type specified in clause 5.3.2 of 3GPP TS 29.571 [61].
 *
 * The coding of the GPSI field, in octets 'j+1' to 'k' shall be encoded
 * as the Gpsi data type specified in clause 5.3.2 of 3GPP TS 29.571 [61].
 *
 * The coding of the PEI field, in octets 'l+1' to 'm' shall be encoded
 * as the Pei data type specified in clause 5.3.2 of 3GPP TS 29.571 [61].
 *
 * The NAI field, in octets 'f+1' to 'g' shall be encoded as an Octet String
 * (see IETF RFC 4282 [36]).
 */
typedef struct ogs_pfcp_user_id_flags_s {
    union {
        struct {
ED8(uint8_t spare:1;,
    uint8_t peif:1;,
    uint8_t gpsif:1;,
    uint8_t supif:1;,
    uint8_t naif:1;,
    uint8_t msisdnf:1;,
    uint8_t imeif:1;,
    uint8_t imsif:1;)
        };
    uint8_t flags;
    };
} ogs_pfcp_user_id_flags_t;

typedef struct ogs_pfcp_user_id_s {
    union {
        struct {
ED8(uint8_t spare:1;,
    uint8_t peif:1;,
    uint8_t gpsif:1;,
    uint8_t supif:1;,
    uint8_t naif:1;,
    uint8_t msisdnf:1;,
    uint8_t imeif:1;,
    uint8_t imsif:1;)
        };
    uint8_t flags;
    };

    uint8_t imsi_len;
    uint8_t imsi[OGS_MAX_IMSI_LEN];
    uint8_t imeisv_len;
    uint8_t imeisv[OGS_MAX_IMEISV_LEN];
    uint8_t msisdn_len;
    uint8_t msisdn[OGS_MAX_MSISDN_LEN];
} ogs_pfcp_user_id_t;

int16_t ogs_pfcp_build_user_id(
        ogs_tlv_octet_t *octet, ogs_pfcp_user_id_t *user_id,
        void *data, int data_len);

/*
 * 8.2.118 3GPP Interface Type
 *
 * NOTE 1: If separation of roaming and non-roaming traffic is desired
 * this value should only be used for the S5-U interface
 * and "S8-U" (decimal 19) should be used for the S8-U interface.
 * NOTE 2: If separation of roaming and non-roaming traffic is desired
 * this value should only be used for the Gn-U interface
 * and "Gp-U" (decimal 20) should be used for the Gp-U interface.
 * NOTE 3: If separation of roaming and non-roaming traffic is desired,
 * this value should only be used for N9 non-roaming interfaces
 * and (decimal value "21") should be used for N9 roaming interfaces.
 */
#define OGS_PFCP_3GPP_INTERFACE_TYPE_S1_U       0
#define OGS_PFCP_3GPP_INTERFACE_TYPE_S5_S8_U    1  /* NOTE 1 */
#define OGS_PFCP_3GPP_INTERFACE_TYPE_S4_U       2
#define OGS_PFCP_3GPP_INTERFACE_TYPE_S11_U      3
#define OGS_PFCP_3GPP_INTERFACE_TYPE_S12        4
#define OGS_PFCP_3GPP_INTERFACE_TYPE_GN_GP_U    5  /* NOTE 2 */
#define OGS_PFCP_3GPP_INTERFACE_TYPE_S2A_U      6
#define OGS_PFCP_3GPP_INTERFACE_TYPE_S2B_U      7
#define OGS_PFCP_3GPP_INTERFACE_TYPE_ENB_GTP_U_FOR_DL_DATA_FORWARDING 8
#define OGS_PFCP_3GPP_INTERFACE_TYPE_ENB_GTP_U_FOR_UL_DATA_FORWARDING 9
#define OGS_PFCP_3GPP_INTERFACE_TYPE_SGW_UPF_GTP_U_FOR_DL_DATA_FORWARDING 10
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N3_3GPP_ACCESS 11
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N3_TRUSTED_NON_3GPP_ACCESS 12
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N3_UNTRUSTED_NON_3GPP_ACCESS 13
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N3_FOR_DATA_FORWARDING 14
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N9         15 /* NOTE 3 */
#define OGS_PFCP_3GPP_INTERFACE_TYPE_SGI        16
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N6         17
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N19        18
#define OGS_PFCP_3GPP_INTERFACE_TYPE_S8_U       19
#define OGS_PFCP_3GPP_INTERFACE_TYPE_GP_U       20
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N9_FOR_ROAMING 21
#define OGS_PFCP_3GPP_INTERFACE_TYPE_IU_U       22
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N9_FOR_DATA_FORWARDING 23
#define OGS_PFCP_3GPP_INTERFACE_TYPE_SXA_U      24
#define OGS_PFCP_3GPP_INTERFACE_TYPE_SXB_U      25
#define OGS_PFCP_3GPP_INTERFACE_TYPE_SXC_U      26
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N4_U       27
#define OGS_PFCP_3GPP_INTERFACE_TYPE_SGW_UPF_GTP_U_FOR_UL_DATA_FORWARDING 28
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N6MB_NMB9  29
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N3MB       30
#define OGS_PFCP_3GPP_INTERFACE_TYPE_N19MB      31
#define OGS_PFCP_3GPP_INTERFACE_TYPE_UNKNOWN    0xff
typedef uint8_t ogs_pfcp_3gpp_interface_type_t;

/*
 * 8.2.136 PFCPSEReq-Flags
 *
 * The following bits within Octet 5 shall indicate:
 * - Bit 1 – RESTI (Restoration Indication): if this bit is set to "1",
 *   it indicates to the UP function that the PFCP session to be established is
 *   to restore an existing PFCP session.
 * - Bit 2 – SUMPC (Stop of Usage Measurement to Pause Charging):
 *   if this bit is set to "1", it indicates that the UP function shall
 *   stop the usage measurement for all URRs with the "ASPOC" flag set to "1".
 */
typedef struct ogs_pfcp_sereq_flags_s {
    union {
        struct {
ED3(uint8_t     spare:6;,
    uint8_t     stop_of_usage_measurement_to_pause_charging:1;,
    uint8_t     restoration_indication:1;)
        };
        uint8_t value;
    };
} __attribute__ ((packed)) ogs_pfcp_sereq_flags_t;

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_TYPES_H */

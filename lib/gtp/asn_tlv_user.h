#define MAX_ASN_TYPE 372

#define ASNL_VARIABLE 0

#define ASNL_AUTHENTICATOR_IPV4     4
#define ASNL_AUTHENTICATOR_80216    6
#define ASNL_AUTHENTICATOR_IPV6     16
#define ASNL_AUTHENTICATOR_MAX      ASNL_AUTHENTICATOR_IPV6

#define ASNV_AUTH_POLICY_SUPPORT_RSA          0x01
#define ASNV_AUTH_POLICY_SUPPORT_EAP          0x02
#define ASNV_AUTH_POLICY_SUPPORT_AEAP         0x04
#define ASNV_AUTH_POLICY_SUPPORT_RE_RSA       0x10
#define ASNV_AUTH_POLICY_SUPPORT_RE_EAP       0x20
#define ASNV_AUTH_POLICY_SUPPORT_RE_AEAP      0x40

#define ASNL_BSID_IPV4  4
#define ASNL_BSID_80216 6
#define ASNL_BSID_IPV6  16
#define ASNL_BSID_MAX   ASNL_BSID_IPV6

#define ASNV_FAILURE_IND_UNSPECIFIED         0x00 /* Unspecified Error */
/* Error Codes: 0x01-0x0F Message Header Failure Codes */
#define ASNV_FAILURE_IND_PROTOCOL_VERSION    0x01 /* Protocol Version not understood (note 1) */
#define ASNV_FAILURE_IND_UNRECOG_FUNC_TYPE   0x02 /* Unrecognised Function Type */
#define ASNV_FAILURE_IND_INVALID_MSG_TYPE    0x03 /* Invalid Message Type */
#define ASNV_FAILURE_IND_UNKNOWN_MSID        0x04 /* Unknown MSID */
#define ASNV_FAILURE_IND_TRANSACTION         0x05 /* Transaction Failure */
#define ASNV_FAILURE_IND_UNKNOWN_SRC_ID      0x06 /* Source Identifier unknown or inconsistent with the IP source address */
#define ASNV_FAILURE_IND_UNKNOWN_DEST_ID     0x07 /* Destination unknown */
#define ASNV_FAILURE_IND_INVALID_HEADER      0x08 /* Invalid Message Header */
#define ASNV_FAILURE_IND_INVALID_OPID        0x09 /* Invalid OP ID */
#define ASNV_FAILURE_IND_NOORERR_DEST_ID     0x0A /* Destination Identifier missing or erroneous */
#define ASNV_FAILURE_IND_NOORERR_SRC_ID      0x0B /* Source Identifier TLV missing or erroneous */
#define ASNV_FAILURE_IND_UNKNOWN_MSG_TYPE    0x0C /* Message type unknown or inopportune */
#define ASNV_FAILURE_IND_UNRESOLVED_ERR      0x0D /* Unresolved error */
/* Error Codes: 0x10-0x1F General Message Body Failure Codes */
#define ASNV_FAILURE_IND_INVALID_MSG_FORMAT  0x10 /* Invalid message format */
#define ASNV_FAILURE_IND_NO_MANDATORY_TLV    0x11 /* Mandatory TLV missing */
#define ASNV_FAILURE_IND_INVALID_TLV_VALUE   0x12 /* TLV Value Invalid */
#define ASNV_FAILURE_IND_UNSUPPORTED_OPTION  0x13 /* Unsupported Options */
#define ASNV_FAILURE_IND_UNKNOWN_TLV         0x14 /* TLV Unknown */
#define ASNV_FAILURE_IND_UNEXPECTED_TLV      0x15 /* TLV Unexpected */
#define ASNV_FAILURE_IND_TLV_PARSE_ERROR     0x16 /* TLV parsing error */
/* Error Codes: 0x20-0x2F Message Generic Failure Codes */
#define ASNV_FAILURE_IND_TIMER_EXPIRED       0x20 /* Timer expired without response */
#define ASNV_FAILURE_IND_BSID_OUT_OF_SERVICE 0x21 /* BSID out of service */
#define ASNV_FAILURE_IND_UNKNOWN_BSID        0x22 /* Unknown BSID */
#define ASNV_FAILURE_IND_UNREACHABLE_BSID    0x23 /* BSID Unreachable */
/* Error Codes: 0x30-0x7F Message-specific Failure Codes */
#define ASNV_FAILURE_IND_UNAVAILABLE_CONTEXT 0x30 /* Requested Context Unavailable */
#define ASNV_FAILURE_IND_AUTHORIZATION       0x31 /* Authorization Failure */
#define ASNV_FAILURE_IND_REGISTRATION        0x32 /* Registration Failure */
#define ASNV_FAILURE_IND_NO_RESOURCE         0x33 /* No Resources */
#define ASNV_FAILURE_IND_REJECTION_OF_MS     0x34 /* Failure by rejection of MS */
#define ASNV_FAILURE_IND_AUTHENTICATOR_RELOC 0x35 /* Authenticator relocated */
#define ASNV_FAILURE_IND_PERIODIC_REPORT_RRM 0x36 /* Does not support periodic reporting of RRM messages */
#define ASNV_FAILURE_IND_LOCATION_UPDATE     0x37 /* Location Update Failure */
#define ASNV_FAILURE_IND_IDLE_MODE_AUTH      0x38 /* Idle Mode Authorization Failure */
#define ASNV_FAILURE_IND_UNSPPORTED_HO_TYPE  0x39 /* Target BS doesn’t support this HO Type */
#define ASNV_FAILURE_IND_BS_INSUFFICIENT_AIR 0x3A /* Insufficient Target BS airlink resource */
#define ASNV_FAILURE_IND_BS_CPU_OVERLOAD     0x3B /* Target BS CPU overload */

#define ASNL_DEST_ID_IPV4               4
#define ASNL_DEST_ID_80216              6
#define ASNL_DEST_ID_IPV6               16
#define ASNL_DEST_ID_MAX                ASNL_DEST_ID_IPV6

#define ASNL_SRC_ID_IPV4                4
#define ASNL_SRC_ID_80216               6
#define ASNL_SRC_ID_IPV6                16
#define ASNL_SRC_ID_MAX                 ASNL_SRC_ID_IPV6

#define ASNV_INITIAL_NETWORK_ENTRY              0X00000000
#define ASNV_HANDOFF                            0X00000001
#define ASNV_IN_SERVICE_DATA_PATH_ESTABLISHMENT 0X00000002
#define ASNV_MS_NETWORK_EXIT                    0X00000003
#define ASNV_IDLE_MODE_ENTRY                    0X00000004
#define ASNV_IDLE_MODE_EXIT                     0X00000005
#define ASNV_ANCHOR_DPF_RELOCATION              0X00000006

#define ASNV_CREATE_SERVICE_FLOW        0x0001
#define ASNV_ADMIT_SERVICE_FLOW         0x0002
#define ASNV_ACTIVATE_SERVICE_FLOW      0x0004
#define ASNV_MODIFY_SERVICE_FLOW        0x0008
#define ASNV_DELETE_SERVICE_FLOW        0x0010

#define ASNV_UPLINK                     0x0000
#define ASNV_DOWNLINK                   0x0001

#define ASNV_SERVING                    0x00
#define ASNV_TARGET                     0x01

#define ASNV_SUCCESSFULLY_CREATED                                   0X0000
#define ASNV_REQUEST_DENIED_NO_RESOURCES                            0X0001
#define ASNV_REQUEST_DENIED_DUE_TO_POLICY                           0X0002
#define ASNV_REQUEST_DENIED_DUE_TO_REQUESTS_FOR_OTHER_FLOWS_FAILED  0X0003
#define ASNV_REQUEST_FAILED_UNSPECIFIED_REASON                      0X0004
#define ASNV_REQUEST_DENIED_DUE_TO_MS_REASON                        0X0005

#define BUILDER_TRACE() d_trace(1, "Builder: %s()\n", __func__)
#define PARSER_TRACE() d_trace(1, "Parser: %s()\n", __func__)
#if 1
#define TLV_TRACE(__t, __l, __v) \
        d_trace_hex(1, __v, __l)

#define PAYLOAD_TRACE(__b, __l) \
        d_trace(1, "Built payload: "); \
        d_trace_hex(1, __b, __l);
#else
#define TLV_TRACE(__t, __l, __v)
#define PAYLOAD_TRACE(__b, __l)
#endif

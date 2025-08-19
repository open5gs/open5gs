#include "ogs-core.h"      /* ogs_time_now, OGS_INET_NTOP, OGS_INET6_NTOP, OGS_ADDRSTRLEN */
#include "context.h"       /* smf_self(), smf_ue_t, smf_sess_t */
#include "connected_ues.h"


static inline size_t json_append(char *buf, size_t off, size_t buflen,
                                 const char *fmt, ...)
{
    if (!buf || buflen == 0) return 0;
    if (off >= buflen) return buflen - 1;

    size_t rem = buflen - off;
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf + off, rem, fmt, ap);
    va_end(ap);

    if (n < 0) return off;                
    if ((size_t)n >= rem) return buflen - 1; 
    return off + (size_t)n;
}

size_t smf_dump_connected_ues(char *buf, size_t buflen)
{
    if (!buf || buflen < 3) return 0; /* need at least "[]" and NUL */
    buf[0] = '\0';

    size_t off = 0;
    off = json_append(buf, off, buflen, "[");
    int first_ue = 1;

    smf_ue_t *ue = NULL;
    ogs_list_for_each(&smf_self()->smf_ue_list, ue) {
        if (!ue) continue;

        if (!first_ue) off = json_append(buf, off, buflen, ",");
        first_ue = 0;

        off = json_append(buf, off, buflen, "{");

        const char *id = ue->supi ? ue->supi : (ue->imsi_bcd ? ue->imsi_bcd : "");
        off = json_append(buf, off, buflen, "\"supi\":\"%s\",", id);

        off = json_append(buf, off, buflen, "\"pdu\":[");
        int first_pdu = 1;

        smf_sess_t *sess = NULL;
        ogs_list_for_each(&ue->sess_list, sess) {
            if (!sess) continue;

            const unsigned psi = sess->psi;
            const char *dnn = (sess->session.name ? sess->session.name : "");

            /* IPv4 */
            if (sess->ipv4) {
                char ip4[OGS_ADDRSTRLEN] = "";
                OGS_INET_NTOP(&sess->ipv4->addr, ip4);
                if (!first_pdu) off = json_append(buf, off, buflen, ",");
                first_pdu = 0;
                off = json_append(buf, off, buflen,
                    "{\"psi\":%u,\"dnn\":\"%s\",\"ip\":\"%s\",\"ptype\":\"ipv4\"}",
                    psi, dnn, ip4);
            }

            /* IPv6 */
            if (sess->ipv6) {
                char ip6[OGS_ADDRSTRLEN] = "";
                OGS_INET6_NTOP(&sess->ipv6->addr, ip6);
                if (!first_pdu) off = json_append(buf, off, buflen, ",");
                first_pdu = 0;
                off = json_append(buf, off, buflen,
                    "{\"psi\":%u,\"dnn\":\"%s\",\"ip\":\"%s\",\"ptype\":\"ipv6\"}",
                    psi, dnn, ip6);
            }

            /* no IP yet */
            if (!sess->ipv4 && !sess->ipv6) {
                if (!first_pdu) off = json_append(buf, off, buflen, ",");
                first_pdu = 0;
                off = json_append(buf, off, buflen,
                    "{\"psi\":%u,\"dnn\":\"%s\",\"ip\":\"\",\"ptype\":\"unknown\"}",
                    psi, dnn);
            }

            /* If close to the end, stop this UE gracefully (JSON stays valid). */
            if (off >= buflen - 256) break;
        }

        off = json_append(buf, off, buflen, "]");
        long long now_s = (long long)(ogs_time_now() / 1000000LL);
        off = json_append(buf, off, buflen, ",\"last_seen\":%lld", now_s);
        off = json_append(buf, off, buflen, "}");

        if (off >= buflen - 256) break; 
    }

    off = json_append(buf, off, buflen, "]");
    return off;
}


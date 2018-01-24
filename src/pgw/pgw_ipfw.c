#define TRACE_MODULE _pgw_ipfw

#include "core_network.h"
#include "pgw_ipfw.h"

#include "ipfw/ipfw2.h"
#include "ipfw/objs/include_e/netinet/ip_fw.h"

#define MAX_NUM_OF_TOKEN 32
#define MAX_NUM_OF_RULE_BUFFER 1024

void compile_rule(char *av[], uint32_t *rbuf, int *rbufsize, void *tstate);

status_t pgw_compile_packet_filter(pgw_rule_t *pgw_rule, c_int8_t *description)
{
    pgw_rule_t zero_rule;
    char *token, *dir;
    char *saveptr;
    int i = 2;

    char *av[MAX_NUM_OF_TOKEN];
	uint32_t rulebuf[MAX_NUM_OF_RULE_BUFFER];
	int rbufsize;
	struct ip_fw_rule *rule = (struct ip_fw_rule *)rulebuf;

	int l;
	ipfw_insn *cmd;

    d_assert(pgw_rule, return CORE_ERROR, "Null param");
    d_assert(description, return CORE_ERROR, "Null param");

	rbufsize = sizeof(rulebuf);
	memset(rulebuf, 0, rbufsize);

    av[0] = NULL;

    /* ACTION */
    token = strtok_r(description, " ", &saveptr);
    if (strcmp(token, "permit") != 0)
    {
        d_error("Not begins with reserved keyword : 'permit'");
        return CORE_ERROR;
    }
    av[1] = token;

    /* Save DIRECTION */
    dir = token = strtok_r(NULL, " ", &saveptr);
    if (strcmp(token, "out") != 0)
    {
        d_error("Not begins with reserved keyword : 'permit out'");
        return CORE_ERROR;
    }

    /* ADDR */
    token = strtok_r(NULL, " ", &saveptr);
    while(token != NULL)
    {
        av[i++] = token;
        token = strtok_r(NULL, " ", &saveptr);
    }

    /* Add DIRECTION */
    av[i++] = dir;

    av[i] = NULL;

	compile_rule(av, (uint32_t *)rule, &rbufsize, NULL);

    memset(pgw_rule, 0, sizeof(pgw_rule_t));
	for (l = rule->act_ofs, cmd = rule->cmd;
			l > 0 ; l -= F_LEN(cmd) , cmd += F_LEN(cmd))
    {
		switch(cmd->opcode) 
        {
            case O_PROTO:
                pgw_rule->proto = cmd->arg1;
                break;
            case O_IP_SRC:
            case O_IP_SRC_MASK:
            {
                uint32_t *a = ((ipfw_insn_u32 *)cmd)->d;
                pgw_rule->ipv4_local = 1;
                pgw_rule->ip.local.addr[0] = a[0];
                if (cmd->opcode == O_IP_SRC_MASK)
                    pgw_rule->ip.local.mask[0] = a[1];
                else
                    pgw_rule->ip.local.mask[0] = 0xffffffff;
                break;
            }
            case O_IP_DST:
            case O_IP_DST_MASK:
            {
                uint32_t *a = ((ipfw_insn_u32 *)cmd)->d;
                pgw_rule->ipv4_remote = 1;
                pgw_rule->ip.remote.addr[0] = a[0];
                if (cmd->opcode == O_IP_DST_MASK)
                    pgw_rule->ip.remote.mask[0] = a[1];
                else
                    pgw_rule->ip.remote.mask[0] = 0xffffffff;
                break;
            }
            case O_IP6_SRC:
            case O_IP6_SRC_MASK:
            {
                uint32_t *a = ((ipfw_insn_u32 *)cmd)->d;
                pgw_rule->ipv6_local = 1;
                memcpy(pgw_rule->ip.local.addr, a, IPV6_LEN);
                if (cmd->opcode == O_IP6_SRC_MASK)
                    memcpy(pgw_rule->ip.local.mask, a+4, IPV6_LEN);
                else
                    n2mask((struct in6_addr *)pgw_rule->ip.local.mask, 128);
                break;
            }
            case O_IP6_DST:
            case O_IP6_DST_MASK:
            {
                uint32_t *a = ((ipfw_insn_u32 *)cmd)->d;
                pgw_rule->ipv6_remote = 1;
                memcpy(pgw_rule->ip.remote.addr, a, IPV6_LEN);
                if (cmd->opcode == O_IP6_DST_MASK)
                    memcpy(pgw_rule->ip.remote.mask, a+4, IPV6_LEN);
                else
                    n2mask((struct in6_addr *)pgw_rule->ip.remote.mask, 128);
                break;
            }
            case O_IP_SRCPORT:
            {
                uint16_t *p = ((ipfw_insn_u16 *)cmd)->ports;
                pgw_rule->port.local.low = p[0];
                pgw_rule->port.local.high = p[1];
                break;
            }
            case O_IP_DSTPORT:
            {
                uint16_t *p = ((ipfw_insn_u16 *)cmd)->ports;
                pgw_rule->port.remote.low = p[0];
                pgw_rule->port.remote.high = p[1];
                break;
            }
        }
	}

    memset(&zero_rule, 0, sizeof(pgw_rule_t));
    if (memcmp(pgw_rule, &zero_rule, sizeof(pgw_rule_t)) == 0)
    {
        d_error("Cannot find Flow-Description");
        return CORE_ERROR;
    }

    return CORE_OK;
}

static status_t decode_ipv6_header(
        struct ip6_hdr *ip6_h, c_uint8_t *proto, c_uint16_t *hlen)
{
    int done = 0;
    c_uint8_t *p, *jp, *endp;
    c_uint8_t nxt;          /* Next Header */

    d_assert(ip6_h, return CORE_ERROR,);
    d_assert(proto, return CORE_ERROR,);
    d_assert(hlen, return CORE_ERROR,);

    nxt = ip6_h->ip6_nxt;
    p = (c_uint8_t *)ip6_h + sizeof(*ip6_h);
    endp = p + ntohs(ip6_h->ip6_plen);

    jp = p + sizeof(struct ip6_hbh);
    while(p == endp) /* Jumbo Frame */
    {
        c_uint32_t jp_len = 0;
        struct ip6_opt_jumbo *jumbo = NULL;

        d_assert(nxt == 0, return CORE_ERROR,);

        jumbo = (struct ip6_opt_jumbo *)jp;
        memcpy(&jp_len, jumbo->ip6oj_jumbo_len, sizeof(jp_len));
        jp_len = ntohl(jp_len);
        switch(jumbo->ip6oj_type)
        {
            case IP6OPT_JUMBO:
                endp = p + jp_len;
                break;
            case 0:
                jp++;
                break;
            default:
                jp += (sizeof(struct ip6_opt) + jp_len);
                break;
        }
    }

    while(p < endp)
    {
        struct ip6_ext *ext = (struct ip6_ext *)p;
        switch(nxt)
        {
            case IPPROTO_HOPOPTS:
            case IPPROTO_ROUTING:
            case IPPROTO_DSTOPTS:
            case 135: /* mobility */
            case 139: /* host identity, experimental */
            case 140: /* shim6 */
            case 253: /* testing, experimental */
            case 254: /* testing, experimental */
                p += ((ext->ip6e_len << 3) + 8);
                break;
            case IPPROTO_FRAGMENT:
                p += sizeof(struct ip6_frag);
                break;
            case IPPROTO_AH:
                p += ((ext->ip6e_len + 2) << 2);
                break;
            default: /* Upper Layer */
                done = 1;
                break;     

        }
        if (done)
            break;

        nxt = ext->ip6e_nxt;
    }

    *proto = nxt;
    *hlen = p - (c_uint8_t *)ip6_h;

    return CORE_OK;
}

pgw_bearer_t *pgw_bearer_find_by_packet(pkbuf_t *pkt)
{
    hash_index_t *hi = NULL;
    struct ip *ip_h =  NULL;
    struct ip6_hdr *ip6_h =  NULL;
    c_uint32_t *src_addr = NULL;
    c_uint32_t *dst_addr = NULL;
    int addr_len = 0;
    c_uint8_t proto = 0;
    c_uint16_t ip_hlen = 0;
    char buf[CORE_ADDRSTRLEN];

    d_assert(pkt, return NULL, "pkt is NULL");
    d_assert(pkt->payload, return NULL, "pkt is NULL");

    ip_h = (struct ip *)pkt->payload;
    if (ip_h->ip_v == 4)
    {
        ip_h = (struct ip *)pkt->payload;
        ip6_h = NULL;

        proto = ip_h->ip_p;
        ip_hlen = (ip_h->ip_hl)*4;

        src_addr = &ip_h->ip_src.s_addr;
        dst_addr = &ip_h->ip_dst.s_addr;
        addr_len = 4;
    }
    else if (ip_h->ip_v == 6)
    {
        ip_h = NULL;
        ip6_h = (struct ip6_hdr *)pkt->payload;

        decode_ipv6_header(ip6_h, &proto, &ip_hlen);

        src_addr = (c_uint32_t *)ip6_h->ip6_src.s6_addr;
        dst_addr = (c_uint32_t *)ip6_h->ip6_dst.s6_addr;
        addr_len = 16;

    }
    else
        d_error("Invalid IP version = %d\n", ip_h->ip_v);

    d_trace(5, "[PGW] PROTO:%d SRC:%08x %08x %08x %08x\n",
            proto, ntohl(src_addr[0]), ntohl(src_addr[1]),
            ntohl(src_addr[2]), ntohl(src_addr[3]));
    d_trace(5, "[PGW] HLEN:%d  DST:%08x %08x %08x %08x\n",
            ip_hlen, ntohl(dst_addr[0]), ntohl(dst_addr[1]),
            ntohl(dst_addr[2]), ntohl(dst_addr[3]));


    /* TODO: Need to use the method of FAST matching algorithm and 
     *          implementation .
     *       Until be ready, linear searching will be use to find the bearer.
     */

    for (hi = pgw_sess_first(); hi; hi = pgw_sess_next(hi))
    {
        pgw_sess_t *sess = pgw_sess_this(hi);
        d_assert(sess, return NULL,);

        if (sess->ipv4)
            d_trace(5, "[PGW] PAA IPv4:%s\n",
                    INET_NTOP(&sess->ipv4->addr, buf));
        if (sess->ipv6)
            d_trace(5, "[PGW] PAA IPv6:%s\n",
                    INET6_NTOP(&sess->ipv6->addr, buf));

        if ((sess->ipv4 && memcmp(dst_addr, sess->ipv4->addr, addr_len) == 0) ||
            (sess->ipv6 && memcmp(dst_addr, sess->ipv6->addr, addr_len) == 0))
        {
            pgw_bearer_t *default_bearer = NULL;
            pgw_bearer_t *bearer = NULL;

            /* Save the default bearer */
            default_bearer = pgw_default_bearer_in_sess(sess);
            d_assert(default_bearer, return NULL, "No default Bearer");

            /* Found */
            d_trace(5, "[PGW] Found Session : EBI[%d]\n", default_bearer->ebi);

            bearer = pgw_bearer_next(default_bearer);
            /* Find the bearer with matched */
            for (; bearer; bearer = pgw_bearer_next(bearer))
            {
                pgw_pf_t *pf = NULL;

                if (bearer->ebi == 0)
                {
                    /* Create Bearer Response is not received */
                    continue;
                }

                for (pf = pgw_pf_first(bearer); pf; pf = pgw_pf_next(pf))
                {
                    int k;
                    c_uint32_t src_mask[4];
                    c_uint32_t dst_mask[4];

                    d_trace(5, "DIR:%d PROTO:%d SRC:%d-%d DST:%d-%d\n",
                            pf->direction, pf->rule.proto,
                            pf->rule.port.local.low,
                            pf->rule.port.local.high,
                            pf->rule.port.remote.low,
                            pf->rule.port.remote.high);
                    d_trace(5, "SRC:%08x %08x %08x %08x/%08x %08x %08x %08x\n",
                            ntohl(pf->rule.ip.local.addr[0]),
                            ntohl(pf->rule.ip.local.addr[1]),
                            ntohl(pf->rule.ip.local.addr[2]),
                            ntohl(pf->rule.ip.local.addr[3]),
                            ntohl(pf->rule.ip.local.mask[0]),
                            ntohl(pf->rule.ip.local.mask[1]),
                            ntohl(pf->rule.ip.local.mask[2]),
                            ntohl(pf->rule.ip.local.mask[3]));
                    d_trace(5, "DST:%08x %08x %08x %08x/%08x %08x %08x %08x\n",
                            ntohl(pf->rule.ip.remote.addr[0]),
                            ntohl(pf->rule.ip.remote.addr[1]),
                            ntohl(pf->rule.ip.remote.addr[2]),
                            ntohl(pf->rule.ip.remote.addr[3]),
                            ntohl(pf->rule.ip.remote.mask[0]),
                            ntohl(pf->rule.ip.remote.mask[1]),
                            ntohl(pf->rule.ip.remote.mask[2]),
                            ntohl(pf->rule.ip.remote.mask[3]));

                    if (pf->direction != 1)
                    {
                        continue;
                    }

                    for (k = 0; k < 4; k++)
                    {
                        src_mask[k] = src_addr[k] & pf->rule.ip.local.mask[k];
                        dst_mask[k] = dst_addr[k] & pf->rule.ip.remote.mask[k];
                    }

                    if (memcmp(src_mask, pf->rule.ip.local.addr,
                                addr_len) == 0 &&
                        memcmp(dst_mask, pf->rule.ip.remote.addr,
                                addr_len) == 0)
                    {
                        /* Protocol match */
                        if (pf->rule.proto == 0) /* IP */
                        {
                            /* No need to match port */
                            break;
                        }

                        if (pf->rule.proto == proto)
                        {
                            if (pf->rule.proto == IPPROTO_TCP)
                            {
                                struct tcphdr *tcph = 
                                    (struct tcphdr *)
                                    ((char *)pkt->payload + ip_hlen);

                                /* Source port */
                                if (pf->rule.port.local.low && 
                                      ntohs(tcph->th_sport) < 
                                              pf->rule.port.local.low)
                                {
                                    continue;
                                }

                                if (pf->rule.port.local.high && 
                                      ntohs(tcph->th_sport) > 
                                              pf->rule.port.local.high)
                                {
                                    continue;
                                }

                                /* Dst Port*/
                                if (pf->rule.port.remote.low && 
                                      ntohs(tcph->th_dport) < 
                                              pf->rule.port.remote.low)
                                {
                                    continue;
                                }

                                if (pf->rule.port.remote.high && 
                                      ntohs(tcph->th_dport) > 
                                              pf->rule.port.remote.high)
                                {
                                    continue;
                                }

                                /* Matched */
                                break;
                            }
                            else if (pf->rule.proto == IPPROTO_UDP)
                            {
                                struct udphdr *udph = 
                                    (struct udphdr *)
                                    ((char *)pkt->payload + ip_hlen);

                                /* Source port */
                                if (pf->rule.port.local.low && 
                                      ntohs(udph->uh_sport) < 
                                              pf->rule.port.local.low)
                                {
                                    continue;
                                }

                                if (pf->rule.port.local.high && 
                                      ntohs(udph->uh_sport) > 
                                              pf->rule.port.local.high)
                                {
                                    continue;
                                }

                                /* Dst Port*/
                                if (pf->rule.port.remote.low && 
                                      ntohs(udph->uh_dport) < 
                                              pf->rule.port.remote.low)
                                {
                                    continue;
                                }

                                if (pf->rule.port.remote.high && 
                                      ntohs(udph->uh_dport) > 
                                              pf->rule.port.remote.high)
                                {
                                    continue;
                                }

                                /* Matched */
                                break;
                            }
                            else
                            {
                                /* No need to match port */
                                break;
                            }

                        }
                    }

                }

                if (pf)
                {
                    bearer = pf->bearer;
                    d_trace(5, "Found Dedicated Bearer : EBI[%d]\n", bearer->ebi);
                    break;
                }

            }

            return (bearer ? bearer : default_bearer);
        }
    }

    return NULL;
}


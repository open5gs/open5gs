#include <stdint.h>
#include <string.h>

#include "fuzzing.h"
#include "ogs-pfcp.h"

#define kMinInputLength 1
#define kMaxInputLength 2048

static void initialize_rule(ogs_pfcp_rule_t *rule, int family,
        uint8_t proto, const char *src, const char *dst)
{
    ogs_ipfw_rule_t *ipfw = &rule->ipfw;

    ipfw->proto = proto;
    ogs_assert(inet_pton(family, src, ipfw->ip.src.addr) == 1);
    ogs_assert(inet_pton(family, dst, ipfw->ip.dst.addr) == 1);

    if (family == AF_INET) {
        ipfw->ipv4_src = ipfw->ipv4_dst = 1;
        ipfw->ip.src.mask[0] = ipfw->ip.dst.mask[0] = htobe32(0xffffff00);
    } else {
        ipfw->ipv6_src = ipfw->ipv6_dst = 1;
        ipfw->ip.src.mask[0] = ipfw->ip.dst.mask[0] = htobe32(0xffffffff);
        ipfw->ip.src.mask[1] = ipfw->ip.dst.mask[1] = htobe32(0xffffffff);
    }
}

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    ogs_pfcp_pdr_t pdr = {0};
    ogs_pfcp_rule_t rules[4] = {0};
    ogs_pkbuf_t *pkbuf;
    size_t index;

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 0;
    }

    if (!initialized) {
        initialize();
        ogs_log_set_mask_level("core", OGS_LOG_NONE);
        ogs_log_install_domain(&__ogs_pfcp_domain, "pfcp", OGS_LOG_NONE);
    }

    initialize_rule(&rules[0], AF_INET, IPPROTO_TCP,
            "192.0.2.0", "198.51.100.0");
    rules[0].ipfw.port.src.low = 10000;
    rules[0].ipfw.port.src.high = 20000;
    rules[0].ipfw.port.dst.low = rules[0].ipfw.port.dst.high = 443;

    initialize_rule(&rules[1], AF_INET, IPPROTO_UDP,
            "192.0.2.0", "198.51.100.0");
    rules[1].ipfw.port.src.low = 1024;
    rules[1].ipfw.port.src.high = 65535;
    rules[1].ipfw.port.dst.low = rules[1].ipfw.port.dst.high = 53;

    initialize_rule(&rules[2], AF_INET6, IPPROTO_TCP,
            "2001:db8:1::", "2001:db8:2::");
    rules[2].ipfw.port.src.low = 10000;
    rules[2].ipfw.port.src.high = 20000;
    rules[2].ipfw.port.dst.low = rules[2].ipfw.port.dst.high = 443;

    initialize_rule(&rules[3], AF_INET6, IPPROTO_ICMPV6,
            "2001:db8:1::", "2001:db8:2::");

    ogs_list_init(&pdr.rule_list);
    for (index = 0; index < OGS_ARRAY_SIZE(rules); index++) {
        rules[index].pdr = &pdr;
        ogs_list_add(&pdr.rule_list, &rules[index]);
    }

    pkbuf = ogs_pkbuf_alloc(NULL, Size);
    if (pkbuf == NULL) {
        return 0;
    }
    ogs_pkbuf_put_data(pkbuf, Data, Size);

    ogs_pfcp_pdr_rule_find_by_packet(&pdr, pkbuf);

    ogs_pkbuf_free(pkbuf);
    return 0;
}
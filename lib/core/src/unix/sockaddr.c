#define TRACE_MODULE _sockaddr

#include "core_debug.h"
#include "core_lib.h"
#include "core_pkbuf.h"

#include "core_arch_network.h"

status_t core_getaddrinfo(c_sockaddr_t **sa_list, 
        int family, const char *hostname, c_uint16_t port, int flags)
{
    *sa_list = NULL;
    return core_addaddrinfo(sa_list, family, hostname, port, flags);
}

status_t core_freeaddrinfo(c_sockaddr_t *sa_list)
{
    c_sockaddr_t *next = NULL, *addr = NULL;

    addr = sa_list;
    while(addr)
    {
        next = addr->next;
        CORE_FREE(addr);
        addr = next;
    }

    return CORE_OK;
}

status_t core_addaddrinfo(c_sockaddr_t **sa_list, 
        int family, const char *hostname, c_uint16_t port, int flags)
{
    int rc;
    char service[NI_MAXSERV];
    struct addrinfo hints, *ai, *ai_list;
    c_sockaddr_t *prev;
    char buf[CORE_ADDRSTRLEN];

    d_assert(sa_list, return CORE_ERROR,);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = flags;

    snprintf(service, sizeof(service), "%u", port);

    rc = getaddrinfo(hostname, service, &hints, &ai_list);
    if (rc != 0)
    {
        d_error("getaddrinfo(%d:%s:%d:0x%x) failed(%d:%s)",
                family, hostname, port, flags, errno, strerror(errno));
        return CORE_ERROR;
    }

    prev = NULL;
    if (*sa_list)
    {
        prev = *sa_list;
        while(prev->next) prev = prev->next;
    }
    for (ai = ai_list; ai; ai = ai->ai_next)
    {
        c_sockaddr_t *new;
        if (ai->ai_family != AF_INET && ai->ai_family != AF_INET6)
            continue;

        new = core_calloc(1, sizeof(c_sockaddr_t));
        memcpy(&new->sa, ai->ai_addr, ai->ai_addrlen);
        new->c_sa_port = htons(port);
        d_trace(3, "addr:%s, port:%d\n", CORE_ADDR(new, buf), port);

        if (!prev)
            *sa_list = new;
        else
            prev->next = new;

        prev = new;
    }

    freeaddrinfo(ai_list);

    if (prev == NULL)
    {
        d_error("core_getaddrinfo(%d:%s:%d:%d) failed(%d:%s)",
                family, hostname, port, flags, errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t core_filteraddrinfo(c_sockaddr_t **sa_list, int family)
{
    c_sockaddr_t *addr = NULL, *prev = NULL, *next = NULL;

    d_assert(sa_list, return CORE_ERROR,);

    prev = NULL;
    addr = *sa_list;
    while(addr)
    {
        next = addr->next;

        if (addr->c_sa_family != family)
        {
            if (prev)
                prev->next = addr->next;
            else
                *sa_list = addr->next;
            CORE_FREE(addr);

        }
        else
        {
            prev = addr;
        }

        addr = next;
    }

    return CORE_OK;
}

status_t core_copyaddrinfo(c_sockaddr_t **dst, const c_sockaddr_t *src)
{
    c_sockaddr_t *d;
    const c_sockaddr_t *s;

    for (*dst = d = NULL, s = src; s; s = s->next)
    {
        if (!d)
        {
            d = core_calloc(1, sizeof *s);
            *dst = memcpy(d, s, sizeof *s);
        }
        else
        {
            d->next = core_calloc(1, sizeof(c_sockaddr_t));
            d = memcpy(d->next, s, sizeof *s);
        }
    }
    return CORE_OK;
}

status_t core_sortaddrinfo(c_sockaddr_t **sa_list, int family)
{
    c_sockaddr_t *head = NULL, *addr = NULL, *new = NULL, *old = NULL;

    d_assert(sa_list, return CORE_ERROR,);

    old = *sa_list;
    while(old)
    {
        addr = old;

        old = old->next;

        if (head == NULL || addr->c_sa_family == family)
        {
            addr->next = head;
            head = addr;
        }
        else
        {
            new = head;
            while(new->next != NULL && new->next->c_sa_family != family)
            {
                new = new->next;
            }
            addr->next = new->next;
            new->next = addr;
        }
    }
    
    *sa_list = head;

    return CORE_OK;
}

c_sockaddr_t *core_link_local_addr_by_dev(const char *dev)
{
	struct ifaddrs *iflist, *cur;
    int rc;

    d_assert(dev, return NULL,);

	rc = getifaddrs(&iflist);
    if (rc != 0)
    {
        d_error("getifaddrs failed(%d:%s)", errno, strerror(errno));
        return NULL;
    }

	for (cur = iflist; cur != NULL; cur = cur->ifa_next)
    {
        c_sockaddr_t *addr = NULL;

		if (cur->ifa_addr == NULL) /* may happen with ppp interfaces */
			continue;

        if (strcmp(dev, cur->ifa_name) != 0)
            continue;

        if (cur->ifa_addr->sa_family == AF_INET)
            continue;

        addr = (c_sockaddr_t *)cur->ifa_addr;
        if (!IN6_IS_ADDR_LINKLOCAL(&addr->sin6.sin6_addr)) 
            continue;

        addr = core_calloc(1, sizeof(c_sockaddr_t));
        d_assert(addr, return NULL,);
        memcpy(&addr->sa, cur->ifa_addr, sockaddr_len(cur->ifa_addr));

        freeifaddrs(iflist);
        return addr;
	}

	freeifaddrs(iflist);
    return NULL;
}

const char *core_inet_ntop(void *sa, char *buf, int buflen)
{
    int family;
    c_sockaddr_t *sockaddr = NULL;

    d_assert(buf, return NULL,);
    sockaddr = sa;
    d_assert(sockaddr, return NULL,);

    family = sockaddr->c_sa_family;
    switch(family)
    {
        case AF_INET:
            d_assert(buflen >= INET_ADDRSTRLEN, return NULL,);
            return inet_ntop(family,
                    &sockaddr->sin.sin_addr, buf, INET_ADDRSTRLEN);
        case AF_INET6:
            d_assert(buflen >= CORE_ADDRSTRLEN, return NULL,);
            return inet_ntop(family,
                    &sockaddr->sin6.sin6_addr, buf, INET6_ADDRSTRLEN);
        default:
            d_assert(0, return NULL, "Unknown family(%d)", family);
    }
}

status_t core_inet_pton(int family, const char *src, void *sa)
{
    c_sockaddr_t *dst = NULL;

    d_assert(src, return CORE_ERROR,);
    dst = sa;
    d_assert(dst, return CORE_ERROR,);

    dst->c_sa_family = family;
    switch(family)
    {
        case AF_INET:
            return inet_pton(family, src, &dst->sin.sin_addr) == 1 ?
                CORE_OK : CORE_ERROR;
        case AF_INET6:
            return inet_pton(family, src, &dst->sin6.sin6_addr) == 1 ?
                 CORE_OK : CORE_ERROR;
        default:
            d_assert(0, return CORE_ERROR, "Unknown family(%d)", family);
    }
}

socklen_t sockaddr_len(const void *sa)
{
    const c_sockaddr_t *sockaddr = sa;

    d_assert(sa, return 0,);

    switch(sockaddr->c_sa_family)
    {
        case AF_INET:
            return sizeof(struct sockaddr_in);
        case AF_INET6:
            return sizeof(struct sockaddr_in6);
        default:
            d_assert(0, return 0, "Unknown family(%d)", sockaddr->c_sa_family);
    }
}

int sockaddr_is_equal(void *p, void *q)
{
    c_sockaddr_t *a, *b;

    a = p;
    d_assert(a, return 0,);
    b = q;
    d_assert(b, return 0,);

    if (a->c_sa_family != b->c_sa_family)
        return 0;

    if (a->c_sa_family == AF_INET && memcmp(
        &a->sin.sin_addr, &b->sin.sin_addr, sizeof(struct in_addr)) == 0)
        return 1;
    else if (a->c_sa_family == AF_INET6 && memcmp(
        &a->sin6.sin6_addr, &b->sin6.sin6_addr, sizeof(struct in6_addr)) == 0)
        return 1;
    else
        d_assert(0, return 0, "Unknown family(%d)", a->c_sa_family);

    return 0;
}

static status_t parse_network(ipsubnet_t *ipsub, const char *network)
{
    /* legacy syntax for ip addrs: a.b.c. ==> a.b.c.0/24 for example */
    int shift;
    char *s, *t;
    int octet;
    char buf[sizeof "255.255.255.255"];

    if (strlen(network) < sizeof buf)
    {
        strcpy(buf, network);
    }
    else
    {
        return CORE_EBADIP;
    }

    /* parse components */
    s = buf;
    ipsub->sub[0] = 0;
    ipsub->mask[0] = 0;
    shift = 24;
    while (*s)
    {
        t = s;
        if (!c_isdigit(*t))
        {
            return CORE_EBADIP;
        }
        while (c_isdigit(*t))
        {
            ++t;
        }
        if (*t == '.')
        {
            *t++ = 0;
        }
        else if (*t)
        {
            return CORE_EBADIP;
        }
        if (shift < 0)
        {
            return CORE_EBADIP;
        }
        octet = atoi(s);
        if (octet < 0 || octet > 255)
        {
            return CORE_EBADIP;
        }
        ipsub->sub[0] |= octet << shift;
        ipsub->mask[0] |= 0xFFUL << shift;
        s = t;
        shift -= 8;
    }
    ipsub->sub[0] = ntohl(ipsub->sub[0]);
    ipsub->mask[0] = ntohl(ipsub->mask[0]);
    ipsub->family = AF_INET;
    return CORE_OK;
}

/* return values:
 * CORE_EINVAL     not an IP address; caller should see
 *                 if it is something else
 * CORE_BADIP      IP address portion is is not valid
 * CORE_BADMASK    mask portion is not valid
 */
static status_t parse_ip(
        ipsubnet_t *ipsub, const char *ipstr, int network_allowed)
{
    /* supported flavors of IP:
     *
     * . IPv6 numeric address string (e.g., "fe80::1")
     * 
     *   IMPORTANT: Don't store IPv4-mapped IPv6 address as an IPv6 address.
     *
     * . IPv4 numeric address string (e.g., "127.0.0.1")
     *
     * . IPv4 network string (e.g., "9.67")
     *
     *   IMPORTANT: This network form is only allowed if network_allowed is on.
     */
    int rc;

    rc = inet_pton(AF_INET6, ipstr, ipsub->sub);
    if (rc == 1)
    {
        if (IN6_IS_ADDR_V4MAPPED((struct in6_addr *)ipsub->sub))
        {
            /* ipsubnet_test() assumes that we don't create IPv4-mapped IPv6
             * addresses; this of course forces the user to specify 
             * IPv4 addresses in a.b.c.d style instead of ::ffff:a.b.c.d style.
             */
            d_error("Cannot support IPv4-mapped IPv6: "
                    "Use IPv4 address in a.b.c.d style "
                    "instead of ::ffff:a.b.c.d style");
            return CORE_EBADIP;
        }
        ipsub->family = AF_INET6;
    }
    else
    {
        rc = inet_pton(AF_INET, ipstr, ipsub->sub);
        if (rc == 1)
        {
            ipsub->family = AF_INET;
        }
    }
    if (rc != 1)
    {
        if (network_allowed)
        {
            return parse_network(ipsub, ipstr);
        }
        else
        {
            return CORE_EBADIP;
        }
    }
    return CORE_OK;
}

static int looks_like_ip(const char *ipstr)
{
    if (strlen(ipstr) == 0)
    {
        return 0;
    }
    
    if (strchr(ipstr, ':'))
    {
        /* definitely not a hostname;
         * assume it is intended to be an IPv6 address */
        return 1;
    }

    /* simple IPv4 address string check */
    while ((*ipstr == '.') || c_isdigit(*ipstr))
        ipstr++;
    return (*ipstr == '\0');
}

static void fix_subnet(ipsubnet_t *ipsub)
{
    /* in case caller specified more bits in network address than are
     * valid according to the mask, turn off the extra bits
     */
    int i;

    for (i = 0; i < sizeof ipsub->mask / sizeof(c_int32_t); i++)
    {
        ipsub->sub[i] &= ipsub->mask[i];
    }
}

/* be sure not to store any IPv4 address as a v4-mapped IPv6 address */
CORE_DECLARE(status_t) core_ipsubnet(
        ipsubnet_t *ipsub, const char *ipstr, const char *mask_or_numbits)
{
    status_t rv;
    char *endptr;
    long bits, maxbits = 32;

    d_assert(ipsub, return CORE_ERROR,);
    d_assert(ipstr, return CORE_ERROR,);

    /* filter out stuff which doesn't look remotely like an IP address;
     * this helps callers like mod_access which have a syntax allowing 
     * hostname or IP address;
     * CORE_EINVAL tells the caller that it was probably not intended 
     * to be an IP address
     */
    if (!looks_like_ip(ipstr))
    {
        d_error("looks_like_ip() is failed");
        return CORE_EINVAL;
    }

    /* assume ipstr is an individual IP address, not a subnet */
    memset(ipsub->mask, 0xFF, sizeof ipsub->mask);

    rv = parse_ip(ipsub, ipstr, mask_or_numbits == NULL);
    if (rv != CORE_OK)
    {
        d_error("parse_ip() is failed");
        return rv;
    }

    if (mask_or_numbits)
    {
        if (ipsub->family == AF_INET6)
        {
            maxbits = 128;
        }
        bits = strtol(mask_or_numbits, &endptr, 10);
        if (*endptr == '\0' && bits > 0 && bits <= maxbits)
        {
            /* valid num-bits string; fill in mask appropriately */
            int cur_entry = 0;
            c_int32_t cur_bit_value;

            memset(ipsub->mask, 0, sizeof ipsub->mask);
            while (bits > 32)
            {
                ipsub->mask[cur_entry] = 0xFFFFFFFF; /* all 32 bits */
                bits -= 32;
                ++cur_entry;
            }
            cur_bit_value = 0x80000000;
            while (bits)
            {
                ipsub->mask[cur_entry] |= cur_bit_value;
                --bits;
                cur_bit_value /= 2;
            }
            ipsub->mask[cur_entry] = htonl(ipsub->mask[cur_entry]);
        }
        else if (inet_pton(AF_INET, mask_or_numbits, ipsub->mask) == 1 &&
            ipsub->family == AF_INET)
        {
            /* valid IPv4 netmask */
        }
        else
        {
            d_error("Bad netmask");
            return CORE_EBADMASK;
        }
    }

    fix_subnet(ipsub);

    return CORE_OK;
}

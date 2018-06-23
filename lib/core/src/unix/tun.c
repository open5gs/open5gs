#define TRACE_MODULE _core_tun

#include "core_debug.h"
#include "core_arch_network.h"

#if LINUX == 1
#include <linux/if_tun.h>
#else
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>
#endif

#if HAVE_NET_ROUTE_H
#include <net/route.h>
#endif

status_t tun_open(sock_id *new, char *ifname, int is_tap)
{
    status_t rv;
    sock_t *sock = NULL;
    int fd = -1;
#if LINUX == 1
    char *dev = "/dev/net/tun";
    int rc;
    struct ifreq ifr;
    int flags = IFF_NO_PI;

    fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        d_error("open() failed(%d:%s) : dev[%s]", errno, strerror(errno), dev);
        return -1;
    }
#else
    char name[C_PATH_MAX];
    int tun = 0;

#define TUNTAP_ID_MAX 255
    for (tun = 0; tun < TUNTAP_ID_MAX; tun++)
    {
        (void)snprintf(name, sizeof(name), "/dev/tun%i", tun);
        if ((fd = open(name, O_RDWR)) > 0)
        {
            (void)snprintf(name, sizeof(name), "tun%i", tun);
            ifname = name;
            break;
        }
    }
#endif

    rv = sock_create(new);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    sock = (sock_t *)(*new);
    d_assert(sock, return CORE_ERROR,);

    /* Save socket descriptor */
    sock->fd = fd;
    /* Save the interface name */
    strncpy(sock->ifname, ifname, IFNAMSIZ-1);

#if LINUX == 1
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = (is_tap ? (flags | IFF_TAP) : (flags | IFF_TUN));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);

    rc = ioctl(sock->fd, TUNSETIFF, (void *)&ifr);
    if (rc < 0)
    {
        d_error("ioctl() failed(%d:%s) : dev[%s] flags[0x%x]",
                errno, strerror(errno), ifname, flags);
        goto cleanup;
    }
#endif

    return CORE_OK;

#if LINUX == 1
cleanup:
    sock_delete(*new);
    return CORE_ERROR;
#endif
}

status_t tun_set_ipv4(sock_id id, ipsubnet_t *ipaddr, ipsubnet_t *ipsub)
{
#if LINUX != 1
    sock_t *sock = NULL;
    int fd;
    
	struct ifaliasreq ifa;
	struct ifreq ifr;
	struct sockaddr_in addr;
	struct sockaddr_in mask;

    char buf[512];
    int len;
    struct rt_msghdr *rtm;
    struct sockaddr_in dst, gw;
	struct sockaddr_in *paddr;

    d_assert(ipaddr, return CORE_ERROR,);
    d_assert(ipsub, return CORE_ERROR,);

    sock = (sock_t *)id;
    d_assert(id, return CORE_ERROR,);

    fd = socket(ipaddr->family, SOCK_DGRAM, 0);

	(void)memset(&ifa, '\0', sizeof ifa);
	(void)strlcpy(ifa.ifra_name, sock->ifname, sizeof ifa.ifra_name);

	(void)memset(&ifr, '\0', sizeof ifr);
	(void)strlcpy(ifr.ifr_name, sock->ifname, sizeof ifr.ifr_name);

#if 0
	/* Delete previously assigned address */
	(void)ioctl(fd, SIOCDIFADDR, &ifr);
#endif

	(void)memset(&addr, '\0', sizeof(addr));
	addr.sin_family = ipaddr->family;
	addr.sin_addr.s_addr = ipaddr->sub[0];
	addr.sin_len = sizeof(addr);
	(void)memcpy(&ifa.ifra_addr, &addr, sizeof(addr));
	(void)memcpy(&ifa.ifra_broadaddr, &addr, sizeof(addr));

	(void)memset(&mask, '\0', sizeof(mask));
	mask.sin_family = ipaddr->family;
	mask.sin_addr.s_addr = ipaddr->mask[0];
	mask.sin_len = sizeof(mask);
	(void)memcpy(&ifa.ifra_mask, &mask, sizeof(ifa.ifra_mask));

	if (ioctl(fd, SIOCAIFADDR, &ifa) == -1) {
		d_error("Can't IP address(%d:%s) : dev[%s]",
                errno, strerror(errno), sock->ifname);
		return CORE_ERROR;
	}

    close(fd); /* SOCK_DGRAM */

    fd = socket(PF_ROUTE, SOCK_RAW, 0);
    if (fd < 0)
    {
        d_error("Can't open PF_ROUTE(%d:%s)", errno, strerror(errno));
        return CORE_ERROR;
    }

    (void)memset(&buf, 0, sizeof(buf));
    rtm = (struct rt_msghdr *)buf;
    rtm->rtm_type = RTM_ADD;
    rtm->rtm_version = RTM_VERSION;
    rtm->rtm_pid = getpid();
    rtm->rtm_seq = 0;
    rtm->rtm_flags = RTF_UP | RTF_GATEWAY;
    rtm->rtm_addrs = RTA_DST | RTA_GATEWAY | RTA_NETMASK;
    paddr = (struct sockaddr_in *)(rtm + 1);

	(void)memset(&dst, '\0', sizeof(dst));
	dst.sin_family = ipaddr->family;
	dst.sin_addr.s_addr = ipsub->sub[0];
	dst.sin_len = sizeof(dst);
	(void)memcpy(paddr, &dst, sizeof(dst));
    paddr = (struct sockaddr_in *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

	(void)memset(&gw, '\0', sizeof(gw));
	gw.sin_family = ipaddr->family;
	gw.sin_addr.s_addr = ipaddr->sub[0];
	gw.sin_len = sizeof(gw);
	(void)memcpy(paddr, &gw, sizeof(gw));
    paddr = (struct sockaddr_in *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

	(void)memset(&mask, '\0', sizeof(mask));
	mask.sin_family = ipaddr->family;
	mask.sin_addr.s_addr = ipsub->mask[0];
	mask.sin_len = sizeof(mask);
	(void)memcpy(paddr, &mask, sizeof(mask));
    paddr = (struct sockaddr_in *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

    len = (char*)paddr - buf;
    rtm->rtm_msglen = len;
    if (write(fd, buf, len) < 0)
    {
        d_error("Can't add routing(%d:%s)", errno, strerror(errno));
        return CORE_ERROR;
    }

    close(fd); /* PF_ROUTE, SOCK_RAW */

#endif  /* LINUX == 1 */

	return CORE_OK;
}

status_t tun_set_ipv6(sock_id id, ipsubnet_t *ipaddr, ipsubnet_t *ipsub)
{
#if LINUX != 1
    sock_t *sock = NULL;
    int fd;
    
	struct in6_aliasreq ifa;
	struct in6_ifreq ifr;
	struct sockaddr_in6 addr;
	struct sockaddr_in6 mask;

    char buf[512];
    int len;
    struct rt_msghdr *rtm;
#if 0
    struct sockaddr_in6 dst, gw;
#else
    struct sockaddr_in6 dst;
#endif
	struct sockaddr_in6 *paddr;

    d_assert(ipaddr, return CORE_ERROR,);
    d_assert(ipsub, return CORE_ERROR,);

    sock = (sock_t *)id;
    d_assert(id, return CORE_ERROR,);

    fd = socket(ipaddr->family, SOCK_DGRAM, 0);

	(void)memset(&ifa, '\0', sizeof ifa);
	(void)strlcpy(ifa.ifra_name, sock->ifname, sizeof ifa.ifra_name);

	(void)memset(&ifr, '\0', sizeof ifr);
	(void)strlcpy(ifr.ifr_name, sock->ifname, sizeof ifr.ifr_name);

#if 0
	/* Delete previously assigned address */
	(void)ioctl(fd, SIOCDIFADDR, &ifr);
#endif

	(void)memset(&addr, '\0', sizeof(addr));
	addr.sin6_family = ipaddr->family;
    memcpy(addr.sin6_addr.s6_addr, ipaddr->sub, sizeof ipaddr->sub);
	addr.sin6_len = sizeof(addr);
	(void)memcpy(&ifa.ifra_addr, &addr, sizeof(addr));
	(void)memcpy(&ifa.ifra_dstaddr, &addr, sizeof(addr));

	(void)memset(&mask, '\0', sizeof(mask));
	mask.sin6_family = ipaddr->family;
    memcpy(mask.sin6_addr.s6_addr, ipaddr->mask, sizeof ipaddr->mask);
	mask.sin6_len = sizeof(mask);
	(void)memcpy(&ifa.ifra_prefixmask, &mask, sizeof(ifa.ifra_prefixmask));

    ifa.ifra_lifetime.ia6t_vltime = ND6_INFINITE_LIFETIME;
    ifa.ifra_lifetime.ia6t_pltime = ND6_INFINITE_LIFETIME;

	if (ioctl(fd, SIOCAIFADDR_IN6, &ifa) == -1) {
		d_error("Can't IP address(%d:%s) : dev[%s]",
                errno, strerror(errno), sock->ifname);
		return CORE_ERROR;
	}

    close(fd); /* SOCK_DGRAM */

    fd = socket(PF_ROUTE, SOCK_RAW, 0);
    if (fd < 0)
    {
        d_error("Can't open PF_ROUTE(%d:%s)", errno, strerror(errno));
        return CORE_ERROR;
    }

    (void)memset(&buf, 0, sizeof(buf));
    rtm = (struct rt_msghdr *)buf;
    rtm->rtm_type = RTM_DELETE;
    rtm->rtm_version = RTM_VERSION;
    rtm->rtm_pid = getpid();
    rtm->rtm_seq = 0;
    rtm->rtm_addrs = RTA_DST;
    paddr = (struct sockaddr_in6 *)(rtm + 1);

	(void)memset(&dst, '\0', sizeof(dst));
	dst.sin6_family = ipaddr->family;
    memcpy(dst.sin6_addr.s6_addr, ipaddr->sub, sizeof ipsub->sub);
	dst.sin6_len = sizeof(dst);
	(void)memcpy(paddr, &dst, sizeof(dst));
    paddr = (struct sockaddr_in6 *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

    len = (char*)paddr - buf;
    rtm->rtm_msglen = len;
    if (write(fd, buf, len) < 0)
    {
        d_error("Can't add routing(%d:%s)", errno, strerror(errno));
        return CORE_ERROR;
    }

#if 0
    (void)memset(&buf, 0, sizeof(buf));
    rtm = (struct rt_msghdr *)buf;
    rtm->rtm_type = RTM_ADD;
    rtm->rtm_version = RTM_VERSION;
    rtm->rtm_pid = getpid();
    rtm->rtm_seq = 0;
    rtm->rtm_flags = RTF_UP | RTF_GATEWAY;
    rtm->rtm_addrs = RTA_DST | RTA_GATEWAY | RTA_NETMASK;
    paddr = (struct sockaddr_in6 *)(rtm + 1);

	(void)memset(&dst, '\0', sizeof(dst));
	dst.sin6_family = ipaddr->family;
    memcpy(dst.sin6_addr.s6_addr, ipsub->sub, sizeof ipsub->sub);
	dst.sin6_len = sizeof(dst);
	(void)memcpy(paddr, &dst, sizeof(dst));
    paddr = (struct sockaddr_in6 *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

	(void)memset(&gw, '\0', sizeof(gw));
	gw.sin6_family = ipaddr->family;
    memcpy(gw.sin6_addr.s6_addr, ipaddr->sub, sizeof ipaddr->sub);
	gw.sin6_len = sizeof(gw);
	(void)memcpy(paddr, &gw, sizeof(gw));
    paddr = (struct sockaddr_in6 *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

	(void)memset(&mask, '\0', sizeof(mask));
	mask.sin6_family = ipaddr->family;
    memcpy(mask.sin6_addr.s6_addr, ipsub->mask, sizeof ipsub->mask);
	mask.sin6_len = sizeof(mask);
	(void)memcpy(paddr, &mask, sizeof(mask));
    paddr = (struct sockaddr_in6 *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

    len = (char*)paddr - buf;
    rtm->rtm_msglen = len;
    if (write(fd, buf, len) < 0)
    {
        d_error("Can't add routing(%d:%s)", errno, strerror(errno));
        return CORE_ERROR;
    }
#endif

    close(fd); /* PF_ROUTE, SOCK_RAW */

#endif  /* LINUX == 1 */

	return CORE_OK;
}

status_t tun_set_ip(sock_id id, ipsubnet_t *gw, ipsubnet_t *sub)
{
    status_t rv = CORE_OK;

    d_assert(id, return CORE_ERROR,);
    d_assert(gw, return CORE_ERROR,);
    d_assert(sub, return CORE_ERROR,);

    if (gw->family == AF_INET)
        rv = tun_set_ipv4(id, gw, sub);
    else if (gw->family == AF_INET6)
    {
#if 0
        rv = tun_set_ipv6(id, gw, sub);
#endif
    }
    else
        d_assert(0, return CORE_ERROR,);

    return rv;
}

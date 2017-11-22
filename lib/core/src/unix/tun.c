#define TRACE_MODULE _core_tun

#include "core_debug.h"
#include "core_arch_network.h"

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
        d_error("Can not open %s",dev);
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
    strncpy(sock->ifname, ifname, IFNAMSIZ);

#if LINUX == 1
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = (is_tap ? (flags | IFF_TAP) : (flags | IFF_TUN));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

    rc = ioctl(sock->fd, TUNSETIFF, (void *)&ifr);
    if (rc < 0)
    {
        d_error("iotcl error(dev:%s flags = %d)", ifname, flags);
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

status_t tun_set_ipv4(sock_id id, c_uint32_t ip_addr, c_uint8_t bits)
{
#if LINUX != 1
    sock_t *sock = NULL;
    int fd;
    
	struct ifaliasreq ifa;
	struct ifreq ifr;
	struct sockaddr_in addr;
	struct sockaddr_in mask;
    c_uint32_t mask_addr = htonl(0xffffffff << (32 - bits));

    char buf[512];
    int len;
    struct rt_msghdr *rtm;
    struct sockaddr_in dst, gw;
	struct sockaddr_in *paddr;

    sock = (sock_t *)id;
    d_assert(id, return CORE_ERROR,);

    fd = socket(AF_INET, SOCK_DGRAM, 0);

	(void)memset(&ifa, '\0', sizeof ifa);
	(void)strlcpy(ifa.ifra_name, sock->ifname, sizeof ifa.ifra_name);

	(void)memset(&ifr, '\0', sizeof ifr);
	(void)strlcpy(ifr.ifr_name, sock->ifname, sizeof ifr.ifr_name);

	/* Delete previously assigned address */
	(void)ioctl(fd, SIOCDIFADDR, &ifr);

	(void)memset(&addr, '\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip_addr;
	addr.sin_len = sizeof(addr);
	(void)memcpy(&ifa.ifra_addr, &addr, sizeof(addr));
	(void)memcpy(&ifa.ifra_broadaddr, &addr, sizeof(addr));

	(void)memset(&mask, '\0', sizeof(mask));
	mask.sin_family = AF_INET;
	mask.sin_addr.s_addr = 0xffffffff;
	mask.sin_len = sizeof(mask);
	(void)memcpy(&ifa.ifra_mask, &mask, sizeof(ifa.ifra_mask));

	if (ioctl(fd, SIOCAIFADDR, &ifa) == -1) {
		d_error("Can't IP address(dev:%s err:%s)",
                sock->ifname, strerror(errno));
		return CORE_ERROR;
	}

    close(fd); /* AF_INET, SOCK_DGRAM */

    fd = socket(PF_ROUTE, SOCK_RAW, 0);
    if (fd < 0)
    {
        d_error("Can't open PF_ROUTE(%s)", strerror(errno));
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
	dst.sin_family = AF_INET;
	dst.sin_addr.s_addr = ip_addr & mask_addr;
	dst.sin_len = sizeof(dst);
	(void)memcpy(paddr, &dst, sizeof(dst));
    paddr = (struct sockaddr_in *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

	(void)memset(&gw, '\0', sizeof(gw));
	gw.sin_family = AF_INET;
	gw.sin_addr.s_addr = ip_addr;
	gw.sin_len = sizeof(gw);
	(void)memcpy(paddr, &gw, sizeof(gw));
    paddr = (struct sockaddr_in *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

	(void)memset(&mask, '\0', sizeof(mask));
	mask.sin_family = AF_INET;
	mask.sin_addr.s_addr = mask_addr;
	mask.sin_len = sizeof(mask);
	(void)memcpy(paddr, &mask, sizeof(mask));
    paddr = (struct sockaddr_in *)((char *)paddr +
            CORE_ALIGN(sizeof(*paddr), sizeof(c_uintptr_t)));

    len = (char*)paddr - buf;
    rtm->rtm_msglen = len;
    if (write(fd, buf, len) < 0)
    {
        d_error("Can't add routing(%s)", strerror(errno));
        return CORE_ERROR;
    }

    close(fd); /* PF_ROUTE, SOCK_RAW */

#endif  /* LINUX == 1 */

	return 0;
}

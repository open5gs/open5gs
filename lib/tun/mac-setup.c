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

#include "ogs-tun.h"
#include "ipfw/ipfw2.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sock_domain

#include <net/if.h>
#include <net/route.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>

#if defined(__APPLE__)
#include <net/if_utun.h>
#include <sys/kern_event.h>
#include <sys/kern_control.h>

static int utun_open(int unit, char *ifname, socklen_t maxlen)
{
	struct sockaddr_ctl addr;
	struct ctl_info info;
	int fd = -1;
	int err = 0;

    ogs_assert(ifname);
    ogs_assert(maxlen);

	fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
	if (fd < 0) return fd;

	bzero(&info, sizeof (info));
	strncpy(info.ctl_name, UTUN_CONTROL_NAME, MAX_KCTL_NAME);

	err = ioctl(fd, CTLIOCGINFO, &info);
	if (err != 0) goto on_error;

	addr.sc_len = sizeof(addr);
	addr.sc_family = AF_SYSTEM;
	addr.ss_sysaddr = AF_SYS_CONTROL;
	addr.sc_id = info.ctl_id;
	addr.sc_unit = unit + 1;

	err = connect(fd, (struct sockaddr *)&addr, sizeof (addr));
	if (err != 0) goto on_error;

	err = getsockopt(fd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME, ifname, &maxlen);
	if (err != 0) goto on_error;

on_error:
	if (err != 0) {
		close(fd);
		return err;
	}

	return fd;
}
#endif

ogs_socket_t ogs_tun_open(char *ifname, int maxlen, int is_tap)
{
    ogs_socket_t fd = INVALID_SOCKET;
    int unit;

    ogs_assert(ifname);

#define TUNTAP_ID_MAX 256
    for (unit = 0; unit < TUNTAP_ID_MAX; unit++) {
#if defined(__APPLE__)
        /* MacOSX "utun" device driver */
        if ((fd = utun_open(unit, ifname, maxlen)) > 0) {
            break;
        }
#else
        /* FreeBSD "tun" device driver */
        char name[IFNAMSIZ];
        ogs_snprintf(name, sizeof(name), "/dev/tun%i", unit);
        if ((fd = open(name, O_RDWR)) > 0) {
            ogs_snprintf(ifname, maxlen, "tun%i", unit);
            break;
        }
#endif
    }
    if (fd < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "open() failed");
        return INVALID_SOCKET;
    }

    return fd;
}

#define TUN_ALIGN(size, boundary) \
        (((size) + ((boundary) - 1)) & ~((boundary) - 1))

static int tun_set_ipv4(char *ifname,
        ogs_ipsubnet_t *ipaddr, ogs_ipsubnet_t *ipsub)
{
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

    ogs_assert(ipaddr);
    ogs_assert(ipsub);

    fd = socket(ipaddr->family, SOCK_DGRAM, 0);

	(void)memset(&ifa, '\0', sizeof ifa);
	(void)strlcpy(ifa.ifra_name, ifname, sizeof ifa.ifra_name);

	(void)memset(&ifr, '\0', sizeof ifr);
	(void)strlcpy(ifr.ifr_name, ifname, sizeof ifr.ifr_name);

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
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "Can't IP address : dev[%s]", ifname);
		return OGS_ERROR;
	}

    close(fd); /* SOCK_DGRAM */

    fd = socket(PF_ROUTE, SOCK_RAW, 0);
    if (fd < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "Can't open PF_ROUTE");
        return OGS_ERROR;
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
            TUN_ALIGN(sizeof(*paddr), sizeof(uintptr_t)));

	(void)memset(&gw, '\0', sizeof(gw));
	gw.sin_family = ipaddr->family;
	gw.sin_addr.s_addr = ipaddr->sub[0];
	gw.sin_len = sizeof(gw);
	(void)memcpy(paddr, &gw, sizeof(gw));
    paddr = (struct sockaddr_in *)((char *)paddr +
            TUN_ALIGN(sizeof(*paddr), sizeof(uintptr_t)));

	(void)memset(&mask, '\0', sizeof(mask));
	mask.sin_family = ipaddr->family;
	mask.sin_addr.s_addr = ipsub->mask[0];
	mask.sin_len = sizeof(mask);
	(void)memcpy(paddr, &mask, sizeof(mask));
    paddr = (struct sockaddr_in *)((char *)paddr +
            TUN_ALIGN(sizeof(*paddr), sizeof(uintptr_t)));

    len = (char*)paddr - buf;
    rtm->rtm_msglen = len;
    if (write(fd, buf, len) < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "Can't add routing");
        return OGS_ERROR;
    }

    close(fd); /* PF_ROUTE, SOCK_RAW */

	return OGS_OK;
}

static int tun_set_ipv6(char *ifname,
        ogs_ipsubnet_t *ipaddr, ogs_ipsubnet_t *ipsub)
{
#if 1 /* We'll just run ifconfig command in the shell */
    ogs_proc_t process;
    int ret = 0, out_return_code = 0;

    const char *commandLine[OGS_ARG_MAX];
    char addr[128];

    char buf[OGS_ADDRSTRLEN];
    int prefixlen;

    ogs_assert(ifname);
    ogs_assert(ipsub);
    ogs_assert(ipaddr);

#define IPV6_BITLEN    (OGS_IPV6_LEN * 8)
    prefixlen = contigmask((uint8_t *)ipsub->mask, IPV6_BITLEN);

    ogs_snprintf(addr, sizeof addr, "%s/%d",
            OGS_INET6_NTOP(ipaddr->sub, buf), prefixlen);

    commandLine[0] = "/sbin/ifconfig";
    commandLine[1] = ifname;;
    commandLine[2] = "inet6";
    commandLine[3] = addr;
    commandLine[4] = "up";
    commandLine[5] = NULL;

    ret = ogs_proc_create(commandLine, 0, &process);
    ogs_assert(ret == 0);

    ret = ogs_proc_join(&process, &out_return_code);
    ogs_assert(ret == 0);
    ogs_assert(out_return_code == 0);

    ret = ogs_proc_destroy(&process);
    ogs_assert(ret == 0);

#else /* IPv6 Setting API is not working in UTUN  */
    int fd;
    
	struct in6_aliasreq ifa;
	struct in6_ifreq ifr;
	struct sockaddr_in6 addr;
	struct sockaddr_in6 mask;

    char buf[512];
    int len;
    struct rt_msghdr *rtm;
    struct sockaddr_in6 dst, gw;
	struct sockaddr_in6 *paddr;

    ogs_assert(ipaddr);
    ogs_assert(ipsub);

    fd = socket(ipaddr->family, SOCK_DGRAM, 0);

	(void)memset(&ifa, '\0', sizeof ifa);
	(void)strlcpy(ifa.ifra_name, ifname, sizeof ifa.ifra_name);

	(void)memset(&ifr, '\0', sizeof ifr);
	(void)strlcpy(ifr.ifr_name, ifname, sizeof ifr.ifr_name);

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
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "Can't IP address : dev[%s]", ifname);
		return OGS_ERROR;
	}

    close(fd); /* SOCK_DGRAM */

    fd = socket(PF_ROUTE, SOCK_RAW, 0);
    if (fd < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "Can't open PF_ROUTE");
        return OGS_ERROR;
    }

#if 0
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
            TUN_ALIGN(sizeof(*paddr), sizeof(uintptr_t)));

    len = (char*)paddr - buf;
    rtm->rtm_msglen = len;
    if (write(fd, buf, len) < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "Can't add routing");
        return OGS_ERROR;
    }
#endif

    /* The followings does not work in IPv6 */
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
            TUN_ALIGN(sizeof(*paddr), sizeof(uintptr_t)));

	(void)memset(&gw, '\0', sizeof(gw));
	gw.sin6_family = ipaddr->family;
    memcpy(gw.sin6_addr.s6_addr, ipaddr->sub, sizeof ipaddr->sub);
	gw.sin6_len = sizeof(gw);
	(void)memcpy(paddr, &gw, sizeof(gw));
    paddr = (struct sockaddr_in6 *)((char *)paddr +
            TUN_ALIGN(sizeof(*paddr), sizeof(uintptr_t)));

	(void)memset(&mask, '\0', sizeof(mask));
	mask.sin6_family = ipaddr->family;
    memcpy(mask.sin6_addr.s6_addr, ipsub->mask, sizeof ipsub->mask);
	mask.sin6_len = sizeof(mask);
	(void)memcpy(paddr, &mask, sizeof(mask));
    paddr = (struct sockaddr_in6 *)((char *)paddr +
            TUN_ALIGN(sizeof(*paddr), sizeof(uintptr_t)));

    len = (char*)paddr - buf;
    rtm->rtm_msglen = len;
    if (write(fd, buf, len) < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "Can't add routing");
        return OGS_ERROR;
    }

    close(fd); /* PF_ROUTE, SOCK_RAW */

#endif

	return OGS_OK;
}

int ogs_tun_set_ip(char *ifname, ogs_ipsubnet_t *gw, ogs_ipsubnet_t *sub)
{
    int rv = OGS_OK;

    ogs_assert(gw);
    ogs_assert(sub);

    if (gw->family == AF_INET)
        rv = tun_set_ipv4(ifname, gw, sub);
    else if (gw->family == AF_INET6)
        rv = tun_set_ipv6(ifname, gw, sub);

    return rv;
}

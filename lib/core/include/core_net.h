#ifndef __CORE_NET_H__
#define __CORE_NET_H__

#include "core.h"
#include "core_errno.h"
#include "core_index.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#if HAVE_NETDB_H
#include <netdb.h>
#endif

#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#define MAX_NET_POOL_SIZE       512

/* Max length of interface name, ex: eth0, ath0 .. */
#define MAX_IFNAME_LEN      16

#define INET_NTOP(src, dst)   inet_ntop(AF_INET,(void *)(src),(dst),INET_ADDRSTRLEN)
#define INET6_NTOP(src, dst)   inet_ntop(AF_INET6,(void *)(src),(dst),INET6_ADDRSTRLEN)

/** Network handler */
typedef int (*net_handler)(void *net_sl, void *data);

/** Network socket descriptor */
typedef struct {
    int type;
    int proto;
#define SCTP_S1AP_PPID 18
#define SCTP_X2AP_PPID 27
    c_uint32_t ppid;

    int sock_id;
#if 0 /* deprecated */
    struct sockaddr_in local;
#endif
    struct sockaddr_in remote;
    int opt;

    index_t app_index;
    int sndrcv_errno;
} net_sock_t;

/** Network socket handler */
typedef int (*net_sock_handler)(net_sock_t *net_sock, void *data);

/** Basic socket library */

/** Initialize network library */
CORE_DECLARE(status_t) net_init(void);

/** Finalize network library */
CORE_DECLARE(status_t) net_final(void);

/**
 * Create network session.
 * @param net_sock Connected network session.
 * @param host Host IP address to be connected.
 * @param lport Local Port number(only for UDP)
 * @param rport Remote Port number
 * @param proto Protocol proto
 * @param type Protocol type
 * @param flag Option flags to be set for this connection
 */
CORE_DECLARE(int) net_open(net_sock_t **net_sock,const char *host, 
              const int lport,
              const int rport,
              int type, int proto);

/**
 * Create network session.
 * @param net_sock Connected network session.
 * @param local_addr Local Host IP address to bind(Network order)
 * @param remote_host Remote Host IP address to be connected.
 * @param lport Local Port number(only for UDP)
 * @param rport Remote Port number
 * @param proto Protocol proto
 * @param type Protocol type
 * @param flag Option flags to be set for this connection
 */
CORE_DECLARE(int) net_open_ext(net_sock_t **net_sock,
              const c_uint32_t local_addr,
              const char *remote_host, 
              const int lport,
              const int rport,
              int type, int proto, c_uint32_t ppid, const int flag);
/**
 * Read the data from the socket
 * @param net_sock Socket which created before
 * @param buffer Buffer which data be saved
 * @param size  Total length of buffer
 * @param timeout Wait timeout. If 0 , it will block until data ready
 */
CORE_DECLARE(int) net_read(net_sock_t *net_sock, char *buffer, size_t size,int timeout);

/** Write the data into the socket
 * @param net_sock Socket which created before
 * @param buffer Buffer which data be saved
 * @param size  Total length of buffer
 */
CORE_DECLARE(int) net_write(net_sock_t *net_sock, char *buffer, size_t size,
        struct sockaddr_in *dest_addr, int addrlen);

CORE_DECLARE(int) net_send(net_sock_t *net_sock, char *buffer, size_t size);

CORE_DECLARE(int) net_sendto(net_sock_t *net_sock, char *buffer, size_t size,
        c_uint32_t ip_addr, c_uint16_t port);


/** Close the socket
 * @param net_sock Socket to be closed
 */
CORE_DECLARE(int) net_close(net_sock_t *net_sock);

/** Wait the new socket session until given timeout
 * @param new_accept_sock Newly created socket for new connection
 * @param net_sock Listend socket before created
 * @param timeout Wait timeout. If 0, it will be blocked until new connection
 *        received
 */
CORE_DECLARE(int) net_accept(net_sock_t **new_accept_sock, 
        net_sock_t *net_sock, int timeout);

/** Create socket and listen to the specified port
 * @param net_sock Returned socket
 * @param type Protocol type
 * @param proto Protocol proto
 * @param ppid SCTP PPID
 * @param addr Specific address
 * @param port Port number
 */
CORE_DECLARE(int) net_listen_ext(net_sock_t **net_sock, 
        const int type, const int proto, const c_uint32_t ppid, 
        const c_uint32_t addr, const int port);
/** Create socket and listen to the specified port
 * @param net_sock Returned socket
 * @param type Protocol type
 * @param proto Protocol proto
 * @param port Port number
 */
CORE_DECLARE(int) net_listen(
        net_sock_t **net_sock, const int type, const int proto, 
        const int port);

/** Network application protocol */
#define MAX_FTP_SESSION_SIZE   10
typedef struct {
    net_sock_t *ctrl_sock;          /* Control channel */
    net_sock_t *data_sock;          /* Data channel */
    int flag;
    char cmd_buf[256];
    char resp_buf[256];
} net_ftp_t;

/** Open ftp session.
 * @param host host name or IP address to connect
 * @param username User ID or NULL if username is anonymous
 * @param passwd Password or NULL if no passwd
 * @param flag Option flags
 * @param ftp_session Ftp session structure. If connection failed , it will be NULL
 */
CORE_DECLARE(int) net_ftp_open(const char *host,
                  const char *username,
                  const char *passwd,
                  int flag,
                  net_ftp_t **ftp_session);

/** Retrieve file using FTP
 * @param ftp_session Ftp session which created from net_ftp_open
 * @param remote_filename Remote filename to retrieve
 * @param local_filename Local filename. If null, the same name as remote will
 *                       be used.
 */
CORE_DECLARE(int) net_ftp_get(net_ftp_t *ftp_session,
                 const char *remote_filename,
                 const char *local_filename);

/** Upload file using FTP
 * @param ftp_session Ftp session which created from net_ftp_open
 * @param local_filename Local filename to upload.
 * @param remote_filename Remote filename.If null, the same name as local will
 *                        be used.
 */
CORE_DECLARE(int) net_ftp_put(net_ftp_t *ftp_session,
                 const char *local_filename,
                 const char *remote_filename);
/** Quit from ftp
 * @param ftp_session Ftp session which created from net_ftp_open
 */
CORE_DECLARE(int) net_ftp_quit(net_ftp_t *ftp_session);

/** Close ftp session
 * @param ftp_session Ftp session which created from net_ftp_open
 */
CORE_DECLARE(int) net_ftp_close(net_ftp_t *ftp_session);

/** Network session pool */
CORE_DECLARE(int) net_pool_avail();

/** Network link interface */
typedef struct {
    int fd;
    int ioctl_sock;
    char ifname[MAX_IFNAME_LEN];
    struct sockaddr hwaddr;
} net_link_t;

/** Network link handler */
typedef int (*net_link_handler)(net_link_t *net_sock, void *data);


/** Open network interface */
CORE_DECLARE(int) net_link_open(net_link_t **net_link, char *device, int proto);
/** Close network interface */
CORE_DECLARE(int) net_link_close(net_link_t *net_link);

/** Enable or disable promisc mode of network interface */
CORE_DECLARE(int) net_link_promisc(net_link_t *net_link, int enable);

/** Write the data into the link */
CORE_DECLARE(int) net_link_write(net_link_t *net_link, char *buf, int len);

/** Reate the data from the link */
CORE_DECLARE(int) net_link_read(net_link_t *net_link, char *buffer, int size, 
                    int timeout);

/** Open the specified protocol raw socket */
CORE_DECLARE(int) net_raw_open(net_link_t **net_link, int proto);

/** Clse the specified protocol raw socket */
CORE_DECLARE(int) net_raw_close(net_link_t *net_link);

CORE_DECLARE(int) net_link_sendto(net_link_t *net_link, char *buf, int len,
        struct sockaddr *dest_addr, int addrlen);

/** Register net_sock */
CORE_DECLARE(int) net_register_sock(net_sock_t *net_sock, 
                                    net_sock_handler handler, void *data);

/** Register net_link */
CORE_DECLARE(int) net_register_link(net_link_t *net_link, 
                                    net_link_handler handler, void *data);

/** Unregister net_sock */
CORE_DECLARE(int) net_unregister_sock(net_sock_t *net_sock);

/** Unregister net_link */
CORE_DECLARE(int) net_unregister_link(net_link_t *net_link);

/** Read the multiple fds and run the registered handler
 * @param timeout timeout(milliseconds)
 */
CORE_DECLARE(int) net_fds_read_run(long timeout);

/** TunTap interface */
CORE_DECLARE(int) net_tuntap_open(net_link_t **net_link, char *tuntap_dev_name, 
        int is_tap);

CORE_DECLARE(int) net_tuntap_close(net_link_t *net_link);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif    /* ! __CORE_NET_H__ */

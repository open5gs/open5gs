/*
 * Userland functions missing in linux
 * taken from /usr/src/lib/libc/stdtime/time32.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>	/* sockaddr_in */
#include <netinet/tcp.h>	/* TCP_NODELAY */
#include <sys/uio.h>
#include <unistd.h>	/* uint* types */
#include <errno.h>
#include <string.h>	/* bzero */
#include <arpa/inet.h>	/* htonl */

#ifndef HAVE_NAT
/* dummy nat functions */
void
ipfw_show_nat(int ac, char **av)
{
        D("unsupported");
}

void
ipfw_config_nat(int ac, char **av)
{
        D("unsupported");
}
#endif /* HAVE_NAT */

#ifdef NEED_STRTONUM
/* missing in linux and windows */
long long int
strtonum(const char *nptr, long long minval, long long maxval,
         const char **errstr)
{
        long long ret;
        int errno_c = errno;    /* save actual errno */

        errno = 0;
#ifdef TCC
        ret = strtol(nptr, (char **)errstr, 0);
#else
        ret = strtoll(nptr, (char **)errstr, 0);
#endif
        /* We accept only a string that represent exactly a number (ie. start
         * and end with a digit).
         * FreeBSD version wants errstr==NULL if no error occurs, otherwise
         * errstr should point to an error string.
         * For our purspose, we implement only the invalid error, ranges
         * error aren't checked
         */
        if (errno != 0 || nptr == *errstr || **errstr != '\0')
                *errstr = "invalid";
        else  {
                *errstr = NULL;
                errno = errno_c;
        }
        return ret;
}

int
ishexnumber(int c)
{
        return ((c >= '0' && c <= '9') ||
                (c >= 'a' && c <= 'f') ||
                (c >= 'A' && c <= 'F') );
}

#endif /* NEED_STRTONUM */

#ifdef __linux__


int optreset;	/* missing in linux */

/*
 * not implemented in linux.
 * taken from /usr/src/lib/libc/string/strlcpy.c
 */
size_t
strlcpy(dst, src, siz)
        char *dst;
        const char *src;
        size_t siz;
{
        char *d = dst;
        const char *s = src;
        size_t n = siz;
 
        /* Copy as many bytes as will fit */
        if (n != 0 && --n != 0) {
                do {
                        if ((*d++ = *s++) == 0)
                                break;
                } while (--n != 0);
        }

        /* Not enough room in dst, add NUL and traverse rest of src */
        if (n == 0) {
                if (siz != 0)
                        *d = '\0';              /* NUL-terminate dst */
                while (*s++)
                        ;
        }

        return(s - src - 1);    /* count does not include NUL */
}


/********************************************************
 * modifed by acetcom
 *
 * move down to remove sysctlbyname() in MacOSX and FreeBSD
 */

/* #endif */ /* __linux__ */

/********************************************************/


#if defined (EMULATE_SYSCTL)
//XXX missing prerequisites
#include <net/if.h> 		//openwrt
#include <netinet/ip.h> 	//openwrt
#include <netinet/ip_fw.h>
#include <netinet/ip_dummynet.h>
int do_cmd(int optname, void *optval, uintptr_t optlen);
#endif /* EMULATE_SYSCTL */

/*
 * set or get system information
 * XXX lock acquisition/serialize calls
 *
 * we export this as sys/module/ipfw_mod/parameters/___
 * This function get or/and set the value of the sysctl passed by
 * the name parameter. If the old value is not desired,
 * oldp and oldlenp should be set to NULL.
 *
 * XXX
 * I do not know how this works in FreeBSD in the case
 * where there are no write permission on the sysctl var.
 * We read the value and set return variables in any way
 * but returns -1 on write failures, regardless the
 * read success.
 *
 * Since there is no information on types, in the following
 * code we assume a length of 4 is a int.
 *
 * Returns 0 on success, -1 on errors.
 */
int
sysctlbyname(const char *name, void *oldp, size_t *oldlenp, void *newp,
         size_t newlen)
{
#if defined (EMULATE_SYSCTL)
	/*
	 * we embed the sysctl request in the usual sockopt mechanics.
	 * the sockopt buffer il filled with a dn_id with IP_DUMMYNET3
	 * command, and the special DN_SYSCTL_GET and DN_SYSCTL_SET
	 * subcommands.
	 * the syntax of this function is fully compatible with
	 * POSIX sysctlby name:
	 * if newp and newlen are != 0 => this is a set
	 * else if oldp and oldlen are != 0 => this is a get
	 *		to avoid too much overhead in the module, the whole
	 *		sysctltable is returned, and the parsing is done in userland,
	 *		a probe request is done to retrieve the size needed to
	 *		transfer the table, before the real request
	 * if both old and new params = 0 => this is a print
	 *		this is a special request, done only by main()
	 *		to implement the extension './ipfw sysctl',
	 *		a command that bypasses the normal getopt, and that
	 *		is available on those platforms that use this
	 *		sysctl emulation.
	 *		in this case, a negative oldlen signals that *oldp
	 *		is actually a FILE* to print somewhere else than stdout
	 */

	int l;
	int ret;
	struct dn_id* oid;
	struct sysctlhead* entry;
	char* pstring;
	char* pdata;
	FILE* fp;

	if((oldlenp != NULL) && ((int)*oldlenp < 0))
		fp = (FILE*)oldp;
	else
		fp = stdout;
	if(newp != NULL && newlen != 0)
	{
		//this is a set
		l = sizeof(struct dn_id) + sizeof(struct sysctlhead) + strlen(name)+1 + newlen;
		oid = malloc(l);
		if (oid == NULL)
			return -1;
		oid->len = l;
		oid->type = DN_SYSCTL_SET;
		oid->id = DN_API_VERSION;

		entry = (struct sysctlhead*)(oid+1);
		pdata = (char*)(entry+1);
		pstring = pdata + newlen;

		entry->blocklen = ((sizeof(struct sysctlhead) + strlen(name)+1 + newlen) + 3) & ~3;
		entry->namelen = strlen(name)+1;
		entry->flags = 0;
		entry->datalen = newlen;

		bcopy(newp, pdata, newlen);
		bcopy(name, pstring, strlen(name)+1);

		ret = do_cmd(IP_DUMMYNET3, oid, (uintptr_t)l);
		if (ret != 0)
			return -1;
	}
	else
	{
		//this is a get or a print
		l = sizeof(struct dn_id);
		oid = malloc(l);
		if (oid == NULL)
			return -1;
		oid->len = l;
		oid->type = DN_SYSCTL_GET;
		oid->id = DN_API_VERSION;

		ret = do_cmd(-IP_DUMMYNET3, oid, (uintptr_t)&l);
		if (ret != 0)
			return -1;

		l=oid->id;
		free(oid);
		oid = malloc(l);
		if (oid == NULL)
			return -1;
		oid->len = l;
		oid->type = DN_SYSCTL_GET;
		oid->id = DN_API_VERSION;

		ret = do_cmd(-IP_DUMMYNET3, oid, (uintptr_t)&l);
		if (ret != 0)
			return -1;

		entry = (struct sysctlhead*)(oid+1);
		while(entry->blocklen != 0)
		{
			pdata = (char*)(entry+1);
			pstring = pdata+entry->datalen;

			//time to check if this is a get or a print
			if(name != NULL && oldp != NULL && *oldlenp > 0)
			{
				//this is a get
				if(strcmp(name,pstring) == 0)
				{
					//match found, sanity chech on len
					if(*oldlenp < entry->datalen)
					{
						printf("%s error: buffer too small\n",__FUNCTION__);
						return -1;
					}
					*oldlenp = entry->datalen;
					bcopy(pdata, oldp, *oldlenp);
					return 0;
				}
			}
			else
			{
				//this is a print
				if( name == NULL )
					goto print;
				if ( (strncmp(pstring,name,strlen(name)) == 0) && ( pstring[strlen(name)]=='\0' || pstring[strlen(name)]=='.' ) )
						goto print;
				else
						goto skip;
print:
				fprintf(fp, "%s: ",pstring);
				switch( entry->flags >> 2 )
				{
					case SYSCTLTYPE_LONG:
						fprintf(fp, "%li ", *(long*)(pdata));
						break;
					case SYSCTLTYPE_UINT:
						fprintf(fp, "%u ", *(unsigned int*)(pdata));
						break;
					case SYSCTLTYPE_ULONG:
						fprintf(fp, "%lu ", *(unsigned long*)(pdata));
						break;
					case SYSCTLTYPE_INT:
					default:
						fprintf(fp, "%i ", *(int*)(pdata));
				}
				if( (entry->flags & 0x00000003) == CTLFLAG_RD )
					fprintf(fp, "\t(read only)\n");
				else
					fprintf(fp, "\n");
skip:			;
			}
			entry = (struct sysctlhead*)((unsigned char*)entry + entry->blocklen);
		}
		free(oid);
		return 0;
	}
	//fallback for invalid options
	return -1;

#else /* __linux__ */
	FILE *fp;
	char *basename = "/sys/module/ipfw_mod/parameters/";
	char filename[256];	/* full filename */
	char *varp;
	int ret = 0;		/* return value */
	int d;

	if (name == NULL) /* XXX set errno */
		return -1;

	/* locate the filename */
	varp = strrchr(name, '.');
	if (varp == NULL) /* XXX set errno */
		return -1;

	snprintf(filename, sizeof(filename), "%s%s", basename, varp+1);

	/*
	 * XXX we could open the file here, in rw mode
	 * but need to check if a file have write
	 * permissions.
	 */

	/* check parameters */
	if (oldp && oldlenp) { /* read mode */
		fp = fopen(filename, "r");
		if (fp == NULL) {
			fprintf(stderr, "%s fopen error reading filename %s\n", __FUNCTION__, filename);
			return -1;
		}
		if (*oldlenp == 4) {
			if (fscanf(fp, "%d", &d) == 1)
				memcpy(oldp, &d, *oldlenp);
			else
				ret = -1;
		}
		fclose(fp);
	}

	if (newp && newlen) { /* write */
		fp = fopen(filename, "w");
		if (fp == NULL) {
			fprintf(stderr, "%s fopen error writing filename %s\n", __FUNCTION__, filename);
			return -1;
		}
		if (newlen == 4) {
			if (fprintf(fp, "%d", *(int*)newp) < 1)
				ret = -1;
		}

		fclose(fp);
	}

	return ret;
#endif /* __linux__ */
}

/********************************************************
 * modifed by acetcom
 *
 * move down to remove sysctlbyname in MacOSX and FreeBSD
 */

#endif /* __linux__ */

/********************************************************/

/*
 * The following two functions implement getsockopt/setsockopt
 * replacements to talk over a TCP socket.
 * Because the calls are synchronous, we can run blocking code
 * and do not need to play special tricks to be selectable.
 * The wire protocol for the emulation is the following:
 *	REQUEST:  n32 req_size, level, optname; u8 data[req_size]
 *	RESPONSE: n32 resp_size, ret_code; u8 data[resp_size]
 *		data is only present if ret_code == 0
 *
 * Return 0 if the message wan sent to the remote
 * endpoint, -1 on error.
 *
 * If the required lenght is greater then the
 * available buffer size, -1 is returned and
 * optlen is the required lenght.
 */
enum sock_type {GET_SOCKOPT, SET_SOCKOPT};

struct wire_hdr {
	uint32_t optlen;	/* actual data len */
	uint32_t level;		/* or error */
	uint32_t optname;	/* or act len */
	uint32_t dir;		/* in or out */
};

/* do a complete write of the buffer */
static int
writen(int fd, const char *buf, int len)
{
	int i;

	for (; len > 0; buf += i, len -= i) {
		i = write(fd, buf, len);
		ND("have %d wrote %d", len, i);
		if (i < 0) {
			if (errno == EAGAIN)
				continue;
			return -1;
		}
	}
	return 0;
}

/* do a complete read */
static int
readn(int fd, char *buf, int len)
{
	int i, pos;

	for (pos = 0; pos < len; pos += i) {
		i = read(fd, buf + pos, len - pos);
		ND("have %d want %d got %d", pos, len, i);
		if (i < 0) {
			if (errno == EAGAIN)
				continue;
			return -1;
		}
	}
	ND("full read got %d", pos);
	return 0;
}

int
__sockopt2(int s, int level, int optname, void *optval, socklen_t *optlen,
		enum sopt_dir dir)
{
	struct wire_hdr r;
	int len = optlen && optval ? *optlen : 0;
	int new_errno;

	ND("dir %d optlen %d level %d optname %d", dir, len, level, optname);
	/* send request to the server */
	r.optlen = htonl(len);
	r.level = htonl(level);
	r.optname = htonl(optname);
	r.dir = htonl(dir);

	if (writen(s, (const char *) &r, sizeof(r)))
		return -1;	/* error writing */

	/* send data, if present */
	if (len < 0) {
		fprintf(stderr, "%s invalid args found\n", __FUNCTION__);
		return -1;
	} else if (len > 0) {
		if (writen(s, optval, len))
			return -1;	/* error writing */
	}

	/* read response size and error code */
	if (readn(s, (char *)&r, sizeof(r)))
		return -1;	/* error reading */
	len = ntohl(r.optlen);
	ND("got header, datalen %d", len);
	if (len > 0) {
		if (readn(s, optval, len)) {
			return -1;	/* error reading */
		}
	}
	if (optlen)
		*optlen = ntohl(r.optlen); /* actual len */
	new_errno = ntohl(r.level);
	if (new_errno)
		errno = new_errno;
	return (new_errno ? -1 : 0);
}

/*
 * getsockopt() replacement.
 */
int
getsockopt2(int s, int level, int optname, void *optval,
	socklen_t *optlen)
{
	return __sockopt2(s, level, optname, optval, optlen, SOPT_GET);
}

/*
 * setsockopt() replacement
 */
int
setsockopt2(int s, int level, int optname, void *optval,
	socklen_t optlen)
{
	/* optlen not changed, use the local address */
	return __sockopt2(s, level, optname, optval, &optlen, SOPT_SET);
}

#ifdef socket
#undef socket	/* we want the real one */
#endif
/*
 * This function replaces the socket() call to connect to
 * the ipfw control socket.
 * We actually ignore the paramerers if IPFW_HOST and IPFW_PORT
 * are defined.
 */
int
do_connect(const char *addr, int port)
{
	int conn_fd;

	/* open the socket */
#ifdef NETLINK

struct rtnl_handle rth;

	conn_fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
#else
        struct sockaddr_in server;		/* server address */
	const char *s;

        conn_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (conn_fd < 0) {
		perror("socket");
		return -1;
	}
#endif
#ifndef NETLINK
	/* fill the sockaddr structure with server address */
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;

	/* override the host if set in the environment */
	s = getenv("IPFW_HOST");
	if (s)
		addr = s;
	inet_aton(addr, &server.sin_addr);
	s = getenv("IPFW_PORT");
	if (s && atoi(s) > 0)
		port = atoi(s);
	server.sin_port = htons(port);

	/* connect to the server */
        if (connect(conn_fd, (struct sockaddr*) &server, sizeof(server)) < 0) {
		perror("connect");
		return -1;
	}
#ifdef setsockopt /* we want the real one here */
#undef setsockopt
#undef getsockopt
#endif
	{
		int on = 1, ret;
		ret = setsockopt(conn_fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));
		ND("set TCP_NODELAY %d returns %d", on, ret);
	}
	if (0)
		fprintf(stderr, "connected to %s:%d\n",
			inet_ntoa(server.sin_addr), ntohs(server.sin_port));
#endif
	return conn_fd;
}

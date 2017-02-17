/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2013, WIDE Project and NICT								 *
* All rights reserved.											 *
* 													 *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:						 *
* 													 *
* * Redistributions of source code must retain the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer.										 *
*    													 *
* * Redistributions in binary form must reproduce the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer in the documentation and/or other						 *
*   materials provided with the distribution.								 *
* 													 *
* * Neither the name of the WIDE Project or NICT nor the 						 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of WIDE Project and 						 *
*   NICT.												 *
* 													 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 	 *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 	 *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.								 *
*********************************************************************************************************/

/* This file contains the definitions for internal use in the freeDiameter core library */

#ifndef _FDCORE_INTERNAL_H
#define _FDCORE_INTERNAL_H

#include <freeDiameter/freeDiameter-host.h>
#include <freeDiameter/libfdcore.h>

#ifdef DISABLE_SCTP
#undef IPPROTO_SCTP
#define IPPROTO_SCTP	(2 = 4) /* some compilation error to spot the references */
#endif /* DISABLE_SCTP */

#ifndef HAVE_AI_ADDRCONFIG
#define AI_ADDRCONFIG 0 /* ignore this flag at the moment */
#endif /* HAVE_AI_ADDRCONFIG */

/* Timeout for establishing a connection */
#ifndef CNX_TIMEOUT
#define  CNX_TIMEOUT	10	/* in seconds */
#endif /* CNX_TIMEOUT */

/* Timeout for receiving a CER after incoming connection is established */
#ifndef INCNX_TIMEOUT
#define  INCNX_TIMEOUT	 20	/* in seconds */
#endif /* INCNX_TIMEOUT */

/* Timeout for receiving a CEA after CER is sent */
#ifndef CEA_TIMEOUT
#define  CEA_TIMEOUT	10	/* in seconds */
#endif /* CEA_TIMEOUT */

/* The timeout value to wait for answer to a DPR */
#ifndef DPR_TIMEOUT
#define DPR_TIMEOUT 	15	/* in seconds */
#endif /* DPR_TIMEOUT */

/* Delay where the connection is maintained opened to allow exchanging remaining pending answers after DPR/DPA */
#ifndef GRACE_TIMEOUT
#define GRACE_TIMEOUT   1	/* in seconds */
#endif /* GRACE_TIMEOUT */

/* The Vendor-Id to advertise in CER/CEA */
#ifndef MY_VENDOR_ID
#define MY_VENDOR_ID	0 	/* Reserved value to tell it must be ignored */
#endif /* MY_VENDOR_ID */



/* Configuration */
int fd_conf_init();
int fd_conf_deinit();
int fd_conf_parse();
int fddparse(struct fd_config * conf); /* yacc generated */
int fd_conf_stream_to_gnutls_datum(FILE * pemfile, gnutls_datum_t *out);


/* Extensions */
int fd_ext_add( char * filename, char * conffile );
int fd_ext_load();
int fd_ext_term(void);

/* Messages */
int fd_msg_init(void);
extern struct dict_object * fd_dict_avp_OSI; /* Origin-State-Id */
extern struct dict_object * fd_dict_cmd_CER; /* Capabilities-Exchange-Request */
extern struct dict_object * fd_dict_cmd_DWR; /* Device-Watchdog-Request */
extern struct dict_object * fd_dict_avp_DC;  /* Disconnect-Cause */
extern struct dict_object * fd_dict_cmd_DPR; /* Disconnect-Peer-Request */

/* Global message queues */
extern struct fifo * fd_g_incoming; /* all messages received from other peers, except local messages (CER, ...) */
extern struct fifo * fd_g_outgoing; /* messages to be sent to other peers on the network following routing procedure */
extern struct fifo * fd_g_local; /* messages to be handled to local extensions */
/* Message queues */
int fd_queues_init(void);
int fd_queues_fini(struct fifo ** queue);

/* Trigged events */
int fd_event_trig_call_cb(int trigger_val);
int fd_event_trig_fini(void);

/* Create all the dictionary objects defined in the Diameter base RFC. */
int fd_dict_base_protocol(struct dictionary * dict);

/* Routing */
int fd_rtdisp_init(void);
int fd_rtdisp_cleanstop(void);
int fd_rtdisp_fini(void);
int fd_rtdisp_cleanup(void);

/* Sentinel for the sent requests list */
struct sr_list {
	struct fd_list 	srs; /* requests ordered by hop-by-hop id */
	struct fd_list  exp; /* requests that have a timeout set, ordered by timeout */
	long            cnt; /* number of requests in the srs list */
	long		cnt_lost; /* number of requests that have not been answered in time. 
				     It is decremented when an unexpected answer is received, so this may not be accurate. */
	pthread_mutex_t	mtx; /* mutex to protect these lists */
	pthread_cond_t  cnd; /* cond var used by the thread that handles timeouts */
	pthread_t       thr; /* the thread that handles timeouts (expirecb called in separate forked threads) */
};

/* Peers */
struct fd_peer { /* The "real" definition of the peer structure */
	
	/* The public data */
	struct peer_hdr	 p_hdr;
	
	/* Eye catcher, EYEC_PEER */
	int		 p_eyec;
	#define EYEC_PEER	0x373C9336
	
	/* Origin of this peer object, for debug */
	char		*p_dbgorig;
	
	/* State of the peer, and its lock */
	enum peer_state	 p_state;
	pthread_mutex_t  p_state_mtx;
	
	/* Chaining in peers sublists */
	struct fd_list	 p_actives;	/* list of peers in the STATE_OPEN state -- used by routing */
	struct fd_list	 p_expiry; 	/* list of expiring peers, ordered by their timeout value */
	struct timespec	 p_exp_timer;	/* Timestamp where the peer will expire; updated each time activity is seen on the peer (except DW) */
	
	/* Some flags influencing the peer state machine */
	struct {
		unsigned pf_responder	: 1;	/* The peer has been created to handle incoming connection */
		unsigned pf_delete	: 1;	/* Destroy the peer when the connection is terminated */
		unsigned pf_localterm	: 1;	/* If the latest DPR/DPA was initiated from this side */
		
		unsigned pf_dw_pending 	: 1;	/* A DWR message was sent and not answered yet */
		
		unsigned pf_cnx_pb	: 1;	/* The peer was disconnected because of watchdogs; must exchange 3 watchdogs before putting back to normal */
		unsigned pf_reopen_cnt	: 2;	/* remaining DW to be exchanged after re-established connection */
		
	}		 p_flags;
	
	/* The events queue, peer state machine thread, timer for states timeouts */
	struct fifo	*p_events;	/* The mutex of this FIFO list protects also the state and timer information */
	pthread_t	 p_psm;
	struct timespec	 p_psm_timer;
	
	/* Outgoing message queue, and thread managing sending the messages */
	struct fifo	*p_tosend;
	pthread_t	 p_outthr;
	
	/* The next hop-by-hop id value for the link, only read & modified by p_outthr */
	uint32_t	 p_hbh;
	
	/* Sent requests (for fallback), list of struct sentreq ordered by hbh */
	struct sr_list	 p_sr;
	struct fifo	*p_tofailover;
	
	/* Pending received requests not yet answered (count only) */
	long		 p_reqin_count; /* We use p_state_mtx to protect this value */
	
	/* Data for transitional states before the peer is in OPEN state */
	struct {
		struct cnxctx * p_receiver;	/* Only used in case of election */
		struct msg    * p_cer;		/* Only used in case of election */
		
		pthread_t	p_ini_thr;	/* Initiator thread for establishing a connection */
		struct fd_list  p_connparams;	/* The list of connection attempts, see p_cnx.c */
	};
	
	/* connection context: socket and related information */
	struct cnxctx	*p_cnxctx;
	
	/* Callback for peer validation after the handshake */
	int		(*p_cb2)(struct peer_info *);
	
	/* Callback on initial connection success / failure after the peer was added */
	void 		(*p_cb)(struct peer_info *, void *);
	void 		*p_cb_data;
	
};
#define CHECK_PEER( _p ) \
	(((_p) != NULL) && (((struct fd_peer *)(_p))->p_eyec == EYEC_PEER))

#define fd_peer_getstate(peer)  fd_peer_get_state((struct peer_hdr *)(peer))


/* Events codespace for struct fd_peer->p_events */
enum {
	/* request to terminate this peer : disconnect, requeue all messages */
	FDEVP_TERMINATE = 1500
	
	/* A connection object has received a message. (data contains the buffer + padding + struct fd_msg_pmdl) */
	,FDEVP_CNX_MSG_RECV
			 
	/* A connection object has encountered an error (disconnected). */
	,FDEVP_CNX_ERROR
	
	/* Endpoints of a connection have been changed (multihomed SCTP). */
	,FDEVP_CNX_EP_CHANGE
	
	/* The connection is being shutdown (SCTP notification). */
	,FDEVP_CNX_SHUTDOWN
	
	/* A new connection (with a CER) has been received */
	,FDEVP_CNX_INCOMING
	
	/* A new connection has been established to the remote peer (event data is the cnxctx object) */
	,FDEVP_CNX_ESTABLISHED
	
	/* A connection attempt (initiator side) has failed */
	,FDEVP_CNX_FAILED
	
	/* The PSM state is expired */
	,FDEVP_PSM_TIMEOUT
	
};
#define CHECK_PEVENT( _e ) \
	(((int)(_e) >= FDEVP_TERMINATE) && ((int)(_e) <= FDEVP_PSM_TIMEOUT))
/* The following macro is actually called in p_psm.c -- another solution would be to declare it static inline */
#define DECLARE_PEV_STR()				\
const char * fd_pev_str(int event)			\
{							\
	switch (event) {				\
		case_str(FDEVP_TERMINATE);		\
		case_str(FDEVP_CNX_MSG_RECV);		\
		case_str(FDEVP_CNX_ERROR);		\
		case_str(FDEVP_CNX_EP_CHANGE);		\
		case_str(FDEVP_CNX_INCOMING);		\
		case_str(FDEVP_CNX_ESTABLISHED);	\
		case_str(FDEVP_CNX_FAILED);		\
		case_str(FDEVP_PSM_TIMEOUT);		\
	}						\
	TRACE_DEBUG(FULL, "Unknown event : %d", event);	\
	return "Unknown event";				\
}
const char * fd_pev_str(int event);

/* The data structure for FDEVP_CNX_INCOMING event */
struct cnx_incoming {
	struct msg	* cer;		/* the CER message received on this connection */
	struct cnxctx	* cnx;		/* The connection context */
	int  		  validate;	/* The peer is new, it must be validated (by an extension) or error CEA to be sent */
};

/* Functions */
int  fd_peer_fini();
int  fd_peer_alloc(struct fd_peer ** ptr);
int  fd_peer_free(struct fd_peer ** ptr);
int fd_peer_handle_newCER( struct msg ** cer, struct cnxctx ** cnx );
/* fd_peer_add declared in freeDiameter.h */
int fd_peer_validate( struct fd_peer * peer );
void fd_peer_failover_msg(struct fd_peer * peer);

/* Peer expiry */
int fd_p_expi_init(void);
int fd_p_expi_fini(void);
int fd_p_expi_update(struct fd_peer * peer );

/* Peer state machine */
int  fd_psm_start();
int  fd_psm_begin(struct fd_peer * peer );
int  fd_psm_terminate(struct fd_peer * peer, char * reason );
void fd_psm_abord(struct fd_peer * peer );
void fd_psm_next_timeout(struct fd_peer * peer, int add_random, int delay);
int fd_psm_change_state(struct fd_peer * peer, int new_state);
void fd_psm_cleanup(struct fd_peer * peer, int terminate);

/* Peer out */
int fd_out_send(struct msg ** msg, struct cnxctx * cnx, struct fd_peer * peer, int update_reqin_cnt);
int fd_out_start(struct fd_peer * peer);
int fd_out_stop(struct fd_peer * peer);

/* Initiating connections */
int fd_p_cnx_init(struct fd_peer * peer);
void fd_p_cnx_abort(struct fd_peer * peer, int cleanup_all);

/* Peer sent requests cache */
int fd_p_sr_store(struct sr_list * srlist, struct msg **req, uint32_t *hbhloc, uint32_t hbh_restore);
int fd_p_sr_fetch(struct sr_list * srlist, uint32_t hbh, struct msg **req);
int fd_p_sr_start(struct sr_list * srlist);
int fd_p_sr_stop(struct sr_list * srlist);
void fd_p_sr_failover(struct sr_list * srlist);

/* Local Link messages (CER/CEA, DWR/DWA, DPR/DPA) */
int fd_p_ce_msgrcv(struct msg ** msg, int req, struct fd_peer * peer);
int fd_p_ce_handle_newCER(struct msg ** msg, struct fd_peer * peer, struct cnxctx ** cnx, int valid);
int fd_p_ce_handle_newcnx(struct fd_peer * peer, struct cnxctx * initiator);
int fd_p_ce_process_receiver(struct fd_peer * peer);
void fd_p_ce_clear_cnx(struct fd_peer * peer, struct cnxctx ** cnx_kept);
int fd_p_dw_handle(struct msg ** msg, int req, struct fd_peer * peer);
int fd_p_dw_timeout(struct fd_peer * peer);
int fd_p_dw_reopen(struct fd_peer * peer);
int fd_p_dp_handle(struct msg ** msg, int req, struct fd_peer * peer);
int fd_p_dp_initiate(struct fd_peer * peer, char * reason);
int fd_p_dp_newdelay(struct fd_peer * peer);

/* Active peers -- routing process should only ever take the read lock, the write lock is managed by PSMs */
extern struct fd_list fd_g_activ_peers;
extern pthread_rwlock_t fd_g_activ_peers_rw; /* protect the list */


/* Server sockets */
int  fd_servers_start();
int  fd_servers_stop();

/* Connection contexts -- there are also definitions in cnxctx.h for the relevant files */
struct cnxctx * fd_cnx_serv_tcp(uint16_t port, int family, struct fd_endpoint * ep);
struct cnxctx * fd_cnx_serv_sctp(uint16_t port, struct fd_list * ep_list);
int             fd_cnx_serv_listen(struct cnxctx * conn);
struct cnxctx * fd_cnx_serv_accept(struct cnxctx * serv);
struct cnxctx * fd_cnx_cli_connect_tcp(sSA * sa, socklen_t addrlen);
struct cnxctx * fd_cnx_cli_connect_sctp(int no_ip6, uint16_t port, struct fd_list * list);
int             fd_cnx_start_clear(struct cnxctx * conn, int loop);
void		fd_cnx_sethostname(struct cnxctx * conn, DiamId_t hn);
int		fd_cnx_proto_info(struct cnxctx * conn, char * buf, size_t len);
int		fd_cnx_remote_ip_port(struct cnxctx * conn, char * buf, size_t len, unsigned short * port);
int		fd_cnx_local_ip_port(struct cnxctx * conn, char * buf, size_t len, unsigned short * port);
#define ALGO_HANDSHAKE_DEFAULT	0 /* TLS for TCP, DTLS for SCTP */
#define ALGO_HANDSHAKE_3436	1 /* For TLS for SCTP also */
int             fd_cnx_handshake(struct cnxctx * conn, int mode, int algo, char * priority, void * alt_creds);
char *          fd_cnx_getid(struct cnxctx * conn);
int		fd_cnx_getproto(struct cnxctx * conn);
int		fd_cnx_getTLS(struct cnxctx * conn);
int		fd_cnx_is_unordered_delivery_supported(struct cnxctx * conn);
int		fd_cnx_unordered_delivery(struct cnxctx * conn, int is_allowed);
int             fd_cnx_getcred(struct cnxctx * conn, const gnutls_datum_t **cert_list, unsigned int *cert_list_size);
int 		fd_cnx_get_local_eps(struct fd_list * list);
int             fd_cnx_getremoteeps(struct cnxctx * conn, struct fd_list * eps);
char *          fd_cnx_getremoteid(struct cnxctx * conn);
int             fd_cnx_receive(struct cnxctx * conn, struct timespec * timeout, unsigned char **buf, size_t * len);
int             fd_cnx_recv_setaltfifo(struct cnxctx * conn, struct fifo * alt_fifo); /* send FDEVP_CNX_MSG_RECV event to the fifo list */
int             fd_cnx_send(struct cnxctx * conn, unsigned char * buf, size_t len);
void            fd_cnx_destroy(struct cnxctx * conn);
#ifdef GNUTLS_VERSION_300
int             fd_tls_verify_credentials_2(gnutls_session_t session);
#endif /* GNUTLS_VERSION_300 */

/* Internal calls of the hook mechanism */
void   fd_hook_call(enum fd_hook_type type, struct msg * msg, struct fd_peer * peer, void * other, struct fd_msg_pmdl * pmdl);
void   fd_hook_associate(struct msg * msg, struct fd_msg_pmdl * pmdl);
int    fd_hooks_init(void);
size_t fd_msg_pmdl_sizewithoverhead(size_t datalen);
struct fd_msg_pmdl * fd_msg_pmdl_get_inbuf(uint8_t * buf, size_t datalen); 

#endif /* _FDCORE_INTERNAL_H */

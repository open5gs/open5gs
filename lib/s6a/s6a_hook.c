#define TRACE_MODULE _s6a_hook

#include "core_debug.h"

#include "s6a_lib.h"

static struct fd_hook_hdl *s6a_hook_hdl = NULL;
static char * buf = NULL;
static size_t len;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static s6a_hook_user_handler s6a_hook_user_handler_instance = NULL;

/* The callback called when messages are received and sent */
static void s6a_hook_cb_tree(enum fd_hook_type type, struct msg * msg, 
    struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, 
    void * regdata)
{
	char * peer_name = peer ? peer->info.pi_diamid : "<unknown peer>";
	
	CHECK_POSIX_DO( pthread_mutex_lock(&mtx), );

    if (s6a_hook_user_handler_instance)
        s6a_hook_user_handler_instance(type, msg, peer, other, pmd, regdata);
	
	if (msg) {
		CHECK_MALLOC_DO( fd_msg_dump_treeview(&buf, &len, NULL, msg, fd_g_config->cnf_dict, (type == HOOK_MESSAGE_PARSING_ERROR) ? 0 : 1, 1), 
			{ LOG_E("Error while dumping a message"); pthread_mutex_unlock(&mtx); return; } );
	}
	
	switch (type) {
/* errors */
	case HOOK_MESSAGE_FAILOVER:
		LOG_E("FAILOVER from '%s':", peer_name);
		LOG_SPLIT(FD_LOG_ERROR, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_PARSING_ERROR:
		if (msg) {
			DiamId_t id = NULL;
			if (fd_msg_source_get( msg, &id, NULL ))
				id = (DiamId_t)"<error getting source>";
			if (!id)
				id = (DiamId_t)"<local>";
			LOG_E("PARSING ERROR: '%s' from '%s': ", (char *)other, (char *)id);
			LOG_SPLIT(FD_LOG_ERROR, "     ", buf, NULL);
		} else {
			struct fd_cnx_rcvdata *rcv_data = other;
			CHECK_MALLOC_DO(fd_dump_extend_hexdump(&buf, &len, NULL, rcv_data->buffer, rcv_data->length, 0, 0), break);
			LOG_E("PARSING ERROR: %zdB msg from '%s': %s", rcv_data->length, peer_name, buf);
		}
		break;
	case HOOK_MESSAGE_ROUTING_ERROR:
		LOG_E("ROUTING ERROR '%s' for: ", (char *)other);
		LOG_SPLIT(FD_LOG_ERROR, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_DROPPED:
		LOG_E("DROPPED '%s'", (char *)other);
		LOG_SPLIT(FD_LOG_ERROR, "     ", buf, NULL);
		break;
	
/* send receive */
	case HOOK_MESSAGE_RECEIVED:
		LOG_N("RCV from '%s':", peer_name);
		LOG_SPLIT(FD_LOG_DEBUG, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_SENT:
		LOG_N("SND to '%s':", peer_name);
		LOG_SPLIT(FD_LOG_DEBUG, "     ", buf, NULL);
		break;
	
/* routing */
	case HOOK_MESSAGE_LOCAL:
		LOG_N("ISSUED:");
		LOG_SPLIT(FD_LOG_DEBUG, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_ROUTING_FORWARD:
		LOG_D("FORWARDING: %s", buf);
		LOG_SPLIT(FD_LOG_DEBUG, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_ROUTING_LOCAL:
		LOG_D("DISPATCHING: %s", buf);
		LOG_SPLIT(FD_LOG_DEBUG, "     ", buf, NULL);
		break;
	
/* peers */
	case HOOK_PEER_CONNECT_FAILED:
		LOG_N("CONNECT FAILED to %s: %s", peer_name, (char *)other);
		break;
	case HOOK_PEER_CONNECT_SUCCESS:
		{
			char protobuf[40];
			if (peer) {
				CHECK_FCT_DO(fd_peer_cnx_proto_info(peer, protobuf, sizeof(protobuf)), break );
			} else {
				protobuf[0] = '-';
				protobuf[1] = '\0';
			}
			d_info("CONNECTED TO '%s' (%s):", peer_name, protobuf);
			LOG_N("CONNECTED TO '%s' (%s):", peer_name, protobuf);
			LOG_SPLIT(FD_LOG_NOTICE, "     ", buf, NULL);
		}
		break;

/* Not handled */
	case HOOK_DATA_RECEIVED:
		break;
	}
	
	CHECK_POSIX_DO( pthread_mutex_unlock(&mtx), );
}

int s6a_hook_init()
{
	uint32_t mask_errors, mask_sndrcv, mask_routing, mask_peers;
	uint32_t mask_tree;
	
	mask_errors = HOOK_MASK( HOOK_MESSAGE_FAILOVER, HOOK_MESSAGE_PARSING_ERROR,
            HOOK_MESSAGE_ROUTING_ERROR, HOOK_MESSAGE_DROPPED  );
	mask_sndrcv = HOOK_MASK( HOOK_MESSAGE_RECEIVED, HOOK_MESSAGE_SENT );
	mask_routing= HOOK_MASK( HOOK_MESSAGE_LOCAL,
            HOOK_MESSAGE_ROUTING_FORWARD, HOOK_MESSAGE_ROUTING_LOCAL );
	mask_peers  = HOOK_MASK( HOOK_PEER_CONNECT_FAILED,
            HOOK_PEER_CONNECT_SUCCESS );
	
	mask_tree  = mask_errors;
	mask_tree |= mask_sndrcv;
	mask_tree |= mask_routing;
	mask_tree |= mask_peers;
	
    CHECK_FCT( fd_hook_register( 
            mask_tree, s6a_hook_cb_tree, NULL, NULL, &s6a_hook_hdl) );

	return 0;
}

void s6a_hook_final()
{
	if (s6a_hook_hdl) { CHECK_FCT_DO( fd_hook_unregister( s6a_hook_hdl ), ); }
}

void s6a_hook_register(s6a_hook_user_handler instance)
{
    s6a_hook_user_handler_instance = instance;
}

void s6a_hook_unregister(void)
{
    s6a_hook_user_handler_instance = NULL;
}

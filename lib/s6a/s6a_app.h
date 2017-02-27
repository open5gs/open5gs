#ifndef __S6A_APP_H__
#define __S6A_APP_H__

#include "core_errno.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/extension.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Mode for the extension */
#define MODE_SERV	0x1
#define	MODE_CLI	0x2

/* The module configuration */
struct s6a_conf {
    c_uint32_t vendor_id;    /* default 999999 */
    c_uint32_t appli_id;    /* default 123456 */
    int mode;        /* default MODE_SERV | MODE_CLI */
    char *dest_realm;    /* default local realm */
    char *dest_host;    /* default NULL */
    char *user_name;    /* default NULL */
    struct ta_stats {
        c_uint64_t nb_echoed; /* server */
        c_uint64_t nb_sent;   /* client */
        c_uint64_t nb_recv;   /* client */
        c_uint64_t nb_errs;   /* client */
    } stats;
    pthread_mutex_t        stats_lock;
};
extern struct s6a_conf *s6a_conf;

CORE_DECLARE(int) s6a_app_init(void);
void s6a_app_final(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __S6A_APP_H__ */

#if 0
/* Handle incoming messages (server) */
int ta_serv_init(void);
void ta_serv_fini(void);

/* Create outgoing message (client) */
int ta_cli_init(void);
void ta_cli_fini(void);

/* Initialize dictionary definitions */
int ta_dict_init(void);

/* Some global variables for dictionary */
extern struct dict_object * ta_vendor;
extern struct dict_object * ta_appli;
extern struct dict_object * ta_cmd_r;
extern struct dict_object * ta_cmd_a;
extern struct dict_object * ta_avp;
extern struct dict_object * ta_avp_long;

extern struct dict_object * ta_sess_id;
extern struct dict_object * ta_origin_host;
extern struct dict_object * ta_origin_realm;
extern struct dict_object * ta_dest_host;
extern struct dict_object * ta_dest_realm;
extern struct dict_object * ta_user_name;
extern struct dict_object * ta_res_code;
#endif

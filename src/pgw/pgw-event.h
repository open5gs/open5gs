#ifndef __PGW_EVENT_H__
#define __PGW_EVENT_H__

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    PGW_EVT_BASE = OGS_FSM_USER_SIG,

    PGW_EVT_S5C_MESSAGE,
    PGW_EVT_GX_MESSAGE,

    PGW_EVT_TOP,

} pgw_event_e;

typedef struct pgw_event_s {
    int id;
    void *gtpbuf;
    void *gxbuf;
    ogs_index_t sess_index;
    ogs_index_t xact_index;
} pgw_event_t;

void pgw_event_init(void);
void pgw_event_term(void);
void pgw_event_final(void);

pgw_event_t *pgw_event_new(pgw_event_e id);
void pgw_event_free(pgw_event_t *e);

const char *pgw_event_get_name(pgw_event_t *e);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_EVENT_H__ */

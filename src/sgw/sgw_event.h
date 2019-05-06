#ifndef __SGW_EVENT_H__
#define __SGW_EVENT_H__

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    SGW_EVT_BASE = OGS_FSM_USER_SIG,

    SGW_EVT_S11_MESSAGE,
    SGW_EVT_S5C_MESSAGE,

    SGW_EVT_LO_DLDATA_NOTI,

    SGW_EVT_TOP,

} sgw_event_e;

typedef struct sgw_event_s {
    int id;
    void *pkbuf;
    void *bearer;
} sgw_event_t;

void sgw_event_init(void);
void sgw_event_term(void);
void sgw_event_final(void);

sgw_event_t *sgw_event_new(sgw_event_e id);
void sgw_event_free(sgw_event_t *e);

const char *sgw_event_get_name(sgw_event_t *e);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_EVENT_H__ */

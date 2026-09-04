#ifndef AANF_EVENT_H
#define AANF_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct aanf_ue_s aanf_ue_t;

typedef struct aanf_event_s {
    ogs_event_t h;

    ogs_pool_id_t aanf_ue_id;
} aanf_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(aanf_event_t));

aanf_event_t *aanf_event_new(int id);

const char *aanf_event_get_name(aanf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* AANF_EVENT_H */
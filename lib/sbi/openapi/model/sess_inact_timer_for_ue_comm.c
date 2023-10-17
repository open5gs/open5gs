
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sess_inact_timer_for_ue_comm.h"

OpenAPI_sess_inact_timer_for_ue_comm_t *OpenAPI_sess_inact_timer_for_ue_comm_create(
    int n4_sess_id,
    int sess_inactive_timer
)
{
    OpenAPI_sess_inact_timer_for_ue_comm_t *sess_inact_timer_for_ue_comm_local_var = ogs_malloc(sizeof(OpenAPI_sess_inact_timer_for_ue_comm_t));
    ogs_assert(sess_inact_timer_for_ue_comm_local_var);

    sess_inact_timer_for_ue_comm_local_var->n4_sess_id = n4_sess_id;
    sess_inact_timer_for_ue_comm_local_var->sess_inactive_timer = sess_inactive_timer;

    return sess_inact_timer_for_ue_comm_local_var;
}

void OpenAPI_sess_inact_timer_for_ue_comm_free(OpenAPI_sess_inact_timer_for_ue_comm_t *sess_inact_timer_for_ue_comm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sess_inact_timer_for_ue_comm) {
        return;
    }
    ogs_free(sess_inact_timer_for_ue_comm);
}

cJSON *OpenAPI_sess_inact_timer_for_ue_comm_convertToJSON(OpenAPI_sess_inact_timer_for_ue_comm_t *sess_inact_timer_for_ue_comm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sess_inact_timer_for_ue_comm == NULL) {
        ogs_error("OpenAPI_sess_inact_timer_for_ue_comm_convertToJSON() failed [SessInactTimerForUeComm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "n4SessId", sess_inact_timer_for_ue_comm->n4_sess_id) == NULL) {
        ogs_error("OpenAPI_sess_inact_timer_for_ue_comm_convertToJSON() failed [n4_sess_id]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "sessInactiveTimer", sess_inact_timer_for_ue_comm->sess_inactive_timer) == NULL) {
        ogs_error("OpenAPI_sess_inact_timer_for_ue_comm_convertToJSON() failed [sess_inactive_timer]");
        goto end;
    }

end:
    return item;
}

OpenAPI_sess_inact_timer_for_ue_comm_t *OpenAPI_sess_inact_timer_for_ue_comm_parseFromJSON(cJSON *sess_inact_timer_for_ue_commJSON)
{
    OpenAPI_sess_inact_timer_for_ue_comm_t *sess_inact_timer_for_ue_comm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n4_sess_id = NULL;
    cJSON *sess_inactive_timer = NULL;
    n4_sess_id = cJSON_GetObjectItemCaseSensitive(sess_inact_timer_for_ue_commJSON, "n4SessId");
    if (!n4_sess_id) {
        ogs_error("OpenAPI_sess_inact_timer_for_ue_comm_parseFromJSON() failed [n4_sess_id]");
        goto end;
    }
    if (!cJSON_IsNumber(n4_sess_id)) {
        ogs_error("OpenAPI_sess_inact_timer_for_ue_comm_parseFromJSON() failed [n4_sess_id]");
        goto end;
    }

    sess_inactive_timer = cJSON_GetObjectItemCaseSensitive(sess_inact_timer_for_ue_commJSON, "sessInactiveTimer");
    if (!sess_inactive_timer) {
        ogs_error("OpenAPI_sess_inact_timer_for_ue_comm_parseFromJSON() failed [sess_inactive_timer]");
        goto end;
    }
    if (!cJSON_IsNumber(sess_inactive_timer)) {
        ogs_error("OpenAPI_sess_inact_timer_for_ue_comm_parseFromJSON() failed [sess_inactive_timer]");
        goto end;
    }

    sess_inact_timer_for_ue_comm_local_var = OpenAPI_sess_inact_timer_for_ue_comm_create (
        
        n4_sess_id->valuedouble,
        
        sess_inactive_timer->valuedouble
    );

    return sess_inact_timer_for_ue_comm_local_var;
end:
    return NULL;
}

OpenAPI_sess_inact_timer_for_ue_comm_t *OpenAPI_sess_inact_timer_for_ue_comm_copy(OpenAPI_sess_inact_timer_for_ue_comm_t *dst, OpenAPI_sess_inact_timer_for_ue_comm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sess_inact_timer_for_ue_comm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sess_inact_timer_for_ue_comm_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_sess_inact_timer_for_ue_comm_free(dst);
    dst = OpenAPI_sess_inact_timer_for_ue_comm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


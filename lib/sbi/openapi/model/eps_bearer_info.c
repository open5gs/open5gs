
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_bearer_info.h"

OpenAPI_eps_bearer_info_t *OpenAPI_eps_bearer_info_create(
    int ebi,
    char pgw_s8u_fteid,
    char bearer_level_qo_s
    )
{
    OpenAPI_eps_bearer_info_t *eps_bearer_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_eps_bearer_info_t));
    if (!eps_bearer_info_local_var) {
        return NULL;
    }
    eps_bearer_info_local_var->ebi = ebi;
    eps_bearer_info_local_var->pgw_s8u_fteid = pgw_s8u_fteid;
    eps_bearer_info_local_var->bearer_level_qo_s = bearer_level_qo_s;

    return eps_bearer_info_local_var;
}

void OpenAPI_eps_bearer_info_free(OpenAPI_eps_bearer_info_t *eps_bearer_info)
{
    if (NULL == eps_bearer_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(eps_bearer_info);
}

cJSON *OpenAPI_eps_bearer_info_convertToJSON(OpenAPI_eps_bearer_info_t *eps_bearer_info)
{
    cJSON *item = NULL;

    if (eps_bearer_info == NULL) {
        ogs_error("OpenAPI_eps_bearer_info_convertToJSON() failed [EpsBearerInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!eps_bearer_info->ebi) {
        ogs_error("OpenAPI_eps_bearer_info_convertToJSON() failed [ebi]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "ebi", eps_bearer_info->ebi) == NULL) {
        ogs_error("OpenAPI_eps_bearer_info_convertToJSON() failed [ebi]");
        goto end;
    }

    if (!eps_bearer_info->pgw_s8u_fteid) {
        ogs_error("OpenAPI_eps_bearer_info_convertToJSON() failed [pgw_s8u_fteid]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "pgwS8uFteid", eps_bearer_info->pgw_s8u_fteid) == NULL) {
        ogs_error("OpenAPI_eps_bearer_info_convertToJSON() failed [pgw_s8u_fteid]");
        goto end;
    }

    if (!eps_bearer_info->bearer_level_qo_s) {
        ogs_error("OpenAPI_eps_bearer_info_convertToJSON() failed [bearer_level_qo_s]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "bearerLevelQoS", eps_bearer_info->bearer_level_qo_s) == NULL) {
        ogs_error("OpenAPI_eps_bearer_info_convertToJSON() failed [bearer_level_qo_s]");
        goto end;
    }

end:
    return item;
}

OpenAPI_eps_bearer_info_t *OpenAPI_eps_bearer_info_parseFromJSON(cJSON *eps_bearer_infoJSON)
{
    OpenAPI_eps_bearer_info_t *eps_bearer_info_local_var = NULL;
    cJSON *ebi = cJSON_GetObjectItemCaseSensitive(eps_bearer_infoJSON, "ebi");
    if (!ebi) {
        ogs_error("OpenAPI_eps_bearer_info_parseFromJSON() failed [ebi]");
        goto end;
    }


    if (!cJSON_IsNumber(ebi)) {
        ogs_error("OpenAPI_eps_bearer_info_parseFromJSON() failed [ebi]");
        goto end;
    }

    cJSON *pgw_s8u_fteid = cJSON_GetObjectItemCaseSensitive(eps_bearer_infoJSON, "pgwS8uFteid");
    if (!pgw_s8u_fteid) {
        ogs_error("OpenAPI_eps_bearer_info_parseFromJSON() failed [pgw_s8u_fteid]");
        goto end;
    }


    if (!cJSON_IsNumber(pgw_s8u_fteid)) {
        ogs_error("OpenAPI_eps_bearer_info_parseFromJSON() failed [pgw_s8u_fteid]");
        goto end;
    }

    cJSON *bearer_level_qo_s = cJSON_GetObjectItemCaseSensitive(eps_bearer_infoJSON, "bearerLevelQoS");
    if (!bearer_level_qo_s) {
        ogs_error("OpenAPI_eps_bearer_info_parseFromJSON() failed [bearer_level_qo_s]");
        goto end;
    }


    if (!cJSON_IsNumber(bearer_level_qo_s)) {
        ogs_error("OpenAPI_eps_bearer_info_parseFromJSON() failed [bearer_level_qo_s]");
        goto end;
    }

    eps_bearer_info_local_var = OpenAPI_eps_bearer_info_create (
        ebi->valuedouble,
        pgw_s8u_fteid->valueint,
        bearer_level_qo_s->valueint
        );

    return eps_bearer_info_local_var;
end:
    return NULL;
}

OpenAPI_eps_bearer_info_t *OpenAPI_eps_bearer_info_copy(OpenAPI_eps_bearer_info_t *dst, OpenAPI_eps_bearer_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eps_bearer_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eps_bearer_info_convertToJSON() failed");
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

    OpenAPI_eps_bearer_info_free(dst);
    dst = OpenAPI_eps_bearer_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


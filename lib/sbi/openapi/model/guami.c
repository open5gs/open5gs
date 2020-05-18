
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "guami.h"

OpenAPI_guami_t *OpenAPI_guami_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *amf_id
    )
{
    OpenAPI_guami_t *guami_local_var = OpenAPI_malloc(sizeof(OpenAPI_guami_t));
    if (!guami_local_var) {
        return NULL;
    }
    guami_local_var->plmn_id = plmn_id;
    guami_local_var->amf_id = amf_id;

    return guami_local_var;
}

void OpenAPI_guami_free(OpenAPI_guami_t *guami)
{
    if (NULL == guami) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_plmn_id_free(guami->plmn_id);
    ogs_free(guami->amf_id);
    ogs_free(guami);
}

cJSON *OpenAPI_guami_convertToJSON(OpenAPI_guami_t *guami)
{
    cJSON *item = NULL;

    if (guami == NULL) {
        ogs_error("OpenAPI_guami_convertToJSON() failed [Guami]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!guami->plmn_id) {
        ogs_error("OpenAPI_guami_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(guami->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_guami_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_guami_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!guami->amf_id) {
        ogs_error("OpenAPI_guami_convertToJSON() failed [amf_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "amfId", guami->amf_id) == NULL) {
        ogs_error("OpenAPI_guami_convertToJSON() failed [amf_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_guami_t *OpenAPI_guami_parseFromJSON(cJSON *guamiJSON)
{
    OpenAPI_guami_t *guami_local_var = NULL;
    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(guamiJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_guami_parseFromJSON() failed [plmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;

    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);

    cJSON *amf_id = cJSON_GetObjectItemCaseSensitive(guamiJSON, "amfId");
    if (!amf_id) {
        ogs_error("OpenAPI_guami_parseFromJSON() failed [amf_id]");
        goto end;
    }


    if (!cJSON_IsString(amf_id)) {
        ogs_error("OpenAPI_guami_parseFromJSON() failed [amf_id]");
        goto end;
    }

    guami_local_var = OpenAPI_guami_create (
        plmn_id_local_nonprim,
        ogs_strdup(amf_id->valuestring)
        );

    return guami_local_var;
end:
    return NULL;
}


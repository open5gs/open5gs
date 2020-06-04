
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_to_pdu_session.h"

OpenAPI_service_to_pdu_session_t *OpenAPI_service_to_pdu_session_create(
    OpenAPI_list_t *ser_ids,
    OpenAPI_pdu_session_type_t *pdu_session_type,
    OpenAPI_ssc_mode_t *ssc_mode,
    OpenAPI_list_t *slice_info,
    OpenAPI_list_t *dnns
    )
{
    OpenAPI_service_to_pdu_session_t *service_to_pdu_session_local_var = OpenAPI_malloc(sizeof(OpenAPI_service_to_pdu_session_t));
    if (!service_to_pdu_session_local_var) {
        return NULL;
    }
    service_to_pdu_session_local_var->ser_ids = ser_ids;
    service_to_pdu_session_local_var->pdu_session_type = pdu_session_type;
    service_to_pdu_session_local_var->ssc_mode = ssc_mode;
    service_to_pdu_session_local_var->slice_info = slice_info;
    service_to_pdu_session_local_var->dnns = dnns;

    return service_to_pdu_session_local_var;
}

void OpenAPI_service_to_pdu_session_free(OpenAPI_service_to_pdu_session_t *service_to_pdu_session)
{
    if (NULL == service_to_pdu_session) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(service_to_pdu_session->ser_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_to_pdu_session->ser_ids);
    OpenAPI_pdu_session_type_free(service_to_pdu_session->pdu_session_type);
    OpenAPI_ssc_mode_free(service_to_pdu_session->ssc_mode);
    OpenAPI_list_for_each(service_to_pdu_session->slice_info, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(service_to_pdu_session->slice_info);
    OpenAPI_list_for_each(service_to_pdu_session->dnns, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_to_pdu_session->dnns);
    ogs_free(service_to_pdu_session);
}

cJSON *OpenAPI_service_to_pdu_session_convertToJSON(OpenAPI_service_to_pdu_session_t *service_to_pdu_session)
{
    cJSON *item = NULL;

    if (service_to_pdu_session == NULL) {
        ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [ServiceToPduSession]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_to_pdu_session->ser_ids) {
        ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [ser_ids]");
        goto end;
    }
    cJSON *ser_ids = cJSON_AddArrayToObject(item, "serIds");
    if (ser_ids == NULL) {
        ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ser_ids_node;
    OpenAPI_list_for_each(service_to_pdu_session->ser_ids, ser_ids_node)  {
        if (cJSON_AddStringToObject(ser_ids, "", (char*)ser_ids_node->data) == NULL) {
            ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [ser_ids]");
            goto end;
        }
    }

    if (service_to_pdu_session->pdu_session_type) {
        cJSON *pdu_session_type_local_JSON = OpenAPI_pdu_session_type_convertToJSON(service_to_pdu_session->pdu_session_type);
        if (pdu_session_type_local_JSON == NULL) {
            ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [pdu_session_type]");
            goto end;
        }
        cJSON_AddItemToObject(item, "pduSessionType", pdu_session_type_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [pdu_session_type]");
            goto end;
        }
    }

    if (service_to_pdu_session->ssc_mode) {
        cJSON *ssc_mode_local_JSON = OpenAPI_ssc_mode_convertToJSON(service_to_pdu_session->ssc_mode);
        if (ssc_mode_local_JSON == NULL) {
            ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [ssc_mode]");
            goto end;
        }
        cJSON_AddItemToObject(item, "sscMode", ssc_mode_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [ssc_mode]");
            goto end;
        }
    }

    if (service_to_pdu_session->slice_info) {
        cJSON *slice_infoList = cJSON_AddArrayToObject(item, "sliceInfo");
        if (slice_infoList == NULL) {
            ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [slice_info]");
            goto end;
        }

        OpenAPI_lnode_t *slice_info_node;
        if (service_to_pdu_session->slice_info) {
            OpenAPI_list_for_each(service_to_pdu_session->slice_info, slice_info_node) {
                cJSON *itemLocal = OpenAPI_snssai_convertToJSON(slice_info_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [slice_info]");
                    goto end;
                }
                cJSON_AddItemToArray(slice_infoList, itemLocal);
            }
        }
    }

    if (service_to_pdu_session->dnns) {
        cJSON *dnns = cJSON_AddArrayToObject(item, "dnns");
        if (dnns == NULL) {
            ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [dnns]");
            goto end;
        }

        OpenAPI_lnode_t *dnns_node;
        OpenAPI_list_for_each(service_to_pdu_session->dnns, dnns_node)  {
            if (cJSON_AddStringToObject(dnns, "", (char*)dnns_node->data) == NULL) {
                ogs_error("OpenAPI_service_to_pdu_session_convertToJSON() failed [dnns]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_service_to_pdu_session_t *OpenAPI_service_to_pdu_session_parseFromJSON(cJSON *service_to_pdu_sessionJSON)
{
    OpenAPI_service_to_pdu_session_t *service_to_pdu_session_local_var = NULL;
    cJSON *ser_ids = cJSON_GetObjectItemCaseSensitive(service_to_pdu_sessionJSON, "serIds");
    if (!ser_ids) {
        ogs_error("OpenAPI_service_to_pdu_session_parseFromJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_list_t *ser_idsList;

    cJSON *ser_ids_local;
    if (!cJSON_IsArray(ser_ids)) {
        ogs_error("OpenAPI_service_to_pdu_session_parseFromJSON() failed [ser_ids]");
        goto end;
    }
    ser_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ser_ids_local, ser_ids) {
        if (!cJSON_IsString(ser_ids_local)) {
            ogs_error("OpenAPI_service_to_pdu_session_parseFromJSON() failed [ser_ids]");
            goto end;
        }
        OpenAPI_list_add(ser_idsList, ogs_strdup(ser_ids_local->valuestring));
    }

    cJSON *pdu_session_type = cJSON_GetObjectItemCaseSensitive(service_to_pdu_sessionJSON, "pduSessionType");

    OpenAPI_pdu_session_type_t *pdu_session_type_local_nonprim = NULL;
    if (pdu_session_type) {
        pdu_session_type_local_nonprim = OpenAPI_pdu_session_type_parseFromJSON(pdu_session_type);
    }

    cJSON *ssc_mode = cJSON_GetObjectItemCaseSensitive(service_to_pdu_sessionJSON, "sscMode");

    OpenAPI_ssc_mode_t *ssc_mode_local_nonprim = NULL;
    if (ssc_mode) {
        ssc_mode_local_nonprim = OpenAPI_ssc_mode_parseFromJSON(ssc_mode);
    }

    cJSON *slice_info = cJSON_GetObjectItemCaseSensitive(service_to_pdu_sessionJSON, "sliceInfo");

    OpenAPI_list_t *slice_infoList;
    if (slice_info) {
        cJSON *slice_info_local_nonprimitive;
        if (!cJSON_IsArray(slice_info)) {
            ogs_error("OpenAPI_service_to_pdu_session_parseFromJSON() failed [slice_info]");
            goto end;
        }

        slice_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(slice_info_local_nonprimitive, slice_info ) {
            if (!cJSON_IsObject(slice_info_local_nonprimitive)) {
                ogs_error("OpenAPI_service_to_pdu_session_parseFromJSON() failed [slice_info]");
                goto end;
            }
            OpenAPI_snssai_t *slice_infoItem = OpenAPI_snssai_parseFromJSON(slice_info_local_nonprimitive);

            OpenAPI_list_add(slice_infoList, slice_infoItem);
        }
    }

    cJSON *dnns = cJSON_GetObjectItemCaseSensitive(service_to_pdu_sessionJSON, "dnns");

    OpenAPI_list_t *dnnsList;
    if (dnns) {
        cJSON *dnns_local;
        if (!cJSON_IsArray(dnns)) {
            ogs_error("OpenAPI_service_to_pdu_session_parseFromJSON() failed [dnns]");
            goto end;
        }
        dnnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnns_local, dnns) {
            if (!cJSON_IsString(dnns_local)) {
                ogs_error("OpenAPI_service_to_pdu_session_parseFromJSON() failed [dnns]");
                goto end;
            }
            OpenAPI_list_add(dnnsList, ogs_strdup(dnns_local->valuestring));
        }
    }

    service_to_pdu_session_local_var = OpenAPI_service_to_pdu_session_create (
        ser_idsList,
        pdu_session_type ? pdu_session_type_local_nonprim : NULL,
        ssc_mode ? ssc_mode_local_nonprim : NULL,
        slice_info ? slice_infoList : NULL,
        dnns ? dnnsList : NULL
        );

    return service_to_pdu_session_local_var;
end:
    return NULL;
}


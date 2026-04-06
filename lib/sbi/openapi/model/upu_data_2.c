
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upu_data_2.h"

OpenAPI_upu_data_2_t *OpenAPI_upu_data_2_create(
    char *sec_packet,
    OpenAPI_list_t *default_conf_nssai,
    char *routing_id,
    bool is_drei,
    int drei,
    bool is_drei_eps,
    int drei_eps,
    bool is_aol,
    int aol,
    OpenAPI_list_t *disaster_cond_plmn_ids
)
{
    OpenAPI_upu_data_2_t *upu_data_2_local_var = ogs_malloc(sizeof(OpenAPI_upu_data_2_t));
    ogs_assert(upu_data_2_local_var);

    upu_data_2_local_var->sec_packet = sec_packet;
    upu_data_2_local_var->default_conf_nssai = default_conf_nssai;
    upu_data_2_local_var->routing_id = routing_id;
    upu_data_2_local_var->is_drei = is_drei;
    upu_data_2_local_var->drei = drei;
    upu_data_2_local_var->is_drei_eps = is_drei_eps;
    upu_data_2_local_var->drei_eps = drei_eps;
    upu_data_2_local_var->is_aol = is_aol;
    upu_data_2_local_var->aol = aol;
    upu_data_2_local_var->disaster_cond_plmn_ids = disaster_cond_plmn_ids;

    return upu_data_2_local_var;
}

void OpenAPI_upu_data_2_free(OpenAPI_upu_data_2_t *upu_data_2)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == upu_data_2) {
        return;
    }
    if (upu_data_2->sec_packet) {
        ogs_free(upu_data_2->sec_packet);
        upu_data_2->sec_packet = NULL;
    }
    if (upu_data_2->default_conf_nssai) {
        OpenAPI_list_for_each(upu_data_2->default_conf_nssai, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(upu_data_2->default_conf_nssai);
        upu_data_2->default_conf_nssai = NULL;
    }
    if (upu_data_2->routing_id) {
        ogs_free(upu_data_2->routing_id);
        upu_data_2->routing_id = NULL;
    }
    if (upu_data_2->disaster_cond_plmn_ids) {
        OpenAPI_list_for_each(upu_data_2->disaster_cond_plmn_ids, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(upu_data_2->disaster_cond_plmn_ids);
        upu_data_2->disaster_cond_plmn_ids = NULL;
    }
    ogs_free(upu_data_2);
}

cJSON *OpenAPI_upu_data_2_convertToJSON(OpenAPI_upu_data_2_t *upu_data_2)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (upu_data_2 == NULL) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [UpuData_2]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (upu_data_2->sec_packet) {
    if (cJSON_AddStringToObject(item, "secPacket", upu_data_2->sec_packet) == NULL) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [sec_packet]");
        goto end;
    }
    }

    if (upu_data_2->default_conf_nssai) {
    cJSON *default_conf_nssaiList = cJSON_AddArrayToObject(item, "defaultConfNssai");
    if (default_conf_nssaiList == NULL) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [default_conf_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(upu_data_2->default_conf_nssai, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [default_conf_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(default_conf_nssaiList, itemLocal);
    }
    }

    if (upu_data_2->routing_id) {
    if (cJSON_AddStringToObject(item, "routingId", upu_data_2->routing_id) == NULL) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [routing_id]");
        goto end;
    }
    }

    if (upu_data_2->is_drei) {
    if (cJSON_AddBoolToObject(item, "drei", upu_data_2->drei) == NULL) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [drei]");
        goto end;
    }
    }

    if (upu_data_2->is_drei_eps) {
    if (cJSON_AddBoolToObject(item, "dreiEps", upu_data_2->drei_eps) == NULL) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [drei_eps]");
        goto end;
    }
    }

    if (upu_data_2->is_aol) {
    if (cJSON_AddBoolToObject(item, "aol", upu_data_2->aol) == NULL) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [aol]");
        goto end;
    }
    }

    if (upu_data_2->disaster_cond_plmn_ids) {
    cJSON *disaster_cond_plmn_idsList = cJSON_AddArrayToObject(item, "disasterCondPlmnIds");
    if (disaster_cond_plmn_idsList == NULL) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [disaster_cond_plmn_ids]");
        goto end;
    }
    OpenAPI_list_for_each(upu_data_2->disaster_cond_plmn_ids, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [disaster_cond_plmn_ids]");
            goto end;
        }
        cJSON_AddItemToArray(disaster_cond_plmn_idsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_upu_data_2_t *OpenAPI_upu_data_2_parseFromJSON(cJSON *upu_data_2JSON)
{
    OpenAPI_upu_data_2_t *upu_data_2_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sec_packet = NULL;
    cJSON *default_conf_nssai = NULL;
    OpenAPI_list_t *default_conf_nssaiList = NULL;
    cJSON *routing_id = NULL;
    cJSON *drei = NULL;
    cJSON *drei_eps = NULL;
    cJSON *aol = NULL;
    cJSON *disaster_cond_plmn_ids = NULL;
    OpenAPI_list_t *disaster_cond_plmn_idsList = NULL;
    sec_packet = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "secPacket");
    if (sec_packet) {
    if (!cJSON_IsString(sec_packet) && !cJSON_IsNull(sec_packet)) {
        ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [sec_packet]");
        goto end;
    }
    }

    default_conf_nssai = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "defaultConfNssai");
    if (default_conf_nssai) {
        cJSON *default_conf_nssai_local = NULL;
        if (!cJSON_IsArray(default_conf_nssai)) {
            ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [default_conf_nssai]");
            goto end;
        }

        default_conf_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(default_conf_nssai_local, default_conf_nssai) {
            if (!cJSON_IsObject(default_conf_nssai_local)) {
                ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [default_conf_nssai]");
                goto end;
            }
            OpenAPI_snssai_t *default_conf_nssaiItem = OpenAPI_snssai_parseFromJSON(default_conf_nssai_local);
            if (!default_conf_nssaiItem) {
                ogs_error("No default_conf_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(default_conf_nssaiList, default_conf_nssaiItem);
        }
    }

    routing_id = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "routingId");
    if (routing_id) {
    if (!cJSON_IsString(routing_id) && !cJSON_IsNull(routing_id)) {
        ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [routing_id]");
        goto end;
    }
    }

    drei = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "drei");
    if (drei) {
    if (!cJSON_IsBool(drei)) {
        ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [drei]");
        goto end;
    }
    }

    drei_eps = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "dreiEps");
    if (drei_eps) {
    if (!cJSON_IsBool(drei_eps)) {
        ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [drei_eps]");
        goto end;
    }
    }

    aol = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "aol");
    if (aol) {
    if (!cJSON_IsBool(aol)) {
        ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [aol]");
        goto end;
    }
    }

    disaster_cond_plmn_ids = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "disasterCondPlmnIds");
    if (disaster_cond_plmn_ids) {
        cJSON *disaster_cond_plmn_ids_local = NULL;
        if (!cJSON_IsArray(disaster_cond_plmn_ids)) {
            ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [disaster_cond_plmn_ids]");
            goto end;
        }

        disaster_cond_plmn_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(disaster_cond_plmn_ids_local, disaster_cond_plmn_ids) {
            if (!cJSON_IsObject(disaster_cond_plmn_ids_local)) {
                ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [disaster_cond_plmn_ids]");
                goto end;
            }
            OpenAPI_plmn_id_t *disaster_cond_plmn_idsItem = OpenAPI_plmn_id_parseFromJSON(disaster_cond_plmn_ids_local);
            if (!disaster_cond_plmn_idsItem) {
                ogs_error("No disaster_cond_plmn_idsItem");
                goto end;
            }
            OpenAPI_list_add(disaster_cond_plmn_idsList, disaster_cond_plmn_idsItem);
        }
    }

    upu_data_2_local_var = OpenAPI_upu_data_2_create (
        sec_packet && !cJSON_IsNull(sec_packet) ? ogs_strdup(sec_packet->valuestring) : NULL,
        default_conf_nssai ? default_conf_nssaiList : NULL,
        routing_id && !cJSON_IsNull(routing_id) ? ogs_strdup(routing_id->valuestring) : NULL,
        drei ? true : false,
        drei ? drei->valueint : 0,
        drei_eps ? true : false,
        drei_eps ? drei_eps->valueint : 0,
        aol ? true : false,
        aol ? aol->valueint : 0,
        disaster_cond_plmn_ids ? disaster_cond_plmn_idsList : NULL
    );

    return upu_data_2_local_var;
end:
    if (default_conf_nssaiList) {
        OpenAPI_list_for_each(default_conf_nssaiList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(default_conf_nssaiList);
        default_conf_nssaiList = NULL;
    }
    if (disaster_cond_plmn_idsList) {
        OpenAPI_list_for_each(disaster_cond_plmn_idsList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(disaster_cond_plmn_idsList);
        disaster_cond_plmn_idsList = NULL;
    }
    return NULL;
}

OpenAPI_upu_data_2_t *OpenAPI_upu_data_2_copy(OpenAPI_upu_data_2_t *dst, OpenAPI_upu_data_2_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_upu_data_2_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed");
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

    OpenAPI_upu_data_2_free(dst);
    dst = OpenAPI_upu_data_2_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


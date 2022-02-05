
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upu_data_2.h"

OpenAPI_upu_data_2_t *OpenAPI_upu_data_2_create(
    char *sec_packet,
    OpenAPI_list_t *default_conf_nssai,
    char *routing_id
)
{
    OpenAPI_upu_data_2_t *upu_data_2_local_var = ogs_malloc(sizeof(OpenAPI_upu_data_2_t));
    ogs_assert(upu_data_2_local_var);

    upu_data_2_local_var->sec_packet = sec_packet;
    upu_data_2_local_var->default_conf_nssai = default_conf_nssai;
    upu_data_2_local_var->routing_id = routing_id;

    return upu_data_2_local_var;
}

void OpenAPI_upu_data_2_free(OpenAPI_upu_data_2_t *upu_data_2)
{
    if (NULL == upu_data_2) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(upu_data_2->sec_packet);
    OpenAPI_list_for_each(upu_data_2->default_conf_nssai, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(upu_data_2->default_conf_nssai);
    ogs_free(upu_data_2->routing_id);
    ogs_free(upu_data_2);
}

cJSON *OpenAPI_upu_data_2_convertToJSON(OpenAPI_upu_data_2_t *upu_data_2)
{
    cJSON *item = NULL;

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

    OpenAPI_lnode_t *default_conf_nssai_node;
    if (upu_data_2->default_conf_nssai) {
        OpenAPI_list_for_each(upu_data_2->default_conf_nssai, default_conf_nssai_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(default_conf_nssai_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [default_conf_nssai]");
                goto end;
            }
            cJSON_AddItemToArray(default_conf_nssaiList, itemLocal);
        }
    }
    }

    if (upu_data_2->routing_id) {
    if (cJSON_AddStringToObject(item, "routingId", upu_data_2->routing_id) == NULL) {
        ogs_error("OpenAPI_upu_data_2_convertToJSON() failed [routing_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_upu_data_2_t *OpenAPI_upu_data_2_parseFromJSON(cJSON *upu_data_2JSON)
{
    OpenAPI_upu_data_2_t *upu_data_2_local_var = NULL;
    cJSON *sec_packet = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "secPacket");

    if (sec_packet) {
    if (!cJSON_IsString(sec_packet)) {
        ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [sec_packet]");
        goto end;
    }
    }

    cJSON *default_conf_nssai = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "defaultConfNssai");

    OpenAPI_list_t *default_conf_nssaiList;
    if (default_conf_nssai) {
    cJSON *default_conf_nssai_local_nonprimitive;
    if (!cJSON_IsArray(default_conf_nssai)){
        ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [default_conf_nssai]");
        goto end;
    }

    default_conf_nssaiList = OpenAPI_list_create();

    cJSON_ArrayForEach(default_conf_nssai_local_nonprimitive, default_conf_nssai ) {
        if (!cJSON_IsObject(default_conf_nssai_local_nonprimitive)) {
            ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [default_conf_nssai]");
            goto end;
        }
        OpenAPI_snssai_t *default_conf_nssaiItem = OpenAPI_snssai_parseFromJSON(default_conf_nssai_local_nonprimitive);

        if (!default_conf_nssaiItem) {
            ogs_error("No default_conf_nssaiItem");
            OpenAPI_list_free(default_conf_nssaiList);
            goto end;
        }

        OpenAPI_list_add(default_conf_nssaiList, default_conf_nssaiItem);
    }
    }

    cJSON *routing_id = cJSON_GetObjectItemCaseSensitive(upu_data_2JSON, "routingId");

    if (routing_id) {
    if (!cJSON_IsString(routing_id)) {
        ogs_error("OpenAPI_upu_data_2_parseFromJSON() failed [routing_id]");
        goto end;
    }
    }

    upu_data_2_local_var = OpenAPI_upu_data_2_create (
        sec_packet ? ogs_strdup(sec_packet->valuestring) : NULL,
        default_conf_nssai ? default_conf_nssaiList : NULL,
        routing_id ? ogs_strdup(routing_id->valuestring) : NULL
    );

    return upu_data_2_local_var;
end:
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



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sepp_info.h"

OpenAPI_sepp_info_t *OpenAPI_sepp_info_create(
    char *sepp_prefix,
    OpenAPI_list_t* sepp_ports,
    OpenAPI_list_t *remote_plmn_list,
    OpenAPI_list_t *remote_snpn_list
)
{
    OpenAPI_sepp_info_t *sepp_info_local_var = ogs_malloc(sizeof(OpenAPI_sepp_info_t));
    ogs_assert(sepp_info_local_var);

    sepp_info_local_var->sepp_prefix = sepp_prefix;
    sepp_info_local_var->sepp_ports = sepp_ports;
    sepp_info_local_var->remote_plmn_list = remote_plmn_list;
    sepp_info_local_var->remote_snpn_list = remote_snpn_list;

    return sepp_info_local_var;
}

void OpenAPI_sepp_info_free(OpenAPI_sepp_info_t *sepp_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sepp_info) {
        return;
    }
    if (sepp_info->sepp_prefix) {
        ogs_free(sepp_info->sepp_prefix);
        sepp_info->sepp_prefix = NULL;
    }
    if (sepp_info->sepp_ports) {
        OpenAPI_list_for_each(sepp_info->sepp_ports, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sepp_info->sepp_ports);
        sepp_info->sepp_ports = NULL;
    }
    if (sepp_info->remote_plmn_list) {
        OpenAPI_list_for_each(sepp_info->remote_plmn_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(sepp_info->remote_plmn_list);
        sepp_info->remote_plmn_list = NULL;
    }
    if (sepp_info->remote_snpn_list) {
        OpenAPI_list_for_each(sepp_info->remote_snpn_list, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(sepp_info->remote_snpn_list);
        sepp_info->remote_snpn_list = NULL;
    }
    ogs_free(sepp_info);
}

cJSON *OpenAPI_sepp_info_convertToJSON(OpenAPI_sepp_info_t *sepp_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sepp_info == NULL) {
        ogs_error("OpenAPI_sepp_info_convertToJSON() failed [SeppInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sepp_info->sepp_prefix) {
    if (cJSON_AddStringToObject(item, "seppPrefix", sepp_info->sepp_prefix) == NULL) {
        ogs_error("OpenAPI_sepp_info_convertToJSON() failed [sepp_prefix]");
        goto end;
    }
    }

    if (sepp_info->sepp_ports) {
    cJSON *sepp_ports = cJSON_AddObjectToObject(item, "seppPorts");
    if (sepp_ports == NULL) {
        ogs_error("OpenAPI_sepp_info_convertToJSON() failed [sepp_ports]");
        goto end;
    }
    cJSON *localMapObject = sepp_ports;
    if (sepp_info->sepp_ports) {
        OpenAPI_list_for_each(sepp_info->sepp_ports, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sepp_info_convertToJSON() failed [sepp_ports]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sepp_info_convertToJSON() failed [sepp_ports]");
                goto end;
            }
            if (localKeyValue->value == NULL) {
                ogs_error("OpenAPI_sepp_info_convertToJSON() failed [inner]");
                goto end;
            }
            if (cJSON_AddNumberToObject(localMapObject, localKeyValue->key, *(double *)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_sepp_info_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

    if (sepp_info->remote_plmn_list) {
    cJSON *remote_plmn_listList = cJSON_AddArrayToObject(item, "remotePlmnList");
    if (remote_plmn_listList == NULL) {
        ogs_error("OpenAPI_sepp_info_convertToJSON() failed [remote_plmn_list]");
        goto end;
    }
    OpenAPI_list_for_each(sepp_info->remote_plmn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sepp_info_convertToJSON() failed [remote_plmn_list]");
            goto end;
        }
        cJSON_AddItemToArray(remote_plmn_listList, itemLocal);
    }
    }

    if (sepp_info->remote_snpn_list) {
    cJSON *remote_snpn_listList = cJSON_AddArrayToObject(item, "remoteSnpnList");
    if (remote_snpn_listList == NULL) {
        ogs_error("OpenAPI_sepp_info_convertToJSON() failed [remote_snpn_list]");
        goto end;
    }
    OpenAPI_list_for_each(sepp_info->remote_snpn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sepp_info_convertToJSON() failed [remote_snpn_list]");
            goto end;
        }
        cJSON_AddItemToArray(remote_snpn_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_sepp_info_t *OpenAPI_sepp_info_parseFromJSON(cJSON *sepp_infoJSON)
{
    OpenAPI_sepp_info_t *sepp_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sepp_prefix = NULL;
    cJSON *sepp_ports = NULL;
    OpenAPI_list_t *sepp_portsList = NULL;
    cJSON *remote_plmn_list = NULL;
    OpenAPI_list_t *remote_plmn_listList = NULL;
    cJSON *remote_snpn_list = NULL;
    OpenAPI_list_t *remote_snpn_listList = NULL;
    sepp_prefix = cJSON_GetObjectItemCaseSensitive(sepp_infoJSON, "seppPrefix");
    if (sepp_prefix) {
    if (!cJSON_IsString(sepp_prefix) && !cJSON_IsNull(sepp_prefix)) {
        ogs_error("OpenAPI_sepp_info_parseFromJSON() failed [sepp_prefix]");
        goto end;
    }
    }

    sepp_ports = cJSON_GetObjectItemCaseSensitive(sepp_infoJSON, "seppPorts");
    if (sepp_ports) {
        cJSON *sepp_ports_local_map = NULL;
        if (!cJSON_IsObject(sepp_ports) && !cJSON_IsNull(sepp_ports)) {
            ogs_error("OpenAPI_sepp_info_parseFromJSON() failed [sepp_ports]");
            goto end;
        }
        if (cJSON_IsObject(sepp_ports)) {
            sepp_portsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(sepp_ports_local_map, sepp_ports) {
                cJSON *localMapObject = sepp_ports_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsNumber(localMapObject)) {
                    ogs_error("OpenAPI_sepp_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                localDouble = (double *)ogs_calloc(1, sizeof(double));
                if (!localDouble) {
                    ogs_error("OpenAPI_sepp_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                *localDouble = localMapObject->valuedouble;
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), localDouble);
                OpenAPI_list_add(sepp_portsList, localMapKeyPair);
            }
        }
    }

    remote_plmn_list = cJSON_GetObjectItemCaseSensitive(sepp_infoJSON, "remotePlmnList");
    if (remote_plmn_list) {
        cJSON *remote_plmn_list_local = NULL;
        if (!cJSON_IsArray(remote_plmn_list)) {
            ogs_error("OpenAPI_sepp_info_parseFromJSON() failed [remote_plmn_list]");
            goto end;
        }

        remote_plmn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(remote_plmn_list_local, remote_plmn_list) {
            if (!cJSON_IsObject(remote_plmn_list_local)) {
                ogs_error("OpenAPI_sepp_info_parseFromJSON() failed [remote_plmn_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *remote_plmn_listItem = OpenAPI_plmn_id_parseFromJSON(remote_plmn_list_local);
            if (!remote_plmn_listItem) {
                ogs_error("No remote_plmn_listItem");
                goto end;
            }
            OpenAPI_list_add(remote_plmn_listList, remote_plmn_listItem);
        }
    }

    remote_snpn_list = cJSON_GetObjectItemCaseSensitive(sepp_infoJSON, "remoteSnpnList");
    if (remote_snpn_list) {
        cJSON *remote_snpn_list_local = NULL;
        if (!cJSON_IsArray(remote_snpn_list)) {
            ogs_error("OpenAPI_sepp_info_parseFromJSON() failed [remote_snpn_list]");
            goto end;
        }

        remote_snpn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(remote_snpn_list_local, remote_snpn_list) {
            if (!cJSON_IsObject(remote_snpn_list_local)) {
                ogs_error("OpenAPI_sepp_info_parseFromJSON() failed [remote_snpn_list]");
                goto end;
            }
            OpenAPI_plmn_id_nid_t *remote_snpn_listItem = OpenAPI_plmn_id_nid_parseFromJSON(remote_snpn_list_local);
            if (!remote_snpn_listItem) {
                ogs_error("No remote_snpn_listItem");
                goto end;
            }
            OpenAPI_list_add(remote_snpn_listList, remote_snpn_listItem);
        }
    }

    sepp_info_local_var = OpenAPI_sepp_info_create (
        sepp_prefix && !cJSON_IsNull(sepp_prefix) ? ogs_strdup(sepp_prefix->valuestring) : NULL,
        sepp_ports ? sepp_portsList : NULL,
        remote_plmn_list ? remote_plmn_listList : NULL,
        remote_snpn_list ? remote_snpn_listList : NULL
    );

    return sepp_info_local_var;
end:
    if (sepp_portsList) {
        OpenAPI_list_for_each(sepp_portsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sepp_portsList);
        sepp_portsList = NULL;
    }
    if (remote_plmn_listList) {
        OpenAPI_list_for_each(remote_plmn_listList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(remote_plmn_listList);
        remote_plmn_listList = NULL;
    }
    if (remote_snpn_listList) {
        OpenAPI_list_for_each(remote_snpn_listList, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(remote_snpn_listList);
        remote_snpn_listList = NULL;
    }
    return NULL;
}

OpenAPI_sepp_info_t *OpenAPI_sepp_info_copy(OpenAPI_sepp_info_t *dst, OpenAPI_sepp_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sepp_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sepp_info_convertToJSON() failed");
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

    OpenAPI_sepp_info_free(dst);
    dst = OpenAPI_sepp_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


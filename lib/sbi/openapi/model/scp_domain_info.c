
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scp_domain_info.h"

OpenAPI_scp_domain_info_t *OpenAPI_scp_domain_info_create(
    char *scp_fqdn,
    OpenAPI_list_t *scp_ip_end_points,
    char *scp_prefix,
    OpenAPI_list_t* scp_ports
)
{
    OpenAPI_scp_domain_info_t *scp_domain_info_local_var = ogs_malloc(sizeof(OpenAPI_scp_domain_info_t));
    ogs_assert(scp_domain_info_local_var);

    scp_domain_info_local_var->scp_fqdn = scp_fqdn;
    scp_domain_info_local_var->scp_ip_end_points = scp_ip_end_points;
    scp_domain_info_local_var->scp_prefix = scp_prefix;
    scp_domain_info_local_var->scp_ports = scp_ports;

    return scp_domain_info_local_var;
}

void OpenAPI_scp_domain_info_free(OpenAPI_scp_domain_info_t *scp_domain_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == scp_domain_info) {
        return;
    }
    if (scp_domain_info->scp_fqdn) {
        ogs_free(scp_domain_info->scp_fqdn);
        scp_domain_info->scp_fqdn = NULL;
    }
    if (scp_domain_info->scp_ip_end_points) {
        OpenAPI_list_for_each(scp_domain_info->scp_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(scp_domain_info->scp_ip_end_points);
        scp_domain_info->scp_ip_end_points = NULL;
    }
    if (scp_domain_info->scp_prefix) {
        ogs_free(scp_domain_info->scp_prefix);
        scp_domain_info->scp_prefix = NULL;
    }
    if (scp_domain_info->scp_ports) {
        OpenAPI_list_for_each(scp_domain_info->scp_ports, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(scp_domain_info->scp_ports);
        scp_domain_info->scp_ports = NULL;
    }
    ogs_free(scp_domain_info);
}

cJSON *OpenAPI_scp_domain_info_convertToJSON(OpenAPI_scp_domain_info_t *scp_domain_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (scp_domain_info == NULL) {
        ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [ScpDomainInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (scp_domain_info->scp_fqdn) {
    if (cJSON_AddStringToObject(item, "scpFqdn", scp_domain_info->scp_fqdn) == NULL) {
        ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [scp_fqdn]");
        goto end;
    }
    }

    if (scp_domain_info->scp_ip_end_points) {
    cJSON *scp_ip_end_pointsList = cJSON_AddArrayToObject(item, "scpIpEndPoints");
    if (scp_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [scp_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(scp_domain_info->scp_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [scp_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(scp_ip_end_pointsList, itemLocal);
    }
    }

    if (scp_domain_info->scp_prefix) {
    if (cJSON_AddStringToObject(item, "scpPrefix", scp_domain_info->scp_prefix) == NULL) {
        ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [scp_prefix]");
        goto end;
    }
    }

    if (scp_domain_info->scp_ports) {
    cJSON *scp_ports = cJSON_AddObjectToObject(item, "scpPorts");
    if (scp_ports == NULL) {
        ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [scp_ports]");
        goto end;
    }
    cJSON *localMapObject = scp_ports;
    if (scp_domain_info->scp_ports) {
        OpenAPI_list_for_each(scp_domain_info->scp_ports, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [scp_ports]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [scp_ports]");
                goto end;
            }
            if (localKeyValue->value == NULL) {
                ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [inner]");
                goto end;
            }
            if (cJSON_AddNumberToObject(localMapObject, localKeyValue->key, *(double *)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

end:
    return item;
}

OpenAPI_scp_domain_info_t *OpenAPI_scp_domain_info_parseFromJSON(cJSON *scp_domain_infoJSON)
{
    OpenAPI_scp_domain_info_t *scp_domain_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *scp_fqdn = NULL;
    cJSON *scp_ip_end_points = NULL;
    OpenAPI_list_t *scp_ip_end_pointsList = NULL;
    cJSON *scp_prefix = NULL;
    cJSON *scp_ports = NULL;
    OpenAPI_list_t *scp_portsList = NULL;
    scp_fqdn = cJSON_GetObjectItemCaseSensitive(scp_domain_infoJSON, "scpFqdn");
    if (scp_fqdn) {
    if (!cJSON_IsString(scp_fqdn) && !cJSON_IsNull(scp_fqdn)) {
        ogs_error("OpenAPI_scp_domain_info_parseFromJSON() failed [scp_fqdn]");
        goto end;
    }
    }

    scp_ip_end_points = cJSON_GetObjectItemCaseSensitive(scp_domain_infoJSON, "scpIpEndPoints");
    if (scp_ip_end_points) {
        cJSON *scp_ip_end_points_local = NULL;
        if (!cJSON_IsArray(scp_ip_end_points)) {
            ogs_error("OpenAPI_scp_domain_info_parseFromJSON() failed [scp_ip_end_points]");
            goto end;
        }

        scp_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(scp_ip_end_points_local, scp_ip_end_points) {
            if (!cJSON_IsObject(scp_ip_end_points_local)) {
                ogs_error("OpenAPI_scp_domain_info_parseFromJSON() failed [scp_ip_end_points]");
                goto end;
            }
            OpenAPI_ip_end_point_t *scp_ip_end_pointsItem = OpenAPI_ip_end_point_parseFromJSON(scp_ip_end_points_local);
            if (!scp_ip_end_pointsItem) {
                ogs_error("No scp_ip_end_pointsItem");
                goto end;
            }
            OpenAPI_list_add(scp_ip_end_pointsList, scp_ip_end_pointsItem);
        }
    }

    scp_prefix = cJSON_GetObjectItemCaseSensitive(scp_domain_infoJSON, "scpPrefix");
    if (scp_prefix) {
    if (!cJSON_IsString(scp_prefix) && !cJSON_IsNull(scp_prefix)) {
        ogs_error("OpenAPI_scp_domain_info_parseFromJSON() failed [scp_prefix]");
        goto end;
    }
    }

    scp_ports = cJSON_GetObjectItemCaseSensitive(scp_domain_infoJSON, "scpPorts");
    if (scp_ports) {
        cJSON *scp_ports_local_map = NULL;
        if (!cJSON_IsObject(scp_ports) && !cJSON_IsNull(scp_ports)) {
            ogs_error("OpenAPI_scp_domain_info_parseFromJSON() failed [scp_ports]");
            goto end;
        }
        if (cJSON_IsObject(scp_ports)) {
            scp_portsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(scp_ports_local_map, scp_ports) {
                cJSON *localMapObject = scp_ports_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsNumber(localMapObject)) {
                    ogs_error("OpenAPI_scp_domain_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                localDouble = (double *)ogs_calloc(1, sizeof(double));
                if (!localDouble) {
                    ogs_error("OpenAPI_scp_domain_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                *localDouble = localMapObject->valuedouble;
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), localDouble);
                OpenAPI_list_add(scp_portsList, localMapKeyPair);
            }
        }
    }

    scp_domain_info_local_var = OpenAPI_scp_domain_info_create (
        scp_fqdn && !cJSON_IsNull(scp_fqdn) ? ogs_strdup(scp_fqdn->valuestring) : NULL,
        scp_ip_end_points ? scp_ip_end_pointsList : NULL,
        scp_prefix && !cJSON_IsNull(scp_prefix) ? ogs_strdup(scp_prefix->valuestring) : NULL,
        scp_ports ? scp_portsList : NULL
    );

    return scp_domain_info_local_var;
end:
    if (scp_ip_end_pointsList) {
        OpenAPI_list_for_each(scp_ip_end_pointsList, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(scp_ip_end_pointsList);
        scp_ip_end_pointsList = NULL;
    }
    if (scp_portsList) {
        OpenAPI_list_for_each(scp_portsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(scp_portsList);
        scp_portsList = NULL;
    }
    return NULL;
}

OpenAPI_scp_domain_info_t *OpenAPI_scp_domain_info_copy(OpenAPI_scp_domain_info_t *dst, OpenAPI_scp_domain_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scp_domain_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scp_domain_info_convertToJSON() failed");
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

    OpenAPI_scp_domain_info_free(dst);
    dst = OpenAPI_scp_domain_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


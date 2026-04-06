
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prose_context.h"

OpenAPI_prose_context_t *OpenAPI_prose_context_create(
    OpenAPI_ue_auth_e direct_discovery,
    OpenAPI_ue_auth_e direct_comm,
    OpenAPI_ue_auth_e l2_relay,
    OpenAPI_ue_auth_e l3_relay,
    OpenAPI_ue_auth_e l2_remote,
    OpenAPI_ue_auth_e l3_remote,
    OpenAPI_ue_auth_e l2_ue_relay,
    OpenAPI_ue_auth_e l3_ue_relay,
    OpenAPI_ue_auth_e l2_end,
    OpenAPI_ue_auth_e l3_end,
    OpenAPI_ue_auth_e multi_path_comm,
    char *nr_ue_pc5_ambr,
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para,
    OpenAPI_ue_auth_e l3_interm_relay,
    OpenAPI_ue_auth_e l3_multihop_remote,
    OpenAPI_ue_auth_e l3_net_multihop_relay,
    OpenAPI_ue_auth_e l3_ue_multihop_relay,
    OpenAPI_ue_auth_e l3_end_multihop_relay,
    OpenAPI_ue_auth_e l2_interm_relay,
    OpenAPI_ue_auth_e l2_multihop_remote,
    OpenAPI_ue_auth_e l2_net_multihop_relay
)
{
    OpenAPI_prose_context_t *prose_context_local_var = ogs_malloc(sizeof(OpenAPI_prose_context_t));
    ogs_assert(prose_context_local_var);

    prose_context_local_var->direct_discovery = direct_discovery;
    prose_context_local_var->direct_comm = direct_comm;
    prose_context_local_var->l2_relay = l2_relay;
    prose_context_local_var->l3_relay = l3_relay;
    prose_context_local_var->l2_remote = l2_remote;
    prose_context_local_var->l3_remote = l3_remote;
    prose_context_local_var->l2_ue_relay = l2_ue_relay;
    prose_context_local_var->l3_ue_relay = l3_ue_relay;
    prose_context_local_var->l2_end = l2_end;
    prose_context_local_var->l3_end = l3_end;
    prose_context_local_var->multi_path_comm = multi_path_comm;
    prose_context_local_var->nr_ue_pc5_ambr = nr_ue_pc5_ambr;
    prose_context_local_var->pc5_qo_s_para = pc5_qo_s_para;
    prose_context_local_var->l3_interm_relay = l3_interm_relay;
    prose_context_local_var->l3_multihop_remote = l3_multihop_remote;
    prose_context_local_var->l3_net_multihop_relay = l3_net_multihop_relay;
    prose_context_local_var->l3_ue_multihop_relay = l3_ue_multihop_relay;
    prose_context_local_var->l3_end_multihop_relay = l3_end_multihop_relay;
    prose_context_local_var->l2_interm_relay = l2_interm_relay;
    prose_context_local_var->l2_multihop_remote = l2_multihop_remote;
    prose_context_local_var->l2_net_multihop_relay = l2_net_multihop_relay;

    return prose_context_local_var;
}

void OpenAPI_prose_context_free(OpenAPI_prose_context_t *prose_context)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == prose_context) {
        return;
    }
    if (prose_context->nr_ue_pc5_ambr) {
        ogs_free(prose_context->nr_ue_pc5_ambr);
        prose_context->nr_ue_pc5_ambr = NULL;
    }
    if (prose_context->pc5_qo_s_para) {
        OpenAPI_pc5_qo_s_para_free(prose_context->pc5_qo_s_para);
        prose_context->pc5_qo_s_para = NULL;
    }
    ogs_free(prose_context);
}

cJSON *OpenAPI_prose_context_convertToJSON(OpenAPI_prose_context_t *prose_context)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (prose_context == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [ProseContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (prose_context->direct_discovery != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "directDiscovery", OpenAPI_ue_auth_ToString(prose_context->direct_discovery)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [direct_discovery]");
        goto end;
    }
    }

    if (prose_context->direct_comm != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "directComm", OpenAPI_ue_auth_ToString(prose_context->direct_comm)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [direct_comm]");
        goto end;
    }
    }

    if (prose_context->l2_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l2Relay", OpenAPI_ue_auth_ToString(prose_context->l2_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l2_relay]");
        goto end;
    }
    }

    if (prose_context->l3_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l3Relay", OpenAPI_ue_auth_ToString(prose_context->l3_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l3_relay]");
        goto end;
    }
    }

    if (prose_context->l2_remote != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l2Remote", OpenAPI_ue_auth_ToString(prose_context->l2_remote)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l2_remote]");
        goto end;
    }
    }

    if (prose_context->l3_remote != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l3Remote", OpenAPI_ue_auth_ToString(prose_context->l3_remote)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l3_remote]");
        goto end;
    }
    }

    if (prose_context->l2_ue_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l2UeRelay", OpenAPI_ue_auth_ToString(prose_context->l2_ue_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l2_ue_relay]");
        goto end;
    }
    }

    if (prose_context->l3_ue_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l3UeRelay", OpenAPI_ue_auth_ToString(prose_context->l3_ue_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l3_ue_relay]");
        goto end;
    }
    }

    if (prose_context->l2_end != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l2End", OpenAPI_ue_auth_ToString(prose_context->l2_end)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l2_end]");
        goto end;
    }
    }

    if (prose_context->l3_end != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l3End", OpenAPI_ue_auth_ToString(prose_context->l3_end)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l3_end]");
        goto end;
    }
    }

    if (prose_context->multi_path_comm != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "multiPathComm", OpenAPI_ue_auth_ToString(prose_context->multi_path_comm)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [multi_path_comm]");
        goto end;
    }
    }

    if (prose_context->nr_ue_pc5_ambr) {
    if (cJSON_AddStringToObject(item, "nrUePc5Ambr", prose_context->nr_ue_pc5_ambr) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [nr_ue_pc5_ambr]");
        goto end;
    }
    }

    if (prose_context->pc5_qo_s_para) {
    cJSON *pc5_qo_s_para_local_JSON = OpenAPI_pc5_qo_s_para_convertToJSON(prose_context->pc5_qo_s_para);
    if (pc5_qo_s_para_local_JSON == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [pc5_qo_s_para]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pc5QoSPara", pc5_qo_s_para_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [pc5_qo_s_para]");
        goto end;
    }
    }

    if (prose_context->l3_interm_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l3IntermRelay", OpenAPI_ue_auth_ToString(prose_context->l3_interm_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l3_interm_relay]");
        goto end;
    }
    }

    if (prose_context->l3_multihop_remote != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l3MultihopRemote", OpenAPI_ue_auth_ToString(prose_context->l3_multihop_remote)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l3_multihop_remote]");
        goto end;
    }
    }

    if (prose_context->l3_net_multihop_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l3NetMultihopRelay", OpenAPI_ue_auth_ToString(prose_context->l3_net_multihop_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l3_net_multihop_relay]");
        goto end;
    }
    }

    if (prose_context->l3_ue_multihop_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l3UeMultihopRelay", OpenAPI_ue_auth_ToString(prose_context->l3_ue_multihop_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l3_ue_multihop_relay]");
        goto end;
    }
    }

    if (prose_context->l3_end_multihop_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l3EndMultihopRelay", OpenAPI_ue_auth_ToString(prose_context->l3_end_multihop_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l3_end_multihop_relay]");
        goto end;
    }
    }

    if (prose_context->l2_interm_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l2IntermRelay", OpenAPI_ue_auth_ToString(prose_context->l2_interm_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l2_interm_relay]");
        goto end;
    }
    }

    if (prose_context->l2_multihop_remote != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l2MultihopRemote", OpenAPI_ue_auth_ToString(prose_context->l2_multihop_remote)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l2_multihop_remote]");
        goto end;
    }
    }

    if (prose_context->l2_net_multihop_relay != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "l2NetMultihopRelay", OpenAPI_ue_auth_ToString(prose_context->l2_net_multihop_relay)) == NULL) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed [l2_net_multihop_relay]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_prose_context_t *OpenAPI_prose_context_parseFromJSON(cJSON *prose_contextJSON)
{
    OpenAPI_prose_context_t *prose_context_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *direct_discovery = NULL;
    OpenAPI_ue_auth_e direct_discoveryVariable = 0;
    cJSON *direct_comm = NULL;
    OpenAPI_ue_auth_e direct_commVariable = 0;
    cJSON *l2_relay = NULL;
    OpenAPI_ue_auth_e l2_relayVariable = 0;
    cJSON *l3_relay = NULL;
    OpenAPI_ue_auth_e l3_relayVariable = 0;
    cJSON *l2_remote = NULL;
    OpenAPI_ue_auth_e l2_remoteVariable = 0;
    cJSON *l3_remote = NULL;
    OpenAPI_ue_auth_e l3_remoteVariable = 0;
    cJSON *l2_ue_relay = NULL;
    OpenAPI_ue_auth_e l2_ue_relayVariable = 0;
    cJSON *l3_ue_relay = NULL;
    OpenAPI_ue_auth_e l3_ue_relayVariable = 0;
    cJSON *l2_end = NULL;
    OpenAPI_ue_auth_e l2_endVariable = 0;
    cJSON *l3_end = NULL;
    OpenAPI_ue_auth_e l3_endVariable = 0;
    cJSON *multi_path_comm = NULL;
    OpenAPI_ue_auth_e multi_path_commVariable = 0;
    cJSON *nr_ue_pc5_ambr = NULL;
    cJSON *pc5_qo_s_para = NULL;
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para_local_nonprim = NULL;
    cJSON *l3_interm_relay = NULL;
    OpenAPI_ue_auth_e l3_interm_relayVariable = 0;
    cJSON *l3_multihop_remote = NULL;
    OpenAPI_ue_auth_e l3_multihop_remoteVariable = 0;
    cJSON *l3_net_multihop_relay = NULL;
    OpenAPI_ue_auth_e l3_net_multihop_relayVariable = 0;
    cJSON *l3_ue_multihop_relay = NULL;
    OpenAPI_ue_auth_e l3_ue_multihop_relayVariable = 0;
    cJSON *l3_end_multihop_relay = NULL;
    OpenAPI_ue_auth_e l3_end_multihop_relayVariable = 0;
    cJSON *l2_interm_relay = NULL;
    OpenAPI_ue_auth_e l2_interm_relayVariable = 0;
    cJSON *l2_multihop_remote = NULL;
    OpenAPI_ue_auth_e l2_multihop_remoteVariable = 0;
    cJSON *l2_net_multihop_relay = NULL;
    OpenAPI_ue_auth_e l2_net_multihop_relayVariable = 0;
    direct_discovery = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "directDiscovery");
    if (direct_discovery) {
    if (!cJSON_IsString(direct_discovery)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [direct_discovery]");
        goto end;
    }
    direct_discoveryVariable = OpenAPI_ue_auth_FromString(direct_discovery->valuestring);
    }

    direct_comm = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "directComm");
    if (direct_comm) {
    if (!cJSON_IsString(direct_comm)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [direct_comm]");
        goto end;
    }
    direct_commVariable = OpenAPI_ue_auth_FromString(direct_comm->valuestring);
    }

    l2_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l2Relay");
    if (l2_relay) {
    if (!cJSON_IsString(l2_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l2_relay]");
        goto end;
    }
    l2_relayVariable = OpenAPI_ue_auth_FromString(l2_relay->valuestring);
    }

    l3_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l3Relay");
    if (l3_relay) {
    if (!cJSON_IsString(l3_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l3_relay]");
        goto end;
    }
    l3_relayVariable = OpenAPI_ue_auth_FromString(l3_relay->valuestring);
    }

    l2_remote = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l2Remote");
    if (l2_remote) {
    if (!cJSON_IsString(l2_remote)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l2_remote]");
        goto end;
    }
    l2_remoteVariable = OpenAPI_ue_auth_FromString(l2_remote->valuestring);
    }

    l3_remote = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l3Remote");
    if (l3_remote) {
    if (!cJSON_IsString(l3_remote)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l3_remote]");
        goto end;
    }
    l3_remoteVariable = OpenAPI_ue_auth_FromString(l3_remote->valuestring);
    }

    l2_ue_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l2UeRelay");
    if (l2_ue_relay) {
    if (!cJSON_IsString(l2_ue_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l2_ue_relay]");
        goto end;
    }
    l2_ue_relayVariable = OpenAPI_ue_auth_FromString(l2_ue_relay->valuestring);
    }

    l3_ue_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l3UeRelay");
    if (l3_ue_relay) {
    if (!cJSON_IsString(l3_ue_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l3_ue_relay]");
        goto end;
    }
    l3_ue_relayVariable = OpenAPI_ue_auth_FromString(l3_ue_relay->valuestring);
    }

    l2_end = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l2End");
    if (l2_end) {
    if (!cJSON_IsString(l2_end)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l2_end]");
        goto end;
    }
    l2_endVariable = OpenAPI_ue_auth_FromString(l2_end->valuestring);
    }

    l3_end = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l3End");
    if (l3_end) {
    if (!cJSON_IsString(l3_end)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l3_end]");
        goto end;
    }
    l3_endVariable = OpenAPI_ue_auth_FromString(l3_end->valuestring);
    }

    multi_path_comm = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "multiPathComm");
    if (multi_path_comm) {
    if (!cJSON_IsString(multi_path_comm)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [multi_path_comm]");
        goto end;
    }
    multi_path_commVariable = OpenAPI_ue_auth_FromString(multi_path_comm->valuestring);
    }

    nr_ue_pc5_ambr = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "nrUePc5Ambr");
    if (nr_ue_pc5_ambr) {
    if (!cJSON_IsString(nr_ue_pc5_ambr) && !cJSON_IsNull(nr_ue_pc5_ambr)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [nr_ue_pc5_ambr]");
        goto end;
    }
    }

    pc5_qo_s_para = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "pc5QoSPara");
    if (pc5_qo_s_para) {
    pc5_qo_s_para_local_nonprim = OpenAPI_pc5_qo_s_para_parseFromJSON(pc5_qo_s_para);
    if (!pc5_qo_s_para_local_nonprim) {
        ogs_error("OpenAPI_pc5_qo_s_para_parseFromJSON failed [pc5_qo_s_para]");
        goto end;
    }
    }

    l3_interm_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l3IntermRelay");
    if (l3_interm_relay) {
    if (!cJSON_IsString(l3_interm_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l3_interm_relay]");
        goto end;
    }
    l3_interm_relayVariable = OpenAPI_ue_auth_FromString(l3_interm_relay->valuestring);
    }

    l3_multihop_remote = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l3MultihopRemote");
    if (l3_multihop_remote) {
    if (!cJSON_IsString(l3_multihop_remote)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l3_multihop_remote]");
        goto end;
    }
    l3_multihop_remoteVariable = OpenAPI_ue_auth_FromString(l3_multihop_remote->valuestring);
    }

    l3_net_multihop_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l3NetMultihopRelay");
    if (l3_net_multihop_relay) {
    if (!cJSON_IsString(l3_net_multihop_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l3_net_multihop_relay]");
        goto end;
    }
    l3_net_multihop_relayVariable = OpenAPI_ue_auth_FromString(l3_net_multihop_relay->valuestring);
    }

    l3_ue_multihop_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l3UeMultihopRelay");
    if (l3_ue_multihop_relay) {
    if (!cJSON_IsString(l3_ue_multihop_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l3_ue_multihop_relay]");
        goto end;
    }
    l3_ue_multihop_relayVariable = OpenAPI_ue_auth_FromString(l3_ue_multihop_relay->valuestring);
    }

    l3_end_multihop_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l3EndMultihopRelay");
    if (l3_end_multihop_relay) {
    if (!cJSON_IsString(l3_end_multihop_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l3_end_multihop_relay]");
        goto end;
    }
    l3_end_multihop_relayVariable = OpenAPI_ue_auth_FromString(l3_end_multihop_relay->valuestring);
    }

    l2_interm_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l2IntermRelay");
    if (l2_interm_relay) {
    if (!cJSON_IsString(l2_interm_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l2_interm_relay]");
        goto end;
    }
    l2_interm_relayVariable = OpenAPI_ue_auth_FromString(l2_interm_relay->valuestring);
    }

    l2_multihop_remote = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l2MultihopRemote");
    if (l2_multihop_remote) {
    if (!cJSON_IsString(l2_multihop_remote)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l2_multihop_remote]");
        goto end;
    }
    l2_multihop_remoteVariable = OpenAPI_ue_auth_FromString(l2_multihop_remote->valuestring);
    }

    l2_net_multihop_relay = cJSON_GetObjectItemCaseSensitive(prose_contextJSON, "l2NetMultihopRelay");
    if (l2_net_multihop_relay) {
    if (!cJSON_IsString(l2_net_multihop_relay)) {
        ogs_error("OpenAPI_prose_context_parseFromJSON() failed [l2_net_multihop_relay]");
        goto end;
    }
    l2_net_multihop_relayVariable = OpenAPI_ue_auth_FromString(l2_net_multihop_relay->valuestring);
    }

    prose_context_local_var = OpenAPI_prose_context_create (
        direct_discovery ? direct_discoveryVariable : 0,
        direct_comm ? direct_commVariable : 0,
        l2_relay ? l2_relayVariable : 0,
        l3_relay ? l3_relayVariable : 0,
        l2_remote ? l2_remoteVariable : 0,
        l3_remote ? l3_remoteVariable : 0,
        l2_ue_relay ? l2_ue_relayVariable : 0,
        l3_ue_relay ? l3_ue_relayVariable : 0,
        l2_end ? l2_endVariable : 0,
        l3_end ? l3_endVariable : 0,
        multi_path_comm ? multi_path_commVariable : 0,
        nr_ue_pc5_ambr && !cJSON_IsNull(nr_ue_pc5_ambr) ? ogs_strdup(nr_ue_pc5_ambr->valuestring) : NULL,
        pc5_qo_s_para ? pc5_qo_s_para_local_nonprim : NULL,
        l3_interm_relay ? l3_interm_relayVariable : 0,
        l3_multihop_remote ? l3_multihop_remoteVariable : 0,
        l3_net_multihop_relay ? l3_net_multihop_relayVariable : 0,
        l3_ue_multihop_relay ? l3_ue_multihop_relayVariable : 0,
        l3_end_multihop_relay ? l3_end_multihop_relayVariable : 0,
        l2_interm_relay ? l2_interm_relayVariable : 0,
        l2_multihop_remote ? l2_multihop_remoteVariable : 0,
        l2_net_multihop_relay ? l2_net_multihop_relayVariable : 0
    );

    return prose_context_local_var;
end:
    if (pc5_qo_s_para_local_nonprim) {
        OpenAPI_pc5_qo_s_para_free(pc5_qo_s_para_local_nonprim);
        pc5_qo_s_para_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_prose_context_t *OpenAPI_prose_context_copy(OpenAPI_prose_context_t *dst, OpenAPI_prose_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_prose_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_prose_context_convertToJSON() failed");
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

    OpenAPI_prose_context_free(dst);
    dst = OpenAPI_prose_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


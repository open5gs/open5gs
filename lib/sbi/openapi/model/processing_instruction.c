
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "processing_instruction.h"

OpenAPI_processing_instruction_t *OpenAPI_processing_instruction_create(
    OpenAPI_dccf_event_t *event_id,
    int proc_interval,
    OpenAPI_list_t *param_proc_instructs
)
{
    OpenAPI_processing_instruction_t *processing_instruction_local_var = ogs_malloc(sizeof(OpenAPI_processing_instruction_t));
    ogs_assert(processing_instruction_local_var);

    processing_instruction_local_var->event_id = event_id;
    processing_instruction_local_var->proc_interval = proc_interval;
    processing_instruction_local_var->param_proc_instructs = param_proc_instructs;

    return processing_instruction_local_var;
}

void OpenAPI_processing_instruction_free(OpenAPI_processing_instruction_t *processing_instruction)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == processing_instruction) {
        return;
    }
    if (processing_instruction->event_id) {
        OpenAPI_dccf_event_free(processing_instruction->event_id);
        processing_instruction->event_id = NULL;
    }
    if (processing_instruction->param_proc_instructs) {
        OpenAPI_list_for_each(processing_instruction->param_proc_instructs, node) {
            OpenAPI_parameter_processing_instruction_free(node->data);
        }
        OpenAPI_list_free(processing_instruction->param_proc_instructs);
        processing_instruction->param_proc_instructs = NULL;
    }
    ogs_free(processing_instruction);
}

cJSON *OpenAPI_processing_instruction_convertToJSON(OpenAPI_processing_instruction_t *processing_instruction)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (processing_instruction == NULL) {
        ogs_error("OpenAPI_processing_instruction_convertToJSON() failed [ProcessingInstruction]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!processing_instruction->event_id) {
        ogs_error("OpenAPI_processing_instruction_convertToJSON() failed [event_id]");
        return NULL;
    }
    cJSON *event_id_local_JSON = OpenAPI_dccf_event_convertToJSON(processing_instruction->event_id);
    if (event_id_local_JSON == NULL) {
        ogs_error("OpenAPI_processing_instruction_convertToJSON() failed [event_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "eventId", event_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_processing_instruction_convertToJSON() failed [event_id]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "procInterval", processing_instruction->proc_interval) == NULL) {
        ogs_error("OpenAPI_processing_instruction_convertToJSON() failed [proc_interval]");
        goto end;
    }

    if (processing_instruction->param_proc_instructs) {
    cJSON *param_proc_instructsList = cJSON_AddArrayToObject(item, "paramProcInstructs");
    if (param_proc_instructsList == NULL) {
        ogs_error("OpenAPI_processing_instruction_convertToJSON() failed [param_proc_instructs]");
        goto end;
    }
    OpenAPI_list_for_each(processing_instruction->param_proc_instructs, node) {
        cJSON *itemLocal = OpenAPI_parameter_processing_instruction_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_processing_instruction_convertToJSON() failed [param_proc_instructs]");
            goto end;
        }
        cJSON_AddItemToArray(param_proc_instructsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_processing_instruction_t *OpenAPI_processing_instruction_parseFromJSON(cJSON *processing_instructionJSON)
{
    OpenAPI_processing_instruction_t *processing_instruction_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_id = NULL;
    OpenAPI_dccf_event_t *event_id_local_nonprim = NULL;
    cJSON *proc_interval = NULL;
    cJSON *param_proc_instructs = NULL;
    OpenAPI_list_t *param_proc_instructsList = NULL;
    event_id = cJSON_GetObjectItemCaseSensitive(processing_instructionJSON, "eventId");
    if (!event_id) {
        ogs_error("OpenAPI_processing_instruction_parseFromJSON() failed [event_id]");
        goto end;
    }
    event_id_local_nonprim = OpenAPI_dccf_event_parseFromJSON(event_id);
    if (!event_id_local_nonprim) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON failed [event_id]");
        goto end;
    }

    proc_interval = cJSON_GetObjectItemCaseSensitive(processing_instructionJSON, "procInterval");
    if (!proc_interval) {
        ogs_error("OpenAPI_processing_instruction_parseFromJSON() failed [proc_interval]");
        goto end;
    }
    if (!cJSON_IsNumber(proc_interval)) {
        ogs_error("OpenAPI_processing_instruction_parseFromJSON() failed [proc_interval]");
        goto end;
    }

    param_proc_instructs = cJSON_GetObjectItemCaseSensitive(processing_instructionJSON, "paramProcInstructs");
    if (param_proc_instructs) {
        cJSON *param_proc_instructs_local = NULL;
        if (!cJSON_IsArray(param_proc_instructs)) {
            ogs_error("OpenAPI_processing_instruction_parseFromJSON() failed [param_proc_instructs]");
            goto end;
        }

        param_proc_instructsList = OpenAPI_list_create();

        cJSON_ArrayForEach(param_proc_instructs_local, param_proc_instructs) {
            if (!cJSON_IsObject(param_proc_instructs_local)) {
                ogs_error("OpenAPI_processing_instruction_parseFromJSON() failed [param_proc_instructs]");
                goto end;
            }
            OpenAPI_parameter_processing_instruction_t *param_proc_instructsItem = OpenAPI_parameter_processing_instruction_parseFromJSON(param_proc_instructs_local);
            if (!param_proc_instructsItem) {
                ogs_error("No param_proc_instructsItem");
                goto end;
            }
            OpenAPI_list_add(param_proc_instructsList, param_proc_instructsItem);
        }
    }

    processing_instruction_local_var = OpenAPI_processing_instruction_create (
        event_id_local_nonprim,
        
        proc_interval->valuedouble,
        param_proc_instructs ? param_proc_instructsList : NULL
    );

    return processing_instruction_local_var;
end:
    if (event_id_local_nonprim) {
        OpenAPI_dccf_event_free(event_id_local_nonprim);
        event_id_local_nonprim = NULL;
    }
    if (param_proc_instructsList) {
        OpenAPI_list_for_each(param_proc_instructsList, node) {
            OpenAPI_parameter_processing_instruction_free(node->data);
        }
        OpenAPI_list_free(param_proc_instructsList);
        param_proc_instructsList = NULL;
    }
    return NULL;
}

OpenAPI_processing_instruction_t *OpenAPI_processing_instruction_copy(OpenAPI_processing_instruction_t *dst, OpenAPI_processing_instruction_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_processing_instruction_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_processing_instruction_convertToJSON() failed");
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

    OpenAPI_processing_instruction_free(dst);
    dst = OpenAPI_processing_instruction_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


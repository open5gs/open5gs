#define TRACE_MODULE _gx_message

#include "core_debug.h"
#include "core_pkbuf.h"

#include "gx_message.h"

void gx_message_free(gx_message_t *gx_message)
{
    int i;

    d_assert(gx_message, return,);

    for (i = 0; i < gx_message->num_of_pcc_rule; i++)
    {
        PCC_RULE_FREE(&gx_message->pcc_rule[i]);
    }
}

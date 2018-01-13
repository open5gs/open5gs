#define TRACE_MODULE _gx_message

#include "core_debug.h"
#include "core_pkbuf.h"

#include "gx_message.h"

void gx_message_free(gx_message_t *gx_message)
{
    int i, j;

    d_assert(gx_message, return, "Null param");

    for (i = 0; i < gx_message->num_of_pcc_rule; i++)
    {
        pcc_rule_t *pcc_rule = &gx_message->pcc_rule[i];

        for (j = 0; j < pcc_rule->num_of_flow; j++)
        {
            flow_t *flow = &pcc_rule->flow[j];

            if (flow->description)
            {
                CORE_FREE(flow->description);
            }
            else
                d_assert(0,, "Null param");
        }
    }
}

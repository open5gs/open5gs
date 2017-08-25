#define TRACE_MODULE _gx_message

#include "core_debug.h"
#include "core_pkbuf.h"

#include "gx_message.h"

void gx_cca_message_free(gx_cca_message_t *cca_message)
{
    int i, j;

    d_assert(cca_message, return, "Null param");

    for (i = 0; i < cca_message->num_of_pcc_rule; i++)
    {
        pcc_rule_t *pcc_rule = &cca_message->pcc_rule[i];

        if (pcc_rule->name)
            core_free(pcc_rule->name);
        else
            d_assert(0,, "Null param");

        for (j = 0; j < pcc_rule->num_of_flow; j++)
        {
            flow_t *flow = &pcc_rule->flow[j];

            if (flow->description)
                core_free(flow->description);
            else
                d_assert(0,, "Null param");
        }
    }
}

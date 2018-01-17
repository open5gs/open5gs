#define TRACE_MODULE _rx_message

#include "core_debug.h"
#include "core_pkbuf.h"

#include "rx_message.h"

void rx_message_free(rx_message_t *rx_message)
{
    int i, j, k;

    d_assert(rx_message, return, "Null param");

    for (i = 0; i < rx_message->num_of_media_component; i++)
    {
        rx_media_component_t *media_component =
            &rx_message->media_component[i];

        for (j = 0; j < media_component->num_of_sub; j++)
        {
            rx_media_sub_component_t *sub = &media_component->sub[j];

            for (k = 0; k < sub->num_of_flow; k++)
            {
                flow_t *flow = &sub->flow[k];

                if (flow->description)
                {
                    CORE_FREE(flow->description);
                }
                else
                    d_assert(0,, "Null param");
            }
        }
    }
}

#define TRACE_MODULE _rx_message

#include "core_debug.h"
#include "core_pkbuf.h"

#include "rx_message.h"

void rx_message_free(rx_message_t *rx_message)
{
    int i, j;

    d_assert(rx_message, return, "Null param");

    for (i = 0; i < rx_message->num_of_media_component; i++)
    {
        rx_media_component_t *media_component =
            &rx_message->media_component[i];

        for (j = 0; j < media_component->num_of_flow; j++)
        {
            flow_t *flow = &media_component->flow[j];

            if (flow->description)
                core_free(flow->description);
            else
                d_assert(0,, "Null param");
        }
    }
}

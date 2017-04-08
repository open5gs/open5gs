#define TRACE_MODULE _esm_handler

#include "core_debug.h"

#include "nas_message.h"

#include "mme_event.h"

#include "nas_security.h"
#include "nas_conv.h"

void esm_handle_pdn_connectivity_request(mme_esm_t *esm, 
        nas_pdn_connectivity_request_t *pdn_connectivity_request)
{
#if 0 /* TODO */
    printf("request type = 0x%x, %d\n", 
            pdn_connectivity_request->request_type.request_type,
            pdn_connectivity_request->request_type.pdn_type);
    printf("precesce = 0x%x\n", pdn_connectivity_request->presencemask);
    printf("flag = 0x%x\n", 
            pdn_connectivity_request->esm_information_transfer_flag.security_protected_required);
#endif
}

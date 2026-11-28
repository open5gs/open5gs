#include "ogs-sbi.h"
#include "aanf-handler.h"
#include "akma-key.h"

int aanf_aanf_handler_init(void)
{
    ogs_info("AANF handler initialized");

    // You could register HTTP handlers here
    // e.g. ogs_sbi_server_add_handler("/naanf-auth/v1/ue-auth", handler_function);

    return OGS_OK;
}

void aanf_aanf_handler_final(void)
{
    ogs_info("AANF handler finalized");
}

#include "ogs-core.h"
#include "context.h"

aanf_context_t *self = NULL;

int aanf_context_init(void)
{
    ogs_assert(!self);

    self = ogs_calloc(1, sizeof(aanf_context_t));
    if (!self) return OGS_ERROR;

    self->nf_type = OGS_SBI_NF_TYPE_AANF;
    self->nf_instance_id = ogs_app()->instance;

    ogs_info("AANF context initialized");
    return OGS_OK;
}

void aanf_context_final(void)
{
    if (!self) return;

    ogs_info("AANF context finalized");

    ogs_free(self);
    self = NULL;
}

aanf_context_t *aanf_self(void)
{
    ogs_assert(self);
    return self;
}

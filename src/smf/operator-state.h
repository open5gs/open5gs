/*
 * Copyright (C) 2026 by Open5GS contributors
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef SMF_OPERATOR_STATE_H
#define SMF_OPERATOR_STATE_H

#include <stdbool.h>
#include <stdint.h>

#include "sbi/openapi/external/cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct smf_sess_s smf_sess_t;

void smf_operator_state_set_path(const char *path);
void smf_operator_state_init(void);
void smf_operator_state_final(void);

void smf_operator_state_record_upf_admin(const char *addr, int draining);
void smf_operator_state_record_migration(smf_sess_t *sess);
void smf_operator_state_record_migration_terminal(
        smf_sess_t *sess, const char *outcome);

bool smf_operator_state_recovery_required(void);
cJSON *smf_operator_state_recovery_to_json(void);

#ifdef __cplusplus
}
#endif

#endif /* SMF_OPERATOR_STATE_H */

/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-app.h"

static ogs_app_context_t self;

static int initialized = 0;

int ogs_app_context_init(void)
{
    ogs_assert(initialized == 0);

    memset(&self, 0, sizeof(ogs_app_context_t));

    initialized = 1;

    return OGS_OK;
}

void ogs_app_context_final(void)
{
    ogs_assert(initialized == 1);

    if (self.document) {
        yaml_document_delete(self.document);
        free(self.document);
    }

    if (self.pollset)
        ogs_pollset_destroy(self.pollset);
    if (self.timer_mgr)
        ogs_timer_mgr_destroy(self.timer_mgr);
    if (self.queue)
        ogs_queue_destroy(self.queue);

    initialized = 0;
}

ogs_app_context_t *ogs_app(void)
{
    return &self;
}

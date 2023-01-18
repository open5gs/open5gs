/*
 * Copyright (C) 2023 by Ryan Dimsey <ryan@omnitouch.com.au>
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

#ifndef MME_S13_HANDLER_H
#define MME_S13_HANDLER_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t mme_s13_handle_eca(
        mme_ue_t *mme_ue, ogs_diam_s13_message_t *s13_message);

/* The following have been exposed for testing purposes and
 * should ever be called in practice */
uint8_t validate_s13_message(ogs_diam_s13_message_t *s13_message);
uint8_t validate_eca(ogs_diam_s13_eca_message_t eca_message, ogs_nas_eir_t eir_config);

#ifdef __cplusplus
}
#endif

#endif /* MME_S13_HANDLER_H */

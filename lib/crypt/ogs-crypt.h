/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef OGS_CRYPT_H
#define OGS_CRYPT_H

#include "ogs-core.h"

#define OGS_CRYPT_INSIDE

#include "crypt/ogs-sha1.h"
#include "crypt/ogs-sha1-hmac.h"
#include "crypt/ogs-sha2.h"
#include "crypt/ogs-sha2-hmac.h"
#include "crypt/ogs-aes.h"
#include "crypt/ogs-aes-cmac.h"

#include "crypt/milenage.h"
#include "crypt/snow-3g.h"
#include "crypt/zuc.h"
#include "crypt/kasumi.h"

#undef OGS_CRYPT_INSIDE

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_RAND_LEN                    16
#define OGS_AUTN_LEN                    16
#define OGS_AUTS_LEN                    14
#define OGS_MAX_RES_LEN                 16

#ifdef __cplusplus
}
#endif

#endif /* OGS_CRYPT_H */

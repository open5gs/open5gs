/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ogs-app.h"
#include "ogs-dbi.h"
#include "ogs-sctp.h"
#include "ogs-gtp.h"
#include "ogs-diameter-common.h"
#include "ogs-nas-eps.h"
#include "ogs-nas-5gs.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"
#include "ogs-ngap.h"
#include "ogs-s1ap.h"
#pragma GCC diagnostic pop

#include "ogs-sbi.h"

#include "core/abts.h"

#define OGS_TEST_INSIDE

#include "common/context.h"
#include "common/sctp.h"
#include "common/gtpu.h"
#include "common/application.h"
#include "common/gmm-build.h"
#include "common/gmm-handler.h"
#include "common/gsm-build.h"
#include "common/gsm-handler.h"
#include "common/nas-path.h"
#include "common/nas-encoder.h"
#include "common/nas-security.h"

#include "common/ngap-build.h"
#include "common/ngap-handler.h"
#include "common/ngap-path.h"

#include "common/s1ap-build.h"
#include "common/s1ap-handler.h"
#include "common/s1ap-path.h"

#include "common/emm-build.h"
#include "common/emm-handler.h"
#include "common/esm-build.h"
#include "common/esm-handler.h"

#include "common/sgsap-build.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN 1

#define SEND_UE_CONTEXT_RELEASE_COMMAND_IN_INTEGRITY_UNPROTECTED 0
#define SEND_UE_CONTEXT_RELEASE_COMMAND_IN_INTEGRITY_PROTECTED 1

#undef OGS_TEST_INSIDE

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_H */

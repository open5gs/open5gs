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

#include "ogs-sbi.h"

static void handle_nf_service(
        ogs_sbi_nf_service_t *nf_service, OpenAPI_nf_service_t *NFService);
static bool handle_smf_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_smf_info_t *SmfInfo);
static bool handle_scp_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_scp_info_t *ScpInfo);
static bool handle_sepp_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_sepp_info_t *SeppInfo);
static bool handle_amf_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_amf_info_t *AmfInfo);

void ogs_nnrf_nfm_handle_nf_register(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_nf_profile_t *NFProfile = NULL;
    OpenAPI_lnode_t *node;

    ogs_assert(recvmsg);
    ogs_assert(nf_instance);

    NFProfile = recvmsg->NFProfile;
    if (!NFProfile) {
        ogs_error("No NFProfile");
        return;
    }

    /* TIME : Update heartbeat from NRF */
    if (NFProfile->is_heart_beat_timer == true)
        nf_instance->time.heartbeat_interval = NFProfile->heart_beat_timer;

    if (NFProfile->plmn_list) {
        ogs_local_conf()->num_of_serving_plmn_id = 0;
        OpenAPI_list_for_each(NFProfile->plmn_list, node) {
            OpenAPI_plmn_id_t *PlmnId = node->data;
            if (PlmnId) {
                if (ogs_local_conf()->num_of_serving_plmn_id >=
                        OGS_ARRAY_SIZE(ogs_local_conf()->serving_plmn_id)) {
                    ogs_error("OVERFLOW NFProfile->plmn_list [%d:%d:%d]",
                            ogs_local_conf()->num_of_serving_plmn_id,
                            OGS_MAX_NUM_OF_PLMN,
                            (int)OGS_ARRAY_SIZE(
                                ogs_local_conf()->serving_plmn_id));
                    break;
                }
                ogs_sbi_parse_plmn_id(
                    &ogs_local_conf()->serving_plmn_id[
                        ogs_local_conf()->num_of_serving_plmn_id], PlmnId);
                ogs_local_conf()->num_of_serving_plmn_id++;
            }
        }
    }
}

/* ====================================================================
 * NFProfile shadow-swap rebuild
 * ====================================================================
 *
 * ogs_nnrf_nfm_handle_nf_profile() rebuilds an nf_instance's profile
 * from a wire NFProfile. To make the rebuild atomic, the work is
 * performed against a stack-allocated "staged" nf_instance and the
 * result is committed onto the live nf_instance only on success;
 * on failure the staged shadow is freed and the live instance is
 * left exactly as it was.
 *
 * Field category taxonomy
 * -----------------------
 * Each member of ogs_sbi_nf_instance_t falls into one of four
 * categories, tagged inline in lib/sbi/context.h:
 *
 * [RUNTIME]
 *   Lifecycle / identity / binding state. NOT part of NFProfile.
 *   Set by nf_instance_add(), FSM init, ID setter, or client
 *   association code paths. The shadow-swap MUST NOT touch these
 *   fields - they live on the live instance throughout. The staged
 *   shadow is memset(0) so RUNTIME slots are zero there; they are
 *   never copied to the live instance during commit.
 *
 * [PROFILE_REQUIRED]
 *   NFProfile mandatory per 3GPP TS 29.510 (nfInstanceId, nfType,
 *   nfStatus). The public wrapper validates these BEFORE staged
 *   init, so they never enter the build path with NULL values.
 *
 * [PROFILE_OPTIONAL_FLAGGED]
 *   NFProfile optional, OpenAPI generates an `is_xxx` flag to
 *   distinguish "explicitly set to zero" from "absent". If the
 *   flag is false on the wire, the field must keep its previous
 *   value - otherwise re-registering with a partial NFProfile
 *   would clobber working values with zero. Examples: priority,
 *   capacity, load, heartBeatTimer.
 *
 * [PROFILE_OPTIONAL_DATA]
 *   NFProfile optional fields whose presence is signalled by data
 *   shape: a non-NULL pointer (e.g. fqdn) or a non-empty list
 *   (e.g. nfServices). The staged shadow starts these at zero /
 *   NULL; the builder fills them when NFProfile carries the data,
 *   leaves them empty otherwise.
 *
 * Maintainer workflow when adding a new ogs_sbi_nf_instance_t field
 * ----------------------------------------------------------------
 *
 *   [RUNTIME]
 *     Touch nothing in this file. The staged shadow leaves the slot
 *     at zero and commit never copies it.
 *
 *   [PROFILE_REQUIRED]
 *     - ogs_nnrf_nfm_handle_nf_profile(): add a non-null/non-zero
 *       check before nf_profile_payload_init().
 *     - nnrf_nfm_build_nf_profile_payload(): write into staged
 *       unconditionally.
 *     - nf_profile_payload_commit(): copy from staged to live.
 *
 *   [PROFILE_OPTIONAL_FLAGGED]
 *     - nf_profile_payload_init(): carry forward base->field so
 *       an omitted is_xxx flag preserves the live value.
 *     - nnrf_nfm_build_nf_profile_payload(): write into staged
 *       only when NFProfile->is_xxx is true.
 *     - nf_profile_payload_commit(): copy from staged to live.
 *
 *   [PROFILE_OPTIONAL_DATA]
 *     - nnrf_nfm_build_nf_profile_payload(): populate from
 *       NFProfile when present.
 *     - nf_profile_payload_commit(): copy (scalars / fixed-size
 *       value arrays) or move ownership (heap pointers, list
 *       nodes with back-pointer rewriting).
 *     - nf_profile_payload_free(): release the resource for owned
 *       heap or list fields. Plain scalars and fixed-size value
 *       arrays need no free hook.
 *
 * Forgetting to update commit() silently drops the new field on
 * every successful rebuild - the build succeeds but the field
 * never reaches the registry. There is no compiler warning for
 * this; reviewers must explicitly check commit() when builder
 * changes.
 * ==================================================================== */

/*
 * Build the new NFProfile into a staged nf_instance.
 *
 * The live nf_instance must NOT be modified until the staged rebuild
 * succeeds; on failure we discard the staged payload, on success
 * ogs_nnrf_nfm_handle_nf_profile()'s commit step transfers the
 * staged profile payload onto the live nf_instance.
 */
static bool nnrf_nfm_build_nf_profile_payload(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_profile_t *NFProfile)
{
    int rv;
    OpenAPI_lnode_t *node;

    ogs_assert(nf_instance);
    ogs_assert(NFProfile);

    /*
     * Note: no nf_instance_clear() here. The staged shadow arrives
     * already memset(0)+list_init() via nf_profile_payload_init(),
     * so there is nothing to clear. The live instance's old payload
     * is freed later by nf_profile_payload_free() inside commit.
     */

    /* [PROFILE_REQUIRED] nf_type, nf_status. */
    nf_instance->nf_type = NFProfile->nf_type;
    nf_instance->nf_status = NFProfile->nf_status;
    /* [PROFILE_OPTIONAL_FLAGGED] heartBeatTimer. */
    if (NFProfile->is_heart_beat_timer == true)
        nf_instance->time.heartbeat_interval = NFProfile->heart_beat_timer;

    /* [PROFILE_OPTIONAL_DATA] fqdn. Owned string. */
    if (NFProfile->fqdn)
        nf_instance->fqdn = ogs_strdup(NFProfile->fqdn);

    /* [PROFILE_OPTIONAL_FLAGGED] priority / capacity / load. */
    if (NFProfile->is_priority == true)
        nf_instance->priority = NFProfile->priority;
    if (NFProfile->is_capacity == true)
        nf_instance->capacity = NFProfile->capacity;
    if (NFProfile->is_load == true)
        nf_instance->load = NFProfile->load;

    /* [PROFILE_OPTIONAL_DATA] plmnList. Fixed-size value array. */
    nf_instance->num_of_plmn_id = 0;
    OpenAPI_list_for_each(NFProfile->plmn_list, node) {
        OpenAPI_plmn_id_t *PlmnId = node->data;
        if (PlmnId) {
            if (nf_instance->num_of_plmn_id >=
                    OGS_ARRAY_SIZE(nf_instance->plmn_id)) {
                ogs_error("OVERFLOW NFProfile->plmn_list [%d:%d:%d]",
                        nf_instance->num_of_plmn_id, OGS_MAX_NUM_OF_PLMN,
                        (int)OGS_ARRAY_SIZE(nf_instance->plmn_id));
                break;
            }
            ogs_sbi_parse_plmn_id(
                &nf_instance->plmn_id[nf_instance->num_of_plmn_id], PlmnId);
            nf_instance->num_of_plmn_id++;
        }
    }

    /* [PROFILE_OPTIONAL_DATA] ipv4Addresses. Owned sockaddr_t* array.
     * Per-slot ownership; discard releases each non-NULL slot. */
    OpenAPI_list_for_each(NFProfile->ipv4_addresses, node) {
        ogs_sockaddr_t *addr = NULL;

        if (!node->data) {
            ogs_error("No IPv4 Address");
            continue;
        }

        if (nf_instance->num_of_ipv4 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {

            rv = ogs_getaddrinfo(
                    &addr, AF_UNSPEC,
                    node->data,
                    ogs_sbi_default_client_port(OpenAPI_uri_scheme_NULL), 0);
            if (rv != OGS_OK) {
                ogs_error("ogs_getaddrinfo[%s] failed", (char *)node->data);
                continue;
            }

            nf_instance->ipv4[nf_instance->num_of_ipv4] = addr;
            nf_instance->num_of_ipv4++;
        }
    }
    /* [PROFILE_OPTIONAL_DATA] ipv6Addresses. Same ownership as ipv4. */
    OpenAPI_list_for_each(NFProfile->ipv6_addresses, node) {
        ogs_sockaddr_t *addr = NULL;

        if (!node->data) {
            ogs_error("No IPv6 Address");
            continue;
        }

        if (nf_instance->num_of_ipv6 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {

            rv = ogs_getaddrinfo(
                    &addr, AF_UNSPEC,
                    node->data,
                    ogs_sbi_default_client_port(OpenAPI_uri_scheme_NULL), 0);
            if (rv != OGS_OK) {
                ogs_error("ogs_getaddrinfo[%s] failed", (char *)node->data);
                continue;
            }

            nf_instance->ipv6[nf_instance->num_of_ipv6] = addr;
            nf_instance->num_of_ipv6++;
        }
    }

    /* [PROFILE_OPTIONAL_DATA] allowedNfTypes. Fixed-size value array. */
    OpenAPI_list_for_each(NFProfile->allowed_nf_types, node) {
        OpenAPI_nf_type_e AllowedNfType = (uintptr_t)node->data;

        if (!AllowedNfType) {
            ogs_error("AllowedNfType");
            continue;
        }

        if (nf_instance->num_of_allowed_nf_type <
                OGS_SBI_MAX_NUM_OF_NF_TYPE) {
            nf_instance->allowed_nf_type[
                nf_instance->num_of_allowed_nf_type] = AllowedNfType;
            nf_instance->num_of_allowed_nf_type++;
        }
    }

    /*
     * TS 33.518 4.2.2.2.1 - Store S-NSSAIs for slice-based discovery
     * authorization.  sNssais identifies the slices this NF serves;
     * allowedNssais restricts for which slices the NF may be discovered.
     */
    nf_instance->num_of_s_nssai = 0;
    /* [PROFILE_OPTIONAL_DATA] sNssais. Fixed-size value array. */
    OpenAPI_list_for_each(NFProfile->s_nssais, node) {
        OpenAPI_ext_snssai_t *sNssai = node->data;
        if (sNssai) {
            if (nf_instance->num_of_s_nssai >= OGS_MAX_NUM_OF_SLICE) {
                ogs_warn("Overflow: NFProfile.sNssais exceeds %d",
                        OGS_MAX_NUM_OF_SLICE);
                break;
            }
            nf_instance->s_nssai[nf_instance->num_of_s_nssai].sst =
                sNssai->sst;
            nf_instance->s_nssai[nf_instance->num_of_s_nssai].sd =
                ogs_s_nssai_sd_from_string(sNssai->sd);
            nf_instance->num_of_s_nssai++;
        }
    }

    nf_instance->num_of_allowed_nssai = 0;
    /* [PROFILE_OPTIONAL_DATA] allowedNssais. Fixed-size value array. */
    OpenAPI_list_for_each(NFProfile->allowed_nssais, node) {
        OpenAPI_ext_snssai_t *sNssai = node->data;
        if (sNssai) {
            if (nf_instance->num_of_allowed_nssai >= OGS_MAX_NUM_OF_SLICE) {
                ogs_warn("Overflow: NFProfile.allowedNssais exceeds %d",
                        OGS_MAX_NUM_OF_SLICE);
                break;
            }
            nf_instance->allowed_nssai[nf_instance->num_of_allowed_nssai].sst =
                sNssai->sst;
            nf_instance->allowed_nssai[nf_instance->num_of_allowed_nssai].sd =
                ogs_s_nssai_sd_from_string(sNssai->sd);
            nf_instance->num_of_allowed_nssai++;
        }
    }

    /* [PROFILE_OPTIONAL_DATA] nfServices. List of pool-allocated
     * nf_service_t objects with back-pointers to the owning instance.
     * Commit re-parents the back-pointer from staged to live. */
    OpenAPI_list_for_each(NFProfile->nf_services, node) {
        ogs_sbi_nf_service_t *nf_service = NULL;
        OpenAPI_nf_service_t *NFService = node->data;

        if (!NFService) {
            ogs_error("No NFService");
            continue;
        }

        if (!NFService->service_instance_id) {
            ogs_error("No NFService.service_instance_id");
            continue;
        }

        if (!NFService->service_name) {
            ogs_error("No NFService.service_name");
            continue;
        }

        if (!NFService->scheme) {
            ogs_error("No NFService.scheme");
            continue;
        }

        nf_service = ogs_sbi_nf_service_find_by_id(
                        nf_instance, NFService->service_instance_id);
        if (!nf_service) {
            nf_service = ogs_sbi_nf_service_add(
                            nf_instance,
                            NFService->service_instance_id,
                            NFService->service_name, NFService->scheme);
            if (!nf_service) {
                ogs_error("Failed to add NFService [%s]",
                        NFService->service_instance_id);
                return false;
            }
        }

        ogs_sbi_nf_service_clear(nf_service);

        handle_nf_service(nf_service, NFService);
    }

    /* [PROFILE_OPTIONAL_DATA] nfServiceList (legacy MAP form,
     * 3GPP TS 29.510 Release 16+). Same ownership semantics as the
     * nf_services list above. */
    OpenAPI_list_for_each(NFProfile->nf_service_list, node) {
        ogs_sbi_nf_service_t *nf_service = NULL;
        OpenAPI_map_t *NFServiceMap = NULL;
        OpenAPI_nf_service_t *NFService = node->data;

        NFServiceMap = node->data;
        if (NFServiceMap) {
            NFService = NFServiceMap->value;
            if (!NFService) {
                ogs_error("No NFService");
                continue;
            }

            if (!NFService->service_instance_id) {
                ogs_error("No NFService.service_instance_id");
                continue;
            }

            if (!NFService->service_name) {
                ogs_error("No NFService.service_name");
                continue;
            }

            if (!NFService->scheme) {
                ogs_error("No NFService.scheme");
                continue;
            }

            nf_service = ogs_sbi_nf_service_find_by_id(
                            nf_instance, NFService->service_instance_id);
            if (!nf_service) {
                nf_service = ogs_sbi_nf_service_add(
                                nf_instance,
                                NFService->service_instance_id,
                                NFService->service_name, NFService->scheme);
                if (!nf_service) {
                    ogs_error("Failed to add NFService [%s]",
                            NFService->service_instance_id);
                    return false;
                }
            }

            ogs_sbi_nf_service_clear(nf_service);

            handle_nf_service(nf_service, NFService);
        }
    }

    /*
     * [PROFILE_OPTIONAL_DATA] nfInfo list (smfInfo / amfInfo /
     * scpInfo / seppInfo / their *_list MAP variants). Heap-allocated
     * nf_info_t nodes linked through nf_info_list. Owned by the
     * containing instance; released by ogs_sbi_nf_info_remove_all().
     *
     * Note: no nf_info_remove_all() reset here. The staged
     * shadow's nf_info_list starts empty from init().
     */

    if (NFProfile->smf_info &&
            handle_smf_info(nf_instance, NFProfile->smf_info) == false)
        return false;

    OpenAPI_list_for_each(NFProfile->smf_info_list, node) {
        OpenAPI_map_t *SmfInfoMap = node->data;
        if (SmfInfoMap && SmfInfoMap->value &&
                handle_smf_info(nf_instance, SmfInfoMap->value) == false)
            return false;
    }
    if (NFProfile->amf_info &&
            handle_amf_info(nf_instance, NFProfile->amf_info) == false)
        return false;

    OpenAPI_list_for_each(NFProfile->amf_info_list, node) {
        OpenAPI_map_t *AmfInfoMap = node->data;
        if (AmfInfoMap && AmfInfoMap->value &&
                handle_amf_info(nf_instance, AmfInfoMap->value) == false)
            return false;
    }
    if (NFProfile->scp_info &&
            handle_scp_info(nf_instance, NFProfile->scp_info) == false)
        return false;
    if (NFProfile->sepp_info &&
            handle_sepp_info(nf_instance, NFProfile->sepp_info) == false)
        return false;

    return true;
}

/*
 * Initialise a staged nf_instance for shadow rebuild.
 *
 * The staged copy starts out as memset(0), then carries forward the
 * [PROFILE_OPTIONAL_FLAGGED] fields from the live instance. See the
 * field category taxonomy in lib/sbi/context.h.
 *
 * Carry-forward is required for FLAGGED fields specifically because
 * the builder only overwrites them when NFProfile->is_xxx is true.
 * Without the carry-forward, a wire profile that omits the is_xxx
 * flag would let the staged value remain zero, and commit would
 * then silently zero out the live priority / capacity / load on
 * every such rebuild.
 *
 * Categories handled here:
 *   [PROFILE_OPTIONAL_FLAGGED] time, priority, capacity, load
 *                              (copied from base).
 *
 * Categories NOT handled here:
 *   [RUNTIME]                  stays on the live instance, never
 *                              touched on staged.
 *   [PROFILE_REQUIRED]         always overwritten by the builder.
 *   [PROFILE_OPTIONAL_DATA]    starts at zero/NULL on staged and is
 *                              populated by the builder if NFProfile
 *                              provides the data.
 *
 * When you add a new [PROFILE_OPTIONAL_FLAGGED] field to
 * ogs_sbi_nf_instance_t, extend the carry-forward below.
 */
static void nf_profile_payload_init(
        ogs_sbi_nf_instance_t *staged, ogs_sbi_nf_instance_t *base)
{
    ogs_assert(staged);
    ogs_assert(base);

    memset(staged, 0, sizeof(*staged));

    /* [PROFILE_OPTIONAL_FLAGGED] carry-forward. */
    staged->time = base->time;
    staged->priority = base->priority;
    staged->capacity = base->capacity;
    staged->load = base->load;

    /* [PROFILE_OPTIONAL_DATA] list heads: explicit init even though
     * memset(0) already zeroed them, for self-documenting code. */
    ogs_list_init(&staged->nf_service_list);
    ogs_list_init(&staged->nf_info_list);
}

/*
 * Free a profile payload from the given nf_instance.
 *
 * Releases all owned heap and list resources that fall under
 * [PROFILE_OPTIONAL_DATA]; leaves [RUNTIME] state alone (FSM,
 * timers, id, lnode linkage). [PROFILE_OPTIONAL_FLAGGED] and
 * [PROFILE_REQUIRED] scalars are zeroed implicitly by
 * ogs_sbi_nf_instance_clear() where applicable, or simply left
 * alone where not - either way they are not "owned" and require
 * no explicit cleanup.
 *
 * Used in two contexts:
 *   - To clean up a staged payload after a failed build, before
 *     the stack-allocated staged goes out of scope;
 *   - As the first step inside nf_profile_payload_commit() to drop
 *     the live nf_instance's old profile before transferring the
 *     staged one onto it.
 *
 * Both nf_info_remove_all() and nf_service_remove_all() identify
 * services / infos via the per-instance list head and (for
 * services) the back-pointer ogs_sbi_nf_service_t::nf_instance, so
 * this works correctly for both the staged shadow (whose services
 * were added with nf_instance == staged) and the live instance
 * (whose services were added with nf_instance == live).
 *
 * When you add a new [PROFILE_OPTIONAL_DATA] field that owns heap
 * or list resources, extend this function to release them.
 */
static void nf_profile_payload_free(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_assert(nf_instance);

    /* [PROFILE_OPTIONAL_DATA] nf_info_list - heap-allocated nodes. */
    ogs_sbi_nf_info_remove_all(&nf_instance->nf_info_list);
    /* [PROFILE_OPTIONAL_DATA] nf_service_list - pool-allocated
     * services with owned id/name strings and back-pointer. */
    ogs_sbi_nf_service_remove_all(nf_instance);
    /* [PROFILE_OPTIONAL_DATA] fqdn (heap), ipv4/ipv6 (sockaddr).
     * ogs_sbi_nf_instance_clear() handles all of these. */
    ogs_sbi_nf_instance_clear(nf_instance);
}

/*
 * Commit the staged profile payload onto the live nf_instance.
 *
 * This is the only place where staged profile state crosses the
 * boundary onto the live instance. Every [PROFILE_REQUIRED],
 * [PROFILE_OPTIONAL_FLAGGED], and [PROFILE_OPTIONAL_DATA] field
 * must be transferred here. [RUNTIME] fields are deliberately
 * left untouched: id / sm / t_* / lnode / client / hnrf_uri stay
 * on the live nf_instance, and the staged copy was memset(0) at
 * init so its zero values for those fields must not leak across.
 *
 * Transfer rules by category:
 *
 *   [PROFILE_REQUIRED]         direct assignment (always present).
 *   [PROFILE_OPTIONAL_FLAGGED] direct assignment (init() guaranteed
 *                              a valid staged value via carry-forward
 *                              or builder overwrite).
 *   [PROFILE_OPTIONAL_DATA] scalar / fixed-size value array:
 *                              copy by assignment / memcpy.
 *   [PROFILE_OPTIONAL_DATA] owned heap pointer (e.g. fqdn):
 *                              pointer-transfer + NULL the staged
 *                              side so a subsequent discard of
 *                              staged does not double-free.
 *   [PROFILE_OPTIONAL_DATA] owned pointer array (e.g. ipv4/ipv6):
 *                              memcpy + zero the staged side, same
 *                              double-free protection.
 *   [PROFILE_OPTIONAL_DATA] list of objects with back-pointer
 *                              (nf_service_list):
 *                              for each node, unlink from staged,
 *                              rewrite back-pointer to nf_instance,
 *                              link onto nf_instance's list head.
 *   [PROFILE_OPTIONAL_DATA] list of plain objects (nf_info_list):
 *                              for each node, unlink from staged,
 *                              link onto nf_instance's list head.
 *
 * Step 1 discards the previously registered profile so the
 * transfers below don't leak the old fqdn / sockaddr / lists.
 */
static void nf_profile_payload_commit(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_nf_instance_t *staged)
{
    ogs_sbi_nf_service_t *nf_service = NULL, *next_nf_service = NULL;
    ogs_sbi_nf_info_t *nf_info = NULL, *next_nf_info = NULL;

    ogs_assert(nf_instance);
    ogs_assert(staged);

    /* Step 1: drop the previously registered profile. */
    nf_profile_payload_free(nf_instance);

    /* [PROFILE_OPTIONAL_FLAGGED] time (struct copy). */
    nf_instance->time = staged->time;
    /* [PROFILE_REQUIRED] nf_type, nf_status. */
    nf_instance->nf_type = staged->nf_type;
    nf_instance->nf_status = staged->nf_status;
    /* [PROFILE_OPTIONAL_FLAGGED] priority / capacity / load. */
    nf_instance->priority = staged->priority;
    nf_instance->capacity = staged->capacity;
    nf_instance->load = staged->load;

    /* [PROFILE_OPTIONAL_DATA] plmn_id (fixed-size value array). */
    nf_instance->num_of_plmn_id = staged->num_of_plmn_id;
    memcpy(nf_instance->plmn_id, staged->plmn_id,
            sizeof(nf_instance->plmn_id));

    /* [PROFILE_OPTIONAL_DATA] allowed_nf_type (fixed-size value array). */
    nf_instance->num_of_allowed_nf_type = staged->num_of_allowed_nf_type;
    memcpy(nf_instance->allowed_nf_type, staged->allowed_nf_type,
            sizeof(nf_instance->allowed_nf_type));

    /* [PROFILE_OPTIONAL_DATA] s_nssai (fixed-size value array). */
    nf_instance->num_of_s_nssai = staged->num_of_s_nssai;
    memcpy(nf_instance->s_nssai, staged->s_nssai,
            sizeof(nf_instance->s_nssai));

    /* [PROFILE_OPTIONAL_DATA] allowed_nssai (fixed-size value array). */
    nf_instance->num_of_allowed_nssai = staged->num_of_allowed_nssai;
    memcpy(nf_instance->allowed_nssai, staged->allowed_nssai,
            sizeof(nf_instance->allowed_nssai));

    /* [PROFILE_OPTIONAL_DATA] fqdn: owned heap pointer transfer. */
    nf_instance->fqdn = staged->fqdn;
    staged->fqdn = NULL;

    /* [PROFILE_OPTIONAL_DATA] ipv4 / ipv6: owned pointer array.
     * memcpy + zero on staged so a subsequent discard is a no-op
     * for these slots. */
    nf_instance->num_of_ipv4 = staged->num_of_ipv4;
    memcpy(nf_instance->ipv4, staged->ipv4, sizeof(nf_instance->ipv4));
    staged->num_of_ipv4 = 0;
    memset(staged->ipv4, 0, sizeof(staged->ipv4));

    nf_instance->num_of_ipv6 = staged->num_of_ipv6;
    memcpy(nf_instance->ipv6, staged->ipv6, sizeof(nf_instance->ipv6));
    staged->num_of_ipv6 = 0;
    memset(staged->ipv6, 0, sizeof(staged->ipv6));

    /* [PROFILE_OPTIONAL_DATA] nf_service_list: re-parent.
     *
     * While in staged, each service had
     *   nf_service->nf_instance == staged
     * which let ogs_sbi_nf_service_clear() and remove_all() identify
     * the staged list correctly during the build. After commit they
     * live on the live instance, so the back-pointer must be
     * rewritten before relinking. */
    ogs_list_for_each_safe(&staged->nf_service_list,
            next_nf_service, nf_service) {
        ogs_list_remove(&staged->nf_service_list, nf_service);
        nf_service->nf_instance = nf_instance;
        ogs_list_add(&nf_instance->nf_service_list, nf_service);
    }

    /* [PROFILE_OPTIONAL_DATA] nf_info_list: relink (no back-pointer). */
    ogs_list_for_each_safe(&staged->nf_info_list, next_nf_info, nf_info) {
        ogs_list_remove(&staged->nf_info_list, nf_info);
        ogs_list_add(&nf_instance->nf_info_list, nf_info);
    }
}

/*
 * Public entry point: atomically rebuild nf_instance's profile from
 * NFProfile.
 *
 * On failure the live nf_instance is guaranteed to be unmodified.
 * The build is performed against a stack-allocated staged shadow,
 * which is either committed (on success) or discarded (on failure).
 */
bool ogs_nnrf_nfm_handle_nf_profile(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_profile_t *NFProfile)
{
    ogs_sbi_nf_instance_t staged;

    ogs_assert(nf_instance);

    /*
     * Top-level NFProfile validation. These checks never touch any
     * state, so we do them before initialising staged to avoid the
     * memset+carry-forward cost on trivially malformed input.
     */
    if (!NFProfile) {
        ogs_error("No NFProfile");
        return false;
    }
    if (!NFProfile->nf_instance_id) {
        ogs_error("No NFProfile.nf_instance_id");
        return false;
    }
    if (!NFProfile->nf_type) {
        ogs_error("No NFProfile.nf_type");
        return false;
    }
    if (!NFProfile->nf_status) {
        ogs_error("No NFProfile.nf_status");
        return false;
    }

    nf_profile_payload_init(&staged, nf_instance);

    if (nnrf_nfm_build_nf_profile_payload(&staged, NFProfile) == false) {
        nf_profile_payload_free(&staged);
        return false;
    }

    nf_profile_payload_commit(nf_instance, &staged);

    return true;
}

static void handle_nf_service(
        ogs_sbi_nf_service_t *nf_service, OpenAPI_nf_service_t *NFService)
{
    int rv;
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(nf_service);
    ogs_assert(NFService);

    OpenAPI_list_for_each(NFService->versions, node) {
        OpenAPI_nf_service_version_t *NFServiceVersion = node->data;

        if (!NFServiceVersion) {
            ogs_error("No NFServiceVersion");
            continue;
        }

        ogs_sbi_nf_service_add_version(nf_service,
                    NFServiceVersion->api_version_in_uri,
                    NFServiceVersion->api_full_version,
                    NFServiceVersion->expiry);
    }

    if (NFService->fqdn)
        nf_service->fqdn = ogs_strdup(NFService->fqdn);

    OpenAPI_list_for_each(NFService->ip_end_points, node) {
        OpenAPI_ip_end_point_t *IpEndPoint = node->data;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
        int port = 0;

        if (!IpEndPoint) {
            ogs_error("No IpEndPoint");
            continue;
        }

        if (nf_service->num_of_addr < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {
            if (!IpEndPoint->is_port)
                port = ogs_sbi_default_client_port(nf_service->scheme);
            else
                port = IpEndPoint->port;

            if (IpEndPoint->ipv4_address) {
                rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                        IpEndPoint->ipv4_address, port, 0);
                if (rv != OGS_OK) {
                    ogs_error("ogs_getaddrinfo[%s] failed",
                                IpEndPoint->ipv4_address);
                    continue;
                }
            }
            if (IpEndPoint->ipv6_address) {
                rv = ogs_getaddrinfo(&addr6, AF_UNSPEC,
                        IpEndPoint->ipv6_address, port, 0);
                if (rv != OGS_OK) {
                    ogs_error("ogs_getaddrinfo[%s] failed",
                                IpEndPoint->ipv6_address);
                    continue;
                }
            }

            if (addr || addr6) {
                nf_service->addr[nf_service->num_of_addr].
                    is_port = IpEndPoint->is_port;
                nf_service->addr[nf_service->num_of_addr].
                    port = port;
                nf_service->addr[nf_service->num_of_addr].
                    ipv4 = addr;
                nf_service->addr[nf_service->num_of_addr].
                    ipv6 = addr6;
                nf_service->num_of_addr++;
            }
        }
    }

    OpenAPI_list_for_each(NFService->allowed_nf_types, node) {
        OpenAPI_nf_type_e AllowedNfType = (uintptr_t)node->data;

        if (!AllowedNfType) {
            ogs_error("AllowedNfType");
            continue;
        }

        if (nf_service->num_of_allowed_nf_type <
                OGS_SBI_MAX_NUM_OF_NF_TYPE) {
            nf_service->allowed_nf_type[
                nf_service->num_of_allowed_nf_type] = AllowedNfType;
            nf_service->num_of_allowed_nf_type++;
        }
    }

    if (NFService->is_priority == true)
        nf_service->priority = NFService->priority;
    if (NFService->is_capacity == true)
        nf_service->capacity = NFService->capacity;
    if (NFService->is_load == true)
        nf_service->load = NFService->load;
}

static bool handle_smf_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_smf_info_t *SmfInfo)
{
    bool rv = true;
    ogs_sbi_nf_info_t *nf_info = NULL;

    OpenAPI_list_t *sNssaiSmfInfoList = NULL;
    OpenAPI_snssai_smf_info_item_t *sNssaiSmfInfoItem = NULL;
    OpenAPI_ext_snssai_t *sNssai = NULL;
    OpenAPI_list_t *DnnSmfInfoList = NULL;
    OpenAPI_dnn_smf_info_item_t *DnnSmfInfoItem = NULL;

    OpenAPI_list_t *TaiList = NULL;
    OpenAPI_tai_t *TaiItem = NULL;
    OpenAPI_list_t *TaiRangeList = NULL;
    OpenAPI_tai_range_t *TaiRangeItem = NULL;
    OpenAPI_list_t *TacRangeList = NULL;
    OpenAPI_tac_range_t *TacRangeItem = NULL;

    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    ogs_assert(nf_instance);
    ogs_assert(SmfInfo);

    nf_info = ogs_sbi_nf_info_add(
            &nf_instance->nf_info_list, OpenAPI_nf_type_SMF);
    if (!nf_info) {
        ogs_error("Failed to add SMF nfInfo");
        return false;
    }

    sNssaiSmfInfoList = SmfInfo->s_nssai_smf_info_list;
    OpenAPI_list_for_each(sNssaiSmfInfoList, node) {
        sNssaiSmfInfoItem = node->data;
        if (sNssaiSmfInfoItem) {
            if (nf_info->smf.num_of_slice >= OGS_MAX_NUM_OF_SLICE) {
                ogs_error("OVERFLOW Slice [%d:%d]",
                        nf_info->smf.num_of_slice, OGS_MAX_NUM_OF_SLICE);
                rv = false;
                break;
            }

            DnnSmfInfoList = sNssaiSmfInfoItem->dnn_smf_info_list;
            OpenAPI_list_for_each(DnnSmfInfoList, node2) {
                DnnSmfInfoItem = node2->data;
                if (DnnSmfInfoItem && DnnSmfInfoItem->dnn) {
                    int dnn_index = nf_info->smf.slice
                        [nf_info->smf.num_of_slice].num_of_dnn;

                    if (dnn_index >= OGS_MAX_NUM_OF_DNN) {
                        ogs_error("OVERFLOW DNN [%d:%d]",
                                dnn_index, OGS_MAX_NUM_OF_DNN);
                        rv = false;
                        break;
                    }
                    nf_info->smf.slice[nf_info->smf.num_of_slice].
                        dnn[dnn_index] = ogs_strdup(DnnSmfInfoItem->dnn);
                    if (!nf_info->smf.slice[nf_info->smf.num_of_slice].
                            dnn[dnn_index]) {
                        ogs_error("No memory for DNN");
                        rv = false;
                        break;
                    }
                    nf_info->smf.slice[nf_info->smf.num_of_slice].
                        num_of_dnn++;
                }
            }

            if (rv == false)
                break;

            if (!nf_info->smf.slice[nf_info->smf.num_of_slice].num_of_dnn) {
                ogs_error("No DNN");
                continue;
            }

            sNssai = sNssaiSmfInfoItem->s_nssai;
            if (sNssai) {
                ogs_s_nssai_t *s_nssai = NULL;

                s_nssai = &nf_info->smf.
                    slice[nf_info->smf.num_of_slice].s_nssai;
                s_nssai->sst = sNssai->sst;
                s_nssai->sd = ogs_s_nssai_sd_from_string(sNssai->sd);
                nf_info->smf.num_of_slice++;
            }
        }
    }

    if (rv == false) goto out;

    if (nf_info->smf.num_of_slice == 0) {
        ogs_error("No S-NSSAI(DNN) in smfInfo");
        ogs_sbi_nf_info_remove(&nf_instance->nf_info_list, nf_info);
        return true;
    }

    TaiList = SmfInfo->tai_list;
    OpenAPI_list_for_each(TaiList, node) {
        TaiItem = node->data;
        if (TaiItem && TaiItem->plmn_id && TaiItem->tac) {
            ogs_5gs_tai_t *nr_tai = NULL;
            if (nf_info->smf.num_of_nr_tai >= OGS_MAX_NUM_OF_TAI) {
                ogs_error("OVERFLOW TAI [%d:%d]",
                        nf_info->smf.num_of_nr_tai, OGS_MAX_NUM_OF_TAI);
                rv = false;
                break;
            }

            nr_tai = &nf_info->smf.nr_tai[nf_info->smf.num_of_nr_tai];
            ogs_assert(nr_tai);
            ogs_sbi_parse_plmn_id(&nr_tai->plmn_id, TaiItem->plmn_id);
            nr_tai->tac = ogs_uint24_from_string_hexadecimal(TaiItem->tac);

            nf_info->smf.num_of_nr_tai++;
        }
    }

    if (rv == false) goto out;

    TaiRangeList = SmfInfo->tai_range_list;
    OpenAPI_list_for_each(TaiRangeList, node) {
        TaiRangeItem = node->data;
        if (TaiRangeItem && TaiRangeItem->plmn_id &&
                TaiRangeItem->tac_range_list) {

            if (nf_info->smf.num_of_nr_tai_range >= OGS_MAX_NUM_OF_TAI) {
                ogs_error("OVERFLOW TaiRangeItem [%d:%d]",
                        nf_info->smf.num_of_nr_tai_range, OGS_MAX_NUM_OF_TAI);
                rv = false;
                break;
            }

            ogs_sbi_parse_plmn_id(
                &nf_info->smf.nr_tai_range
                    [nf_info->smf.num_of_nr_tai_range].plmn_id,
                TaiRangeItem->plmn_id);

            TacRangeList = TaiRangeItem->tac_range_list;
            OpenAPI_list_for_each(TacRangeList, node2) {
                TacRangeItem = node2->data;
                if (TacRangeItem &&
                        TacRangeItem->start && TacRangeItem->end) {
                    int tac_index = nf_info->smf.nr_tai_range
                        [nf_info->smf.num_of_nr_tai_range].num_of_tac_range;
                    if (tac_index >= OGS_MAX_NUM_OF_TAI) {
                        ogs_error("OVERFLOW TAI [%d:%d]",
                                tac_index, OGS_MAX_NUM_OF_TAI);
                        rv = false;
                        break;
                    }

                    nf_info->smf.nr_tai_range
                        [nf_info->smf.num_of_nr_tai_range].
                            start[tac_index] =
                                ogs_uint24_from_string_hexadecimal(
                                        TacRangeItem->start);
                    nf_info->smf.nr_tai_range
                        [nf_info->smf.num_of_nr_tai_range].
                            end[tac_index] =
                                ogs_uint24_from_string_hexadecimal(
                                        TacRangeItem->end);

                    nf_info->smf.nr_tai_range
                        [nf_info->smf.num_of_nr_tai_range].
                            num_of_tac_range++;
                }
            }

            if (rv == false)
                break;

            nf_info->smf.num_of_nr_tai_range++;
        }
    }

out:
    if (rv == false) {
        ogs_sbi_nf_info_remove(&nf_instance->nf_info_list, nf_info);
        return false;
    }

    return true;
}

static void scp_info_free(ogs_sbi_scp_info_t *scp_info)
{
    int i;

    ogs_assert(scp_info);

    for (i = 0; i < scp_info->num_of_domain; i++) {
        ogs_free(scp_info->domain[i].name);
        ogs_free(scp_info->domain[i].fqdn);
    }
    memset(scp_info, 0, sizeof(*scp_info));
}

static bool handle_scp_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_scp_info_t *ScpInfo)
{
    ogs_sbi_nf_info_t *nf_info = NULL;

    OpenAPI_list_t *PortList = NULL;
    OpenAPI_map_t *PortMap = NULL;

    OpenAPI_map_t *DomainInfoMap = NULL;
    OpenAPI_scp_domain_info_t *DomainInfo = NULL;

    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    ogs_sbi_scp_info_t scp_info;

    ogs_assert(nf_instance);
    ogs_assert(ScpInfo);

    memset(&scp_info, 0, sizeof(scp_info));

    PortList = ScpInfo->scp_ports;
    OpenAPI_list_for_each(PortList, node) {
        PortMap = node->data;
        if (PortMap) {
            if (PortMap->key) {
                double *port = PortMap->value;
                if (strcmp(PortMap->key, "http") == 0) {
                    if (port) {
                        scp_info.http.presence = true;
                        scp_info.http.port = *port;
                    } else {
                        ogs_error("No Port Value");
                    }
                } else if (strcmp(PortMap->key, "https") == 0) {
                    if (port) {
                        scp_info.https.presence = true;
                        scp_info.https.port = *port;
                    } else {
                        ogs_error("No Port Value");
                    }
                } else {
                    ogs_error("Unknown Port Key = %s", PortMap->key);
                }
            }
        }
    }

    OpenAPI_list_for_each(ScpInfo->scp_domain_info_list, node) {
        DomainInfoMap = node->data;
        if (DomainInfoMap && DomainInfoMap->key && DomainInfoMap->value) {
            if (scp_info.num_of_domain >= OGS_MAX_NUM_OF_SCP_DOMAIN) {
                ogs_warn("Exceeded maximum SCP domains (%d)",
                        OGS_MAX_NUM_OF_SCP_DOMAIN);
                break;
            }
            scp_info.domain[scp_info.num_of_domain].name =
                ogs_strdup(DomainInfoMap->key);
            DomainInfo = DomainInfoMap->value;
            if (DomainInfo->scp_fqdn)
                scp_info.domain[scp_info.num_of_domain].fqdn =
                    ogs_strdup(DomainInfo->scp_fqdn);
            OpenAPI_list_for_each(DomainInfo->scp_ports, node2) {
                PortMap = node2->data;
                if (PortMap) {
                    if (PortMap->key) {
                        double *port = PortMap->value;
                        if (strcmp(PortMap->key, "http") == 0) {
                            if (port) {
                                scp_info.domain[scp_info.num_of_domain].
                                    http.presence = true;
                                scp_info.domain[scp_info.num_of_domain].
                                    http.port = *port;
                            } else {
                                ogs_error("No Port Value");
                            }
                        } else if (strcmp(PortMap->key, "https") == 0) {
                            if (port) {
                                scp_info.domain[scp_info.num_of_domain].
                                    https.presence = true;
                                scp_info.domain[scp_info.num_of_domain].
                                    https.port = *port;
                            } else {
                                ogs_error("No Port Value");
                            }
                        } else {
                            ogs_error("Unknown Port Key = %s", PortMap->key);
                        }
                    }
                }
            }
            scp_info.num_of_domain++;
        }
    }

    if (scp_info.http.presence || scp_info.https.presence ||
        scp_info.num_of_domain) {
        nf_info = ogs_sbi_nf_info_add(
                &nf_instance->nf_info_list, OpenAPI_nf_type_SCP);
        if (!nf_info) {
            ogs_error("Failed to add SCP nfInfo");
            scp_info_free(&scp_info);
            return false;
        }

        memcpy(&nf_info->scp, &scp_info, sizeof(scp_info));
    }

    return true;
}

static bool handle_sepp_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_sepp_info_t *SeppInfo)
{
    ogs_sbi_nf_info_t *nf_info = NULL;

    OpenAPI_list_t *PortList = NULL;
    OpenAPI_map_t *PortMap = NULL;

    OpenAPI_lnode_t *node = NULL;

    ogs_port_t http, https;

    ogs_assert(nf_instance);
    ogs_assert(SeppInfo);

    http.port = 0;
    https.port = 0;
    http.presence = false;
    https.presence = false;

    PortList = SeppInfo->sepp_ports;
    OpenAPI_list_for_each(PortList, node) {
        PortMap = node->data;
        if (PortMap) {
            if (PortMap->key) {
                double *port = PortMap->value;
                if (strcmp(PortMap->key, "http") == 0) {
                    if (port) {
                        http.presence = true;
                        http.port = *port;
                    } else {
                        ogs_error("No Port Value");
                    }
                } else if (strcmp(PortMap->key, "https") == 0) {
                    if (port) {
                        https.presence = true;
                        https.port = *port;
                    } else {
                        ogs_error("No Port Value");
                    }
                } else {
                    ogs_error("Unknown Port Key = %s", PortMap->key);
                }
            }
        }
    }

    if (http.presence || https.presence) {
        nf_info = ogs_sbi_nf_info_add(
                &nf_instance->nf_info_list, OpenAPI_nf_type_SEPP);
        if (!nf_info) {
            ogs_error("Failed to add SEPP nfInfo");
            return false;
        }

        nf_info->sepp.http.presence = http.presence;
        nf_info->sepp.http.port = http.port;

        nf_info->sepp.https.presence = https.presence;
        nf_info->sepp.https.port = https.port;
    }

    return true;
}

static bool handle_amf_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_amf_info_t *AmfInfo)
{
    bool rv = true;
    ogs_sbi_nf_info_t *nf_info = NULL;
    OpenAPI_list_t *GuamiList = NULL;
    OpenAPI_guami_t *GuamiAmfInfoItem = NULL;
    OpenAPI_list_t *TaiList = NULL;
    OpenAPI_tai_t *TaiItem = NULL;
    OpenAPI_list_t *TaiRangeList = NULL;
    OpenAPI_tai_range_t *TaiRangeItem = NULL;
    OpenAPI_list_t *TacRangeList = NULL;
    OpenAPI_tac_range_t *TacRangeItem = NULL;
    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    ogs_assert(nf_instance);
    ogs_assert(AmfInfo);

    nf_info = ogs_sbi_nf_info_add(
            &nf_instance->nf_info_list, OpenAPI_nf_type_AMF);
    if (!nf_info) {
        ogs_error("Failed to add AMF nfInfo");
        return false;
    }

    nf_info->amf.amf_set_id = ogs_uint64_from_string_hexadecimal(
            AmfInfo->amf_set_id);
    nf_info->amf.amf_region_id = ogs_uint64_from_string_hexadecimal(
            AmfInfo->amf_region_id);

    GuamiList = AmfInfo->guami_list;
    OpenAPI_list_for_each(GuamiList, node) {
        GuamiAmfInfoItem = node->data;
        if (GuamiAmfInfoItem) {
            if (nf_info->amf.num_of_guami >= OGS_MAX_NUM_OF_SERVED_GUAMI) {
                ogs_error("OVERFLOW Guami [%d:%d]",
                        nf_info->amf.num_of_guami, OGS_MAX_NUM_OF_SERVED_GUAMI);
                rv = false;
                break;
            }

            if (GuamiAmfInfoItem->amf_id && GuamiAmfInfoItem->plmn_id &&
                    GuamiAmfInfoItem->plmn_id->mnc &&
                    GuamiAmfInfoItem->plmn_id->mcc) {

                ogs_sbi_parse_guami(
                        &nf_info->amf.guami[nf_info->amf.num_of_guami],
                        GuamiAmfInfoItem);
                nf_info->amf.num_of_guami++;
            }
        }
    }
    if (rv == false) goto out;

    TaiList = AmfInfo->tai_list;
    OpenAPI_list_for_each(TaiList, node) {
        TaiItem = node->data;
        if (TaiItem && TaiItem->plmn_id && TaiItem->tac) {
            ogs_5gs_tai_t *nr_tai = NULL;

            if (nf_info->amf.num_of_nr_tai >= OGS_MAX_NUM_OF_TAI) {
                ogs_error("OVERFLOW TaiItem [%d:%d]",
                        nf_info->amf.num_of_nr_tai, OGS_MAX_NUM_OF_TAI);
                rv = false;
                break;
            }

            nr_tai = &nf_info->amf.nr_tai[nf_info->amf.num_of_nr_tai];

            ogs_sbi_parse_plmn_id(&nr_tai->plmn_id, TaiItem->plmn_id);
            nr_tai->tac = ogs_uint24_from_string_hexadecimal(TaiItem->tac);
            nf_info->amf.num_of_nr_tai++;
        }
    }
    if (rv == false) goto out;

    TaiRangeList = AmfInfo->tai_range_list;
    OpenAPI_list_for_each(TaiRangeList, node) {
        TaiRangeItem = node->data;
        if (TaiRangeItem && TaiRangeItem->plmn_id &&
                TaiRangeItem->tac_range_list) {
            if (nf_info->amf.num_of_nr_tai_range >= OGS_MAX_NUM_OF_TAI) {
                ogs_error("OVERFLOW TaiRangeItem [%d:%d]",
                        nf_info->amf.num_of_nr_tai_range, OGS_MAX_NUM_OF_TAI);
                rv = false;
                break;
            }

            ogs_sbi_parse_plmn_id(
                &nf_info->amf.nr_tai_range
                    [nf_info->amf.num_of_nr_tai_range].plmn_id,
                TaiRangeItem->plmn_id);

            TacRangeList = TaiRangeItem->tac_range_list;
            OpenAPI_list_for_each(TacRangeList, node2) {
                TacRangeItem = node2->data;
                if (TacRangeItem &&
                        TacRangeItem->start && TacRangeItem->end) {
                    int tac_index = nf_info->amf.nr_tai_range
                        [nf_info->amf.num_of_nr_tai_range].num_of_tac_range;
                    if (tac_index >= OGS_MAX_NUM_OF_TAI) {
                        ogs_error("OVERFLOW TAI [%d:%d]",
                                tac_index, OGS_MAX_NUM_OF_TAI);
                        rv = false;
                        break;
                    }

                    nf_info->amf.nr_tai_range
                        [nf_info->amf.num_of_nr_tai_range].start[tac_index] =
                                ogs_uint24_from_string_hexadecimal(
                                        TacRangeItem->start);
                    nf_info->amf.nr_tai_range
                        [nf_info->amf.num_of_nr_tai_range].end[tac_index] =
                                ogs_uint24_from_string_hexadecimal(
                                        TacRangeItem->end);

                    nf_info->amf.nr_tai_range
                        [nf_info->amf.num_of_nr_tai_range].num_of_tac_range++;
                }
            }

            if (rv == false)
                break;

            nf_info->amf.num_of_nr_tai_range++;
        }
    }

out:
    if (rv == false) {
        ogs_sbi_nf_info_remove(&nf_instance->nf_info_list, nf_info);
        return false;
    }

    return true;
}

static void handle_validity_time(
        ogs_sbi_subscription_data_t *subscription_data,
        char *validity_time, const char *action)
{
    ogs_time_t time, validity, patch;
    char *validity_time_string = NULL;

    ogs_assert(subscription_data);
    ogs_assert(action);

    /*
     * If there is a validity_time, then the NRF is updating
     * the validity_time by sending HTTP_STATUS to 200.
     * Therefore, change subscription_data->valdity_duration
     * according to this value.
     *
     * If validity_time is NULL, NRF sent an HTTP_STATUS of 204 (No content).
     * In this case, use the existing subscription_data->validity_duration.
     */
    if (validity_time) {
        if (ogs_sbi_time_from_string(&time, validity_time) == false) {
            ogs_error("[%s] Subscription %s until %s [parser error]",
                    subscription_data->id, action, validity_time);
            return;
        }

        validity = time - ogs_time_now();
        if (validity < 0) {
            ogs_error("[%s] Subscription %s until %s [validity:%d.%06d]",
                    subscription_data->id, action, validity_time,
                    (int)ogs_time_sec(validity), (int)ogs_time_usec(validity));
            return;
        }

        /*
         * Store subscription_data->validity_duration to derive NRF validity.
         * It will be used in ogs_nnrf_nfm_build_status_update().
         *
         * So, you should not remove the following lines.
         */
        subscription_data->validity_duration =
            /* Normalize seconds */
            ogs_time_from_sec(ogs_time_to_sec(validity));
    }

    if (!subscription_data->t_validity) {
        subscription_data->t_validity =
            ogs_timer_add(ogs_app()->timer_mgr,
                ogs_timer_subscription_validity, subscription_data);
        ogs_assert(subscription_data->t_validity);
    }
    ogs_timer_start(subscription_data->t_validity,
            subscription_data->validity_duration);

    /*
     * PATCH request will be sent before VALIDITY is expired.
     */
#define PATCH_TIME_FROM_VALIDITY(x) ((x) / 2)
    patch = PATCH_TIME_FROM_VALIDITY(subscription_data->validity_duration);

    if (!subscription_data->t_patch) {
        subscription_data->t_patch =
            ogs_timer_add(ogs_app()->timer_mgr,
                ogs_timer_subscription_patch, subscription_data);
        ogs_assert(subscription_data->t_patch);
    }
    ogs_timer_start(subscription_data->t_patch, patch);

    if (validity_time) {
        validity_time_string = ogs_strdup(validity_time);
        ogs_assert(validity_time_string);
    } else {
        validity_time_string = ogs_sbi_localtime_string(
                ogs_time_now() + subscription_data->validity_duration);
        ogs_assert(validity_time_string);
    }

    ogs_info("[%s] Subscription %s until %s "
            "[duration:%lld,validity:%d.%06d,patch:%d.%06d]",
            subscription_data->id, action, validity_time_string,
            (long long)subscription_data->validity_duration,
            (int)ogs_time_sec(subscription_data->validity_duration),
            (int)ogs_time_usec(subscription_data->validity_duration),
            (int)ogs_time_sec(patch), (int)ogs_time_usec(patch));

    ogs_free(validity_time_string);
}

void ogs_nnrf_nfm_handle_nf_status_subscribe(
        ogs_sbi_subscription_data_t *subscription_data,
        ogs_sbi_message_t *recvmsg)
{
    OpenAPI_subscription_data_t *SubscriptionData = NULL;

    int rv;
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;

    bool rc;
    ogs_sbi_client_t *client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    ogs_assert(recvmsg);
    ogs_assert(subscription_data);

    SubscriptionData = recvmsg->SubscriptionData;
    if (!SubscriptionData) {
        ogs_error("No SubscriptionData");
        return;
    }

    if (!recvmsg->http.location) {
        ogs_error("No http.location");
        return;
    }

    memset(&header, 0, sizeof(header));
    header.uri = recvmsg->http.location;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        ogs_error("Cannot parse http.location [%s]",
            recvmsg->http.location);
        return;
    }

    if (!message.h.resource.component[1]) {
        ogs_error("No Subscription ID [%s]", recvmsg->http.location);
        ogs_sbi_header_free(&header);
        return;
    }

    rc = ogs_sbi_getaddr_from_uri(
            &scheme, &fqdn, &fqdn_port, &addr, &addr6, header.uri);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("Invalid URI [%s]", header.uri);
        ogs_sbi_header_free(&header);
        return;
    }

    client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
    if (!client) {
        ogs_debug("%s: ogs_sbi_client_add()", OGS_FUNC);
        client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_error("%s: ogs_sbi_client_add() failed", OGS_FUNC);

            ogs_sbi_header_free(&header);
            ogs_free(fqdn);
            ogs_freeaddrinfo(addr);
            ogs_freeaddrinfo(addr6);

            return;
        }
    }
    OGS_SBI_SETUP_CLIENT(subscription_data, client);

    ogs_free(fqdn);
    ogs_freeaddrinfo(addr);
    ogs_freeaddrinfo(addr6);

    ogs_sbi_subscription_data_set_resource_uri(
            subscription_data, header.uri);
    ogs_sbi_subscription_data_set_id(
            subscription_data, message.h.resource.component[1]);

    ogs_sbi_header_free(&header);

    /* SBI Features */
    if (SubscriptionData->nrf_supported_features) {
        subscription_data->nrf_supported_features =
            ogs_uint64_from_string_hexadecimal(
                    SubscriptionData->nrf_supported_features);
    } else {
        subscription_data->nrf_supported_features = 0;
    }

    /* Subscription Validity Time */
    if (SubscriptionData->validity_time)
        handle_validity_time(
                subscription_data, SubscriptionData->validity_time, "created");
}

void ogs_nnrf_nfm_handle_nf_status_update(
        ogs_sbi_subscription_data_t *subscription_data,
        ogs_sbi_message_t *recvmsg)
{
    OpenAPI_subscription_data_t *SubscriptionData = NULL;
    char *validity_time = NULL;
    const char *action = NULL;

    ogs_assert(recvmsg);
    ogs_assert(subscription_data);

    if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
        SubscriptionData = recvmsg->SubscriptionData;
        if (!SubscriptionData) {
            ogs_error("No SubscriptionData");
            return;
        }
        if (!SubscriptionData->validity_time) {
            ogs_error("No validityTime");
            return;
        }

        validity_time = SubscriptionData->validity_time;
        action = "updated(200 OK)";
    } else if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT) {
        /* No valdityTime. Re-use current subscription_data->valdity_duration */
        action = "updated(204 No Content)";
    } else {
        ogs_fatal("[%s] HTTP response error [%d]",
                subscription_data->id ?  subscription_data->id : "Unknown",
                recvmsg->res_status);
        ogs_assert_if_reached();
    }

    /* Update Subscription Validity Time */
    handle_validity_time(subscription_data, validity_time, action);
}

bool ogs_nnrf_nfm_handle_nf_status_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;

    ogs_sbi_response_t *response = NULL;
    OpenAPI_notification_data_t *NotificationData = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    bool nf_instance_created = false;

    ogs_sbi_message_t message;
    ogs_sbi_header_t header;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    NotificationData = recvmsg->NotificationData;
    if (!NotificationData) {
        ogs_error("No NotificationData");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No NotificationData", NULL, NULL));
        return false;
    }

    if (!NotificationData->nf_instance_uri) {
        ogs_error("No nfInstanceUri");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No nfInstanceUri", NULL, NULL));
        return false;
    }

    memset(&header, 0, sizeof(header));
    header.uri = NotificationData->nf_instance_uri;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        ogs_error("Cannot parse nfInstanceUri [%s]", header.uri);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Cannot parse nfInstanceUri", header.uri, NULL));
        return false;
    }

    if (!message.h.resource.component[1]) {
        ogs_error("No nfInstanceId [%s]", header.uri);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Cannot parse nfInstanceUri", header.uri, NULL));
        ogs_sbi_header_free(&header);
        return false;
    }

    if (NF_INSTANCE_ID_IS_SELF(message.h.resource.component[1])) {
        ogs_warn("[%s] The notification is not allowed",
                message.h.resource.component[1]);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "The notification is not allowed",
                message.h.resource.component[1], NULL));
        ogs_sbi_header_free(&header);
        return false;
    }

    if (NotificationData->event ==
            OpenAPI_notification_event_type_NF_REGISTERED) {

        OpenAPI_nf_profile_t *NFProfile = NULL;

        NFProfile = NotificationData->nf_profile;
        if (!NFProfile) {
            ogs_error("No NFProfile");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile", NULL, NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        if (!NFProfile->nf_instance_id) {
            ogs_error("No NFProfile.NFInstanceId");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile.NFInstanceId", NULL, NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        if (!NFProfile->nf_type) {
            ogs_error("No NFProfile.NFType");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile.NFType", NULL, NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        if (!NFProfile->nf_status) {
            ogs_error("No NFProfile.NFStatus");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile.NFStatus", NULL, NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        nf_instance = ogs_sbi_nf_instance_find(message.h.resource.component[1]);
        if (!nf_instance) {
            nf_instance = ogs_sbi_nf_instance_add();
            ogs_assert(nf_instance);

            ogs_sbi_nf_instance_set_id(
                    nf_instance, message.h.resource.component[1]);
            ogs_sbi_nf_fsm_init(nf_instance);
            nf_instance_created = true;

            ogs_info("[%s] (NRF-notify) NF registered", nf_instance->id);
        } else {
            ogs_warn("[%s] (NRF-notify) NF has already been added [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
            if (!OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_registered)) {
                ogs_error("[%s] (NRF-notify) NF invalid state [type:%s]",
                        nf_instance->id,
                        OpenAPI_nf_type_ToString(nf_instance->nf_type));
            }
        }

        if (ogs_nnrf_nfm_handle_nf_profile(nf_instance, NFProfile) == false) {
            ogs_error("[%s] (NRF-notify) Invalid NFProfile [type:%s]",
                    NFProfile->nf_instance_id,
                    OpenAPI_nf_type_ToString(NFProfile->nf_type));

            /*
             * ogs_nnrf_nfm_handle_nf_profile() rebuilds the NF profile in
             * place. If parsing fails for a newly created cache entry, remove
             * it because it may contain a partial profile. If the entry
             * already existed, keep it in the registry to avoid deleting a
             * previously usable local cache entry because of one bad notify.
             */
            if (nf_instance_created == true) {
                ogs_sbi_nf_fsm_fini(nf_instance);
                ogs_sbi_nf_instance_remove(nf_instance);
            }

            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "Invalid NFProfile",
                    NFProfile->nf_instance_id, NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        ogs_info("[%s] (NRF-notify) NF Profile updated [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));

        ogs_sbi_client_associate(nf_instance);

        switch (nf_instance->nf_type) {
        case OpenAPI_nf_type_SEPP:
            ogs_sbi_self()->sepp_instance = nf_instance;
            break;
        default:
            break;
        }

    } else if (NotificationData->event ==
            OpenAPI_notification_event_type_NF_DEREGISTERED) {
        nf_instance = ogs_sbi_nf_instance_find(message.h.resource.component[1]);
        if (nf_instance) {
            ogs_info("[%s] (NRF-notify) NF_DEREGISTERED event [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
            ogs_sbi_nf_fsm_fini(nf_instance);
            ogs_sbi_nf_instance_remove(nf_instance);
        } else {
            ogs_warn("[%s] (NRF-notify) Not found",
                    message.h.resource.component[1]);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_NOT_FOUND,
                    recvmsg, "Not found", message.h.resource.component[1],
                    NULL));
            ogs_sbi_header_free(&header);
            return false;
        }
    } else {
        char *eventstr = OpenAPI_notification_event_type_ToString(
                            NotificationData->event);
        ogs_error("Not supported event [%d:%s]",
                NotificationData->event, eventstr ? eventstr : "Unknown");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Not supported event",
                eventstr ? eventstr : "Unknown", NULL));
        ogs_sbi_header_free(&header);
        return false;
    }

    response = ogs_sbi_build_response(recvmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_sbi_header_free(&header);
    return true;
}

void ogs_nnrf_disc_handle_nf_discover_search_result(
        OpenAPI_search_result_t *SearchResult)
{
    OpenAPI_lnode_t *node = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(SearchResult);

    OpenAPI_list_for_each(SearchResult->nf_instances, node) {
        OpenAPI_nf_profile_t *NFProfile = NULL;
        bool nf_instance_created = false;

        if (!node->data) continue;

        NFProfile = node->data;

        if (!NFProfile) {
            ogs_error("No NFProfile");
            continue;
        }

        if (!NFProfile->nf_instance_id) {
            ogs_error("No NFProfile.NFInstanceId");
            continue;
        }

        if (!NFProfile->nf_type) {
            ogs_error("No NFProfile.NFType");
            continue;
        }

        if (!NFProfile->nf_status) {
            ogs_error("No NFProfile.NFStatus");
            continue;
        }

        nf_instance = ogs_sbi_nf_instance_find(NFProfile->nf_instance_id);
        if (!nf_instance) {
            nf_instance = ogs_sbi_nf_instance_add();
            ogs_assert(nf_instance);

            ogs_sbi_nf_instance_set_id(nf_instance, NFProfile->nf_instance_id);
            ogs_sbi_nf_fsm_init(nf_instance);
            nf_instance_created = true;

            ogs_info("[%s] (NRF-discover) NF registered [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
        } else {
            ogs_warn("[%s] (NRF-discover) NF has already been added [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
            if (!OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_registered)) {
                ogs_error("[%s] (NRF-notify) NF invalid state [type:%s]",
                        nf_instance->id,
                        OpenAPI_nf_type_ToString(nf_instance->nf_type));
            }
        }

        if (NF_INSTANCE_ID_IS_OTHERS(nf_instance->id)) {
            if (ogs_nnrf_nfm_handle_nf_profile(
                        nf_instance, NFProfile) == false) {
                ogs_error("[%s] (NRF-discover) Invalid NFProfile [type:%s]",
                        NFProfile->nf_instance_id,
                        OpenAPI_nf_type_ToString(NFProfile->nf_type));

                /*
                 * Do not leave a newly created partial NFProfile in the
                 * registry. If the entry already existed, keep it to avoid
                 * deleting a previously usable local cache entry because of
                 * one bad discovery result.
                 */
                if (nf_instance_created == true) {
                    ogs_sbi_nf_fsm_fini(nf_instance);
                    ogs_sbi_nf_instance_remove(nf_instance);
                }
                continue;
            }

            ogs_sbi_client_associate(nf_instance);

            switch (nf_instance->nf_type) {
            case OpenAPI_nf_type_SEPP:
                ogs_sbi_self()->sepp_instance = nf_instance;
                break;
            default:
                break;
            }

            /* TIME : Update validity from NRF */
            if (SearchResult->is_validity_period &&
                SearchResult->validity_period) {
                nf_instance->time.validity_duration =
                        SearchResult->validity_period;

                ogs_assert(nf_instance->t_validity);
                ogs_timer_start(nf_instance->t_validity,
                    ogs_time_from_sec(nf_instance->time.validity_duration));

            } else
                ogs_warn("[%s] NF Instance validity-time should not 0 "
                        "[type:%s]",
                    nf_instance->id,
                    nf_instance->nf_type ?
                        OpenAPI_nf_type_ToString(nf_instance->nf_type) :
                        "NULL");

            ogs_info("[%s] (NF-discover) NF Profile updated "
                    "[type:%s validity:%ds]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type),
                    nf_instance->time.validity_duration);
        }
    }
}

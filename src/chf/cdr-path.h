/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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

/*
 * CHF CDR (Charging Data Record) writer.
 *
 * A session closing - on Release, on quota exhaustion, or because the
 * operator aborted it - becomes one TS 32.298 ChargingRecord.  The record
 * is built once (cdr-record.c) and written in whichever encodings the
 * configuration asks for, so the files never disagree:
 *
 *   BER    the encoding TS 32.298 clause 6.1 requires on the Bx
 *          interface ("Data Record Format" 1), produced by the asn1c
 *          codec in lib/asn1c/cdr.
 *   JSON   the same record with the same 3GPP field names, for anything
 *          that would rather read text than decode BER (cdr-json.c).
 *
 * Filed either one file per session or many records to a file:
 *
 *   individual   {path}/YYYY/MM/DD/cdr-{chargingDataRef}.{json,ber}
 *   aggregated   {path}/YYYY/MM/DD/cdr-{YYYYMMDDhhmmss}-{seq}.{json,ber}
 *
 * All of it from chf.yaml:
 *
 *   charging:
 *     cdr:
 *       enable: true
 *       path: /var/log/open5gs/cdr
 *       format: both              # json | asn1 | both | none
 *       aggregation:              # the default for both encodings
 *         enable: false           # false: one file per session
 *         max_records: 1000       # roll at this many records (0 = no cap)
 *         max_size_mb: 16         # roll at this size (0 = no cap)
 *       asn1:                     # per-encoding override
 *         aggregation:
 *           enable: true
 */

#ifndef CHF_CDR_PATH_H
#define CHF_CDR_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * chf_cdr_init / chf_cdr_final
 *
 * Called from chf_context_init / chf_context_final after the config is parsed.
 * Sets the CDR base directory from chf_self()->cdr_path.
 */
void chf_cdr_init(void);
void chf_cdr_final(void);

/*
 * chf_cdr_write — write the CDR for a closing session.
 *
 * @param sess   The session being closed, with its final usage already
 *               accumulated.
 * @param cause  TS 32.298 causeForRecClosing:
 *               "normalRelease"          — SMF sent Release
 *               "volumeLimit"            — quota exhausted (FinalUnitAction)
 *               "managementIntervention" — the operator closed it, via a
 *                                          ChargingNotify ABORT_CHARGING
 *
 * Returns the path of the file the record went into, for the CDR index
 * in MongoDB; the caller frees it.  With both encodings enabled that is
 * the JSON file, since it is the one a person opens - the log line for
 * the write names both.  Returns NULL when nothing was written, which
 * covers CDRs being disabled, no encoding being selected, and failure.
 *
 * TS 32.298 §5.1.5 — CHF CDR content
 * TS 32.298 §5.2.5 — its ASN.1 syntax
 */
/*
 * Whether a closing session is worth a CDR.
 *
 * A session that was granted nothing and used nothing carries no billable
 * information: it is a session the network refused because the subscriber
 * had no allowance left.  A UE that re-establishes on its own can produce
 * a stream of those, and a CDR file each would bury the real records.
 *
 * A session with a real grant still gets its CDR even at zero usage - that
 * one says the subscriber attached and used nothing.
 */
static ogs_inline bool chf_cdr_is_billable(const chf_sess_t *sess)
{
    return sess->ul_octets || sess->dl_octets || sess->granted_quota_total;
}

char *chf_cdr_write(chf_sess_t *sess, const char *cause);

#ifdef __cplusplus
}
#endif

#endif /* CHF_CDR_PATH_H */

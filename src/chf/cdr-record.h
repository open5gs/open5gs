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
 * The TS 32.298 CHF CDR, as a record.
 *
 * One closing session becomes one CHFRecord, and that record is the only
 * CDR model the CHF has.  Both encodings serialise it: BER through asn1c
 * (chf_cdr_record_encode_ber) and JSON through cdr-json.c.  Writing the
 * two from one structure is what keeps them mirror images - a field added
 * here appears in both files, with the same name, or in neither.
 *
 * The record is the 5G SA subset of TS 32.298 clause 5.2.5: a
 * ChargingRecord carrying pDUSessionChargingInformation, with the
 * uplink/downlink volumes in listOfMultipleUnitUsage.  Which fields are
 * present and why is documented field by field in cdr-record.c.
 *
 * The types come from lib/asn1c/cdr, generated from the published 3GPP
 * module; see lib/asn1c/support/cdr-r19.6.0/.
 */

#ifndef CHF_CDR_RECORD_H
#define CHF_CDR_RECORD_H

/*
 * context.h first: it brings in the bson headers, and asn_system.h defines a
 * statement-expression MIN() that mongo-c-driver 2.x would otherwise pick up
 * for a file-scope constant in bson-vector.h.
 */
#include "context.h"

#include "ogs-asn1c-cdr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * chf_cdr_record_build()
 *
 * Build the record for a session that is closing.
 *
 * @param sess      the session, with its final usage already accumulated
 * @param cause     TS 32.298 causeForRecordClosing as a string:
 *                  "normalRelease", "volumeLimit" or
 *                  "managementIntervention".  NULL means normalRelease.
 * @param local_seq localRecordSequenceNumber for this record.
 *
 * Returns a record the caller frees with chf_cdr_record_free(), or NULL
 * if the session carries too little to make a valid record.
 */
CHFCDR_CHFRecord_t *chf_cdr_record_build(const chf_sess_t *sess,
        const char *cause, uint32_t local_seq);

void chf_cdr_record_free(CHFCDR_CHFRecord_t *rec);

/*
 * chf_cdr_record_encode_ber()
 *
 * DER-encode the record.  DER is a canonical subset of the BER that
 * TS 32.298 clause 6.1 requires on Bx ("Data Record Format" 1), so any
 * BER decoder reads what this writes.
 *
 * On success returns the length and stores an ogs_malloc()ed buffer in
 * *out, which the caller frees.  Returns a negative value on failure and
 * leaves *out NULL.
 */
int chf_cdr_record_encode_ber(const CHFCDR_CHFRecord_t *rec, uint8_t **out);

/*
 * The TS 32.298 CauseForRecClosing value behind a cause string, so the
 * two encodings agree on it.  Unknown strings map to normalRelease (0).
 */
long chf_cdr_cause_value(const char *cause);

/*
 * The TS 32.298 name for a CauseForRecClosing value, for the JSON, or
 * NULL when the value has no name in the module - in which case the
 * number itself is written rather than a guess.
 */
const char *chf_cdr_cause_name(long value);

#ifdef __cplusplus
}
#endif

#endif /* CHF_CDR_RECORD_H */

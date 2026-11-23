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
 * The TS 32.298 CDR, rendered as JSON.
 *
 * The same record the BER encoder writes, with the field names the ASN.1
 * module gives them and the same nesting, so the two files describe one
 * record in two syntaxes.  Walking the generated structure rather than
 * the session is what guarantees that: there is no second notion of what
 * a CDR contains.
 *
 * Where JSON cannot carry the ASN.1 type directly, the rendering is
 * stated rather than invented:
 *
 *   TimeStamp            the 9-octet BCD becomes ISO-8601 UTC, e.g.
 *                        "2026-10-01T12:30:00Z".  Nothing is lost - the
 *                        CHF writes UTC with a +0000 offset.
 *   SliceDifferentiator  3 octets become 6 lower-case hex digits, which
 *                        is how TS 23.003 writes an SD.
 *   named INTEGER and ENUMERATED
 *                        rendered as the name the ASN.1 gives the value
 *                        ("volumeLimit", "sMF"), not the number, since
 *                        the name is the part a reader needs.  A value
 *                        with no name falls back to the number.
 *   other INTEGER        a JSON number.  Volumes can exceed 2^53 only at
 *                        petabyte scale, past which a reader should use
 *                        the BER.
 *   IA5String, UTF8String, OCTET STRING carrying text
 *                        a JSON string.
 */

#ifndef CHF_CDR_JSON_H
#define CHF_CDR_JSON_H

#include "cdr-record.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * chf_cdr_json_render()
 *
 * Render the record as a compact single-line JSON object.  Returns an
 * ogs_malloc()ed NUL-terminated string the caller frees, or NULL on
 * failure.
 *
 * One line, because that is what lets an aggregated file be appended to
 * and read back a record at a time (JSON Lines).  An individual file
 * holds the same single line.
 */
char *chf_cdr_json_render(const CHFCDR_CHFRecord_t *rec);

#ifdef __cplusplus
}
#endif

#endif /* CHF_CDR_JSON_H */

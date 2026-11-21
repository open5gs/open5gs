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
 * ChargingNotify — the CHF reaching a live session (TS 32.291 §6.3.4).
 *
 * The CHF only ever answers the SMF, so anything that changes a
 * subscriber's quota from the side - a top-up through the TMF API, a bar,
 * an edited plan - waits for that session's next report.  With grants
 * capped at 50 MiB and five minutes that is up to five minutes, and for an
 * idle session it is the full five.
 *
 * ChargingNotify closes that gap.  The SMF hands the CHF a notifyUri in
 * every ChargingDataRequest; the CHF POSTs a ChargingNotifyRequest there
 * and the SMF answers by reporting at once, which is what fetches the new
 * grant.  Two kinds, per TS 32.291:
 *
 *   REAUTHORIZATION — the quota changed; report and take a fresh grant.
 *   ABORT_CHARGING  — this session may not continue; release it.
 *
 * What starts it is a row in chf.notifications, written by the billing
 * side and polled here.  See db-handler.h for why a queue.
 */

#ifndef CHF_NOTIFY_PATH_H
#define CHF_NOTIFY_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Start and stop the poll timer.  Called from chf_initialize() /
 * chf_terminate(); both are no-ops when charging.notify.enable is off. */
void chf_notify_init(void);
void chf_notify_final(void);

/*
 * Read whatever the billing side has queued since this CHF last looked,
 * deliver it to the sessions this CHF holds, and clear rows old enough to
 * go.  Called on the poll timer; exposed so a test or an operator trigger
 * can drive one pass.  Returns the number of ChargingNotify requests sent.
 */
int chf_notify_poll(void);

/*
 * How long a queue row is kept after the first CHF read it: five minutes,
 * or three poll intervals if that is longer.  Every CHF sharing the
 * database has to poll within that time or it misses the row, so keep
 * poll_seconds alike on all of them; as shipped, each polls 60 times in it.
 */
#define CHF_NOTIFY_KEEP_MIN_SECONDS 300
uint32_t chf_notify_keep_seconds(uint32_t poll_seconds);

/*
 * Send one ChargingNotifyRequest for a session.  No-op, returning false,
 * when the session has no notifyUri - an SMF that sent none can only be
 * reached by answering its reports.
 */
bool chf_notify_send(chf_sess_t *sess, bool abort_charging,
        const char *reason);

#ifdef __cplusplus
}
#endif

#endif /* CHF_NOTIFY_PATH_H */

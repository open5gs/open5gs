/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Thomas Klausner <tk@giga.or.at>                                                                *
*                                                                                                        *
* Copyright (c) 2014 Thomas Klausner                                                                     *
* All rights reserved.                                                                                   *
*                                                                                                        *
* Written under contract by nfotex IT GmbH, http://nfotex.com/                                           *
*                                                                                                        *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:                                              *
*                                                                                                        *
* * Redistributions of source code must retain the above                                                 *
*   copyright notice, this list of conditions and the                                                    *
*   following disclaimer.                                                                                *
*                                                                                                        *
* * Redistributions in binary form must reproduce the above                                              *
*   copyright notice, this list of conditions and the                                                    *
*   following disclaimer in the documentation and/or other                                               *
*   materials provided with the distribution.                                                            *
*                                                                                                        *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT     *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                             *
*********************************************************************************************************/

#include <freeDiameter/extension.h>

/*
 * Load balancing extension. If there are multiple highest-rated hosts with the same score,
 * randomly increase the score of one of them.
 */

#include <stdlib.h>

static unsigned int seed;

static int rt_randomizing(void * cbdata, struct msg ** pmsg, struct fd_list * candidates)
{
	struct fd_list *lic;
	struct msg * msg = *pmsg;
	int max_score = -1;
	int max_score_count = 0;
	
	TRACE_ENTRY("%p %p %p", cbdata, msg, candidates);
	
	CHECK_PARAMS(msg && candidates);
	
	/* Check if it is worth processing the message */
	if (FD_IS_LIST_EMPTY(candidates))
		return 0;

	/* find out maximal score and how many candidates have it */
	for (lic = candidates->next; lic != candidates; lic = lic->next) {
		struct rtd_candidate * cand = (struct rtd_candidate *) lic;
		if (max_score < cand->score) {
			max_score = cand->score;
			max_score_count = 1;
		}
		else if (cand->score == max_score) {
			max_score_count++;
		}
	}

	/* if there is more than one with positive score, randomly increase one of their scores by one */
	if (max_score >= 0 && max_score_count > 1) {
		int lucky_candidate = rand_r(&seed) % max_score_count;
		int i = 0;

		for (lic = candidates->next; lic != candidates; lic = lic->next) {
			struct rtd_candidate * cand = (struct rtd_candidate *) lic;
			if (cand->score == max_score) {
				if (i == lucky_candidate) {
					cand->score++;
					break;
				}
				i++;
			}
		}
	}

	return 0;
}

/* handler */
static struct fd_rt_out_hdl * rt_randomizing_hdl = NULL;

/* entry point */
static int rt_randomize_entry(char * conffile)
{
	/* Register the callback */
	CHECK_FCT(fd_rt_out_register(rt_randomizing, NULL, 4, &rt_randomizing_hdl));
	seed = (unsigned int)time(NULL);
	TRACE_DEBUG(INFO, "Extension 'Randomizing' initialized");
	return 0;
}

/* Unload */
void fd_ext_fini(void)
{
	/* Unregister the callbacks */
	CHECK_FCT_DO(fd_rt_out_unregister(rt_randomizing_hdl, NULL), /* continue */);
	return ;
}

EXTENSION_ENTRY("rt_randomize", rt_randomize_entry);

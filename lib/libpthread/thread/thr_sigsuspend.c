/*
 * Copyright (c) 1995 John Birrell <jb@cimlogic.com.au>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by John Birrell.
 * 4. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY JOHN BIRRELL AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "thr_private.h"

__weak_reference(__sigsuspend, sigsuspend);

int
_sigsuspend(const sigset_t *set)
{
	struct pthread	*curthread = _get_curthread();
	int             ret = -1;

	/* Check if a new signal set was provided by the caller: */
	if (set != NULL) {
		THR_SCHED_LOCK(curthread, curthread);

		/* Change the caller's mask: */
		memcpy(&curthread->tmbx.tm_context.uc_sigmask,
		    set, sizeof(sigset_t));

		THR_SET_STATE(curthread, PS_SIGSUSPEND);

		THR_SCHED_UNLOCK(curthread, curthread);

		/* Wait for a signal: */
		_thr_sched_switch(curthread);

		/* Always return an interrupted error: */
		errno = EINTR;

		/* Restore the signal mask: */
		memcpy(&curthread->tmbx.tm_context.uc_sigmask,
		    &curthread->sigmask, sizeof(sigset_t));
	} else {
		/* Return an invalid argument error: */
		errno = EINVAL;
	}

	/* Return the completion status: */
	return (ret);
}

int
__sigsuspend(const sigset_t * set)
{
	struct pthread *curthread = _get_curthread();
	int		ret;

	_thr_enter_cancellation_point(curthread);
	ret = _sigsuspend(set);
	_thr_leave_cancellation_point(curthread);

	return (ret);
}

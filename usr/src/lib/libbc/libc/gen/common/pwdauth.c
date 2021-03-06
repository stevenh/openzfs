/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 1992 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include <stdio.h>
#include <pwd.h>
#include <shadow.h>
#include <errno.h>

/*
 * Version to go in the BCP compatibility library in SVr4 version of
 * SunOS. This does not bother talking to rpc.pwdauthd or looking for the
 * password.adjunct file on the system since they do not exist anymore.
 * They have been effectively replaced by a more robust aging security provided
 * by the combination of /etc/shadow file, shadow support in the NIS+
 * passwd table and the use of secure RPC in NIS+.
 */

int
pwdauth(char *name, char *password)
{
	/*
	 * this routine authenticates a password for the named user.
	 * Assumes the adjunct file does not exist.
	 * and therefore checks the passwd "source" using the standard
	 * getpwnam(3C) routine that uses /etc/nsswitch.conf(4).
	 */

	struct passwd	*pwp = NULL;
	struct spwd	*spwp = NULL;
	char *enpwp;

	if (spwp = getspnam(name))
		enpwp = spwp->sp_pwdp;
	else if (pwp = getpwnam(name))
		enpwp = pwp->pw_passwd;
	else
		/* user is not in main password system */
		return (-1);
	if (enpwp[0] == '#' && enpwp[1] == '#') {
		/* this means that /etc/passwd has problems */
		fprintf(stderr, "pwdauth: bad passwd entry for %s\n",
		    name);
		return (-1);
	}
	if (strcmp(crypt(password, enpwp), enpwp) == 0)
		return (0);
	else
		return (-1);
}

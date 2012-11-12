/*
 * Copyright (c) 2012 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <config.h>

#include <sys/types.h>
#include <stdio.h>
#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif /* STDC_HEADERS */
#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */
#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif /* HAVE_STRINGS_H */

#include "sudoers.h"

#ifdef HAVE_SETLOCALE

static int current_locale = SUDOERS_LOCALE_USER;

int
sudoers_getlocale(void)
{
    return current_locale;
}

int
sudoers_setlocale(int newlocale, int *prevlocale)
{
    char *res = NULL;

    switch (newlocale) {
	case SUDOERS_LOCALE_USER:
	    if (prevlocale)
		*prevlocale = current_locale;
	    if (current_locale != SUDOERS_LOCALE_USER) {
		current_locale = SUDOERS_LOCALE_USER;
		res = setlocale(LC_ALL, user_locale ? user_locale : "");
	    }
	    break;
	case SUDOERS_LOCALE_SUDOERS:
	    if (prevlocale)
		*prevlocale = current_locale;
	    if (current_locale != SUDOERS_LOCALE_SUDOERS) {
		current_locale = SUDOERS_LOCALE_SUDOERS;
		res = setlocale(LC_ALL, def_sudoers_locale);
		if (res == NULL) {
		    if (strcmp(def_sudoers_locale, "C") != 0) {
			efree(def_sudoers_locale);
			def_sudoers_locale = estrdup("C");
			res = setlocale(LC_ALL, "C");
		    }
		}
	    }
	    break;
    }
    return res ? 1 : 0;
}
#else
int
sudoers_setlocale(int newlocale, int *prevlocale)
{
    return 1;
}
#endif /* HAVE_SETLOCALE */
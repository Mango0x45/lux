/*
 * BSD Zero Clause License
 * 
 * Copyright (c) 2022 Thomas Voss
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#define _POSIX_C_SOURCE 2

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lux.h>

static int parseint(char *);
static double parsedouble(char *);
static void usage(char *);

int
main(int argc, char **argv)
{
	int opt;
	struct luxdisp disp;

	luxinit(&disp);
	if (argc == 1)
		goto Gflag;
	else while ((opt = getopt(argc, argv, ":d:D:gGi:I:s:S:")) != -1) {
		switch (opt) {
		case 'g':;
			int br;
			if ((br = luxget(&disp)) == -1)
				err(EXIT_FAILURE, "luxget");
			printf("%d\n", br);
			break;
		case 'G':
Gflag:;
			double pr;
			if ((pr = luxgetp(&disp)) == -1)
				err(EXIT_FAILURE, "luxgetp");
			printf("%f\n", pr);
			break;
		case 'd':
			if (luxdec(&disp, parseint(optarg)) == -1)
				err(EXIT_FAILURE, "luxdec");
			break;
		case 'i':
			if (luxinc(&disp, parseint(optarg)) == -1)
				err(EXIT_FAILURE, "luxinc");
			break;
		case 's':
			if (luxset(&disp, parseint(optarg)) == -1)
				err(EXIT_FAILURE, "luxset");
			break;
		case 'D':
			if (luxdecp(&disp, parsedouble(optarg)) == -1)
				err(EXIT_FAILURE, "luxdecp");
			break;
		case 'I':
			if (luxincp(&disp, parsedouble(optarg)) == -1)
				err(EXIT_FAILURE, "luxincp");
			break;
		case 'S':
			if (luxsetp(&disp, parsedouble(optarg)) == -1)
				err(EXIT_FAILURE, "luxsetp");
			break;
		default:
			usage(argv[0]);
		}
	}

	luxfree(&disp);
	return EXIT_SUCCESS;
}

int
parseint(char *s)
{
	char *endptr;
	long n = strtol(optarg, &endptr, 0);
	if (*s != '\0' && *endptr == '\0')
		return (int) n;
	errx(EXIT_FAILURE, "parseint: invalid integer '%s'", s);
}

double
parsedouble(char *s)
{
	char *endptr;
	double n = strtod(optarg, &endptr);
	if (*s != '\0' && *endptr == '\0')
		return n;
	errx(EXIT_FAILURE, "parsedouble: invalid double '%s'", s);
}

void
usage(char *s)
{
	fprintf(stderr, "Usage: %s [-gG | -dDiIsS val]\n", s);
	exit(EXIT_FAILURE);
}

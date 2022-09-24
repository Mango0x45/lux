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

#define die(...) err(EXIT_FAILURE, __VA_ARGS__);
#define diex(...) errx(EXIT_FAILURE, __VA_ARGS__);

static int parseint(char *);
static double parsedouble(char *);
static void usage(char *);

int
main(int argc, char **argv)
{
	int br, max, opt;
	double pr;
	struct luxdisp disp;

	luxinit(&disp);
	if (argc == 1)
		goto Gflag;
	else while ((opt = getopt(argc, argv, ":d:D:gGi:I:s:S:")) != -1) {
		switch (opt) {
		case 'g':
			if ((br = luxget(&disp)) == -1)
				die("luxget");
			printf("%d\n", br);
			break;
		case 'G':
Gflag:
			if ((pr = luxgetp(&disp)) == -1)
				die("luxgetp");
			printf("%f\n", pr);
			break;
		case 'd':
		case 'i':
		case 's':
			br = parseint(optarg);
			if (opt == 'd')
				br = -br;
			if (opt != 's') {
				int tmp = luxget(&disp);
				if (tmp == -1)
					die("luxget");
				br += luxget(&disp);
			}
			if (br > (max = luxmax(&disp)))
				br = max;
			else if (br < 0)
				br = 0;
			if (luxset(&disp, br) == -1)
				die("luxset");
			break;
		case 'D':
		case 'I':
		case 'S':
			pr = parsedouble(optarg);
			if (opt == 'D')
				pr = -pr;
			if (opt != 'S') {
				double tmp = luxgetp(&disp);
				if (tmp == -1)
					die("luxgetp");
				pr += luxgetp(&disp);
			}
			if (pr > 100)
				pr = 100;
			else if (pr < 0)
				pr = 0;
			if (luxsetp(&disp, pr) == -1)
				die("luxset");
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
	diex("parseint: invalid integer '%s'", s);
}

double
parsedouble(char *s)
{
	char *endptr;
	double n = strtod(optarg, &endptr);
	if (*s != '\0' && *endptr == '\0')
		return n;
	diex("parsedouble: invalid double '%s'", s);
}

void
usage(char *s)
{
	fprintf(stderr, "Usage: %s [-gG | -dDiIsS val]\n", s);
	exit(EXIT_FAILURE);
}

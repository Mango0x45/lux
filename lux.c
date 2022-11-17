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

#include <err.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
	lux_t disp;
	const char *optstr = "d:D:gGi:I:ms:S:";
	static struct option longopts[] = {
		{"decrease",         required_argument, NULL, 'd'},
		{"decrease-percent", required_argument, NULL, 'D'},
		{"get",              no_argument,       NULL, 'g'},
		{"get-percent",      no_argument,       NULL, 'G'},
		{"increase",         required_argument, NULL, 'i'},
		{"increase-percent", required_argument, NULL, 'I'},
		{"max",              no_argument,       NULL, 'm'},
		{"set",              required_argument, NULL, 's'},
		{"set-percent",      required_argument, NULL, 'S'},
		{NULL,               0,                 NULL,  0 }
	};

	luxinit(&disp);
	if (argc == 1)
		goto Gflag;
	else while (true) {
		opt = getopt_long(argc, argv, optstr, longopts, NULL);
		if (opt == -1)
			break;

		switch (opt) {
		case 'm':
			if ((max = luxmax(&disp)) == -1)
				die("luxmax");
			printf("%d\n", max);
			break;
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
	fprintf(stderr, "Usage: %s [-gGm] [-dDiIsS val]\n", s);
	exit(EXIT_FAILURE);
}

#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <errno.h>

char f_name[MAXLEN];
int ti = 1, pool_siz = 5;
unsigned int seed = 0;
FILE *out_fd;

int copy_string(char *cnf, const char *arg) {
    int len = strlen(arg);
    // too long file/directoty name
    if (len >= MAXLEN) return 1;
    // copy string
    memcpy(cnf, arg, len + 1);
    return 0;
}

int copy_number(int *cnf, const char *arg) {
    char *endptr;
    errno = 0;
    // set 0 to auto choose 10, 8 or 16
    *cnf = strtol(arg, &endptr, 0);
    // something wrong when convert
    if (errno != 0 || *endptr != 0) return 2;
    return 0;
}

void show_help(char *name) {
    printf("Usage: %s [OPTION]...\n", name);
    printf(" -t NUMBER -- lasting time (seconds) (default: 1)\n");
    printf(" -f STRING -- log file (default: None)\n");
    printf("              Default to stdout\n");
    printf(" -r NUMBER -- rand seed (default: defined on time)\n");
    printf(" -h        -- show this message and exit\n");
    exit(EXIT_SUCCESS);
}

int parse_args(int argc, char *argv[]) {
    static const char optstring[] = "t:f:r:h";
    while (1) {
        int optidx, err = 0;
        int c = getopt_long(argc, argv, optstring, NULL, &optidx);
        if (c == -1) break;
        switch (c) {
            case 'h': show_help(argv[0]); break;
            case 't': {
                if (copy_number(&ti, optarg) != 0) {
                    printf("Bad argment for time!\n");
                    return -1;
                }
                break;
            }
            case 'f': {
                if (copy_string(f_name, optarg) != 0) {
                    printf("Too long argment for file!\n");
                    return -1;
                }
                break;
            }
            case 'r': {
                if (copy_number((int *)&seed, optarg) != 0) {
                    printf("Bad argment for rand seed!\n");
                    return -1;
                }
                break;
            }
            case '?': return -1;
        }
    }
    if (f_name[0]) {
        out_fd = fopen(f_name, "w");
        if (out_fd == NULL) return perror(f_name), -1;
    } else out_fd = stdout;
    if (seed != 0) srand(seed);
    else srand(time(NULL));
    return 0;
}
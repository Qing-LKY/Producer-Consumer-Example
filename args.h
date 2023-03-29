#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>

#define MAXLEN 50

extern int ti, pool_siz;
extern FILE *out_fd;

int parse_args(int argc, char *argv[]);

#endif
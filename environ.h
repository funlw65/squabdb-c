/*
 * environ.h
 *
 * Generated by configure
 *
 */

#ifndef TYPHOON_ENVIRON_H
#define TYPHOON_ENVIRON_H

#include <sys/types.h>
#include <stddef.h>
#define CONFIG_CREATMASK 0666
#define CONFIG_O_BINARY  0
#define CONFIG_DIR_SWITCH '/'
#ifndef offsetof
#define offsetof(s,m) ((int)&(((s *)0))->m)
#endif
#if defined(sparc) || defined(mips) || defined(HPUX) || defined(__alpha)
#define CONFIG_RISC 1
#endif
#define CONFIG_UNIX 1
#define CONFIG_LITTLE_ENDIAN 1
#define CONFIG_PROTOTYPES 1
#define PRM(x)   x
#define CONFIG_ELLIPSIS ,...
#define CONFIG_CONST const
typedef unsigned char uchar;
#define CONFIG_USE_FLOCK 1
#endif
/* end-of-file */

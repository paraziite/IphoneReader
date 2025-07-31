/**
 * common.h
 * Common macros and definitions for libirecovery
 */

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef WIN32
#   define snprintf _snprintf
#   define strcasecmp _stricmp
#   define strdup _strdup
#endif

/* Memory management macros */
#define SAFE_FREE(x) do { if (x) { free(x); x = NULL; } } while (0)

/* Boolean type definitions for C89 compatibility */
#ifndef __cplusplus
#   ifndef bool
#       define bool int
#       define true 1
#       define false 0
#   endif
#endif

/* Debugging support */
extern int verbose;


#define error(...) do { fprintf(stderr, __VA_ARGS__); } while (0)

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

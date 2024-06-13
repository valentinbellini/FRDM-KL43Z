// debug.h
#ifndef DEBUG_H
#define DEBUG_H

#include "fsl_debug_console.h"
#include <assert.h>

#ifdef DEBUG
    #define DEBUG_PRINT(...) PRINTF(__VA_ARGS__)
    #define ASSERT(condition) assert(condition)
	#define LOG_ERROR(message) do { \
		FILE *error_log_file = fopen("error.log", "a"); \
		if (error_log_file != NULL) { \
			fprintf(error_log_file, "[ERROR] File: %s, Line: %d: %s\n", __FILE__, __LINE__, message); \
			fclose(error_log_file); \
		} else { \
			DEBUG_PRINT("[ERROR] Failed to open error log file.\n"); \
			DEBUG_PRINT("[ERROR] File: %s, Line: %d: %s\n", __FILE__, __LINE__, message); \
		} \
	} while(0)
#else
    #define DEBUG_PRINT(...) (void)0
    #define ASSERT(condition) (void)0
    #define LOG_ERROR(message) (void)0
#endif

#endif // DEBUG_H

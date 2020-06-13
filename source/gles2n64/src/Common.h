#ifndef __COMMON_H__
#define __COMMON_H__

//#define PROFILE_GBI

#define LOG_NONE	0
#define LOG_ERROR   1
#define LOG_MINIMAL	2
#define LOG_WARNING 3
#define LOG_VERBOSE 4

#define LOG_LEVEL LOG_ERROR

# ifndef min
#  define min(a,b) ((a) < (b) ? (a) : (b))
# endif
# ifndef max
#  define max(a,b) ((a) > (b) ? (a) : (b))
# endif


#if LOG_LEVEL>0

#define LOG(A, ...) \
    if (A <= LOG_LEVEL) \
    { \
        if (A == LOG_WARNING) printf("(WW) "); \
        if (A == LOG_ERROR) printf("(EE) "); \
        printf(__VA_ARGS__); \
        fflush(stdout); \
    }

#else

#define LOG(A, ...)

#endif

#endif

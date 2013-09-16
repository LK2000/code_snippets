#ifndef _MLOG_H
#define _MLOG_H

/*
 * Note: in your .c file, #define MODULE_NAME, 8 chars long, to print each
 *       log entry with your module name
 */

#define LOCAL_DEBUG

#define LOG_ERROR       0
#define LOG_INFO        1
#define LOG_DEBUG_LOW   2
#define LOG_DEBUG_HIGH  3
#define LOG_LEVEL       LOG_DEBUG_HIGH

/*
 * print always
 */

#define log_error(_fmt, ...)                                          \
do                                                                    \
{                                                                     \
    printf("%s %s: %s: %d: ERROR: " _fmt, MODULE_NAME,  __FILE__,     \
           __func__, __LINE__, ## __VA_ARGS__);                       \
}                                                                     \
while(0)

#define log_always(_fmt, ...)                                         \
do                                                                    \
{                                                                     \
    printf("%s %s: %s: %d: ALWAYS: " _fmt, MODULE_NAME, __FILE__,     \
           __func__, __LINE__, ## __VA_ARGS__);                       \
}                                                                     \
while(0)

#define GOT_HERE() printf("%s %s: %s: %d: ###### GOT HERE ###### \n", \
                          MODULE_NAME, __FILE__, __func__, __LINE__)

/*
 * print conditionally
 */

#ifdef LOCAL_DEBUG
#define log_info(_fmt, ...)                                           \
do                                                                    \
    if (LOG_INFO <= LOG_LEVEL)                                        \
    {                                                                 \
        printf("%s %s: %s: %d: INFO:  " _fmt, MODULE_NAME,  __FILE__, \
               __func__, __LINE__, ## __VA_ARGS__);                   \
    }                                                                 \
while(0)
#else
#define log_info(_fmt, ...)
#endif

#ifdef LOCAL_DEBUG
#define log_debug_low(_fmt, ...)                                      \
do                                                                    \
    if (LOG_DEBUG_LOW <= LOG_LEVEL)                                   \
    {                                                                 \
        printf("%s %s: %s: %d: DEBUG: " _fmt, MODULE_NAME,  __FILE__, \
               __func__, __LINE__, ## __VA_ARGS__);                   \
    }                                                                 \
while(0)
#else
#define log_debug_low(_fmt, ...)
#endif

#ifdef LOCAL_DEBUG
#define log_debug_high(_fmt, ...)                                     \
do                                                                    \
    if (LOG_DEBUG_HIGH <= LOG_LEVEL)                                  \
    {                                                                 \
        printf("%s %s: %s: %d: DEBUG: " _fmt, MODULE_NAME,  __FILE__, \
               __func__, __LINE__, ## __VA_ARGS__);                   \
    }                                                                 \
while(0)
#else
#define log_debug_high(_fmt, ...)
#endif

#endif /* #ifndef _MLOG_H */

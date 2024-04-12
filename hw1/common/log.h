// Disclaimer: Got help from ChatGPT on colored printf and __VA_ARGS__

#ifndef HW1_LOG_H
#define HW1_LOG_H
#pragma once

#include <stdio.h>
#include <time.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define LOG_COMMON(log_color, log_msg, ...) do {\
                                time_t t = time(NULL);\
                                struct tm *tm = localtime(&t);\
                                printf("[%d-%02d-%02d %02d:%02d:%02d] [%s" log_msg"%s] %s\n",\
                                       tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,\
                                       tm->tm_hour, tm->tm_min, tm->tm_sec,                \
                                       log_color, KWHT,              \
                                       __VA_ARGS__);\
                            } while(0)

#define LOG_INFO(...) LOG_COMMON(KGRN, "INFO", __VA_ARGS__)
#define LOG_WARN(...) LOG_COMMON(KYEL, "WARN", __VA_ARGS__)
#define LOG_ERROR(...) LOG_COMMON(KRED, "ERROR", __VA_ARGS__)

#endif //HW1_LOG_H

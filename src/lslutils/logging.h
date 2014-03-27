#ifndef LSL_LOGGING_H
#define LSL_LOGGING_H

#include <cstdio>


extern void lsllogerror(const char* format, ...);

#define LslError(...)   do {lsllogerror(__VA_ARGS__);} while(0)
#define LslDebug(...)   do {lsllogerror(__VA_ARGS__);} while(0)
#define LslWarning(...) do {lsllogerror(__VA_ARGS__);} while(0)
#define LslDebug(...)   do {lsllogerror(__VA_ARGS__);} while(0)



#endif // LSL_LOGGING_H

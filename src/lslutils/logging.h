#ifndef LSL_LOGGING_H
#define LSL_LOGGING_H

#include <cstdio>

#define LslError(...)   do {printf(__VA_ARGS__);} while(0)
#define LslDebug(...)   do {printf(__VA_ARGS__);} while(0)
#define LslWarning(...) do {printf(__VA_ARGS__);} while(0)
#define LslDebug(...)   do {printf(__VA_ARGS__);} while(0)


#endif // LSL_LOGGING_H

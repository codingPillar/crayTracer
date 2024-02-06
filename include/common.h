#ifndef TRACER_COMMON_H
#define TRACER_COMMON_H

#ifndef WEBASSEMBLY
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MALLOC(_x) malloc(_x)

#else

#endif

#endif //TRACER_COMMON_H
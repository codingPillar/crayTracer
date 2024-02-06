#ifndef TRACER_COMMON_H
#define TRACER_COMMON_H

#define MALLOC(_x) malloc(_x)

#ifndef WEBASSEMBLY

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#else

/* THIS LIB IS SUPPOSED TO BE COMPILED AS A WASM32 TARGET */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef long long unsigned int uint64_t;
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int int64_t;

extern void *malloc (unsigned int __size);
extern float sqrtf(float __x);
extern float fabs(float __x);

#endif

#endif //TRACER_COMMON_H
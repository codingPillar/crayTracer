#ifndef TRACER_GRAPHICS_H
#define TRACER_GRAPHICS_H

#include <stdint.h>

#include "tracer.h"

#define GRAPHICS_BLACK 0x00000000
#define GRAPHICS_WHITE 0xFFFFFFFF
#define GRAPHICS_RED 0xFF0000FF
#define GRAPHICS_PURPLE 0xFF00FFFF
/* TODO, IMPLEMENT */
#define GRAPHICS_LIGHT_BLUE 0xFF0000FF

struct VBuffer{
    unsigned int width, height;
    uint32_t *buffer;
};

void tracer_init_buffer(struct VBuffer *buffer, unsigned int width, unsigned int height);

/* SIMPLE RASTER SHAPES (NOT RAYTRACED) */
void tracer_fill_rect(struct VBuffer *buffer, struct Vec3 corner, unsigned int width, unsigned int height, uint32_t color);
void tracer_fill_circle(struct VBuffer *buffer, struct Vec3 center, int radius, uint32_t color);

#endif //TRACER_GRAPHICS_H
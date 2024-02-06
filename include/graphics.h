#ifndef TRACER_GRAPHICS_H
#define TRACER_GRAPHICS_H

#include <stdint.h>

#include "tracer.h"

#define GRAPHICS_BLACK      0x00000000
#define GRAPHICS_WHITE      0xFFFFFFFF
#define GRAPHICS_RED        0xFF0000FF
#define GRAPHICS_PURPLE     0xFF00FFFF
#define GRAPHICS_GREEN      0x00FF00FF
#define GRAPHICS_YELLOW     0xFFFF00FF
#define GRAPHICS_LIGHT_BLUE 0x00FFFFFF

struct VBuffer{
    unsigned int width, height;
    uint32_t *buffer;
};

void tracer_init_buffer(struct VBuffer *buffer, unsigned int width, unsigned int height);

uint32_t tracer_color_from_vec3(struct Vec3 color);

/* SIMPLE RASTER SHAPES (NOT RAYTRACED) */
void tracer_stroke_line(struct VBuffer *buffer, struct Vec3 start, struct Vec3 end, uint32_t color);
void tracer_fill_rect(struct VBuffer *buffer, struct Vec3 corner, unsigned int width, unsigned int height, uint32_t color);
void tracer_fill_circle(struct VBuffer *buffer, struct Vec3 center, int radius, uint32_t color);

struct Scene{
    struct Vec3 camera;
    struct Vec3 projectionDomain;
    struct ModelArray models;
};

void drawScene(const struct Scene *scene, struct VBuffer *vbuffer);

#endif //TRACER_GRAPHICS_H
#ifndef TRACER_H
#define TRACER_H

#include <stdint.h>

#define VEC3_DIM_COUNT 3
struct VBuffer{
    unsigned int width, height;
    uint32_t *buffer;
};

void tracer_init_buffer(struct VBuffer *buffer, unsigned int width, unsigned int height);

/* TRACER MATH */
struct Vec3{
    float x, y, z;
};
struct Vec3 vec3_add(const struct Vec3 first, const struct Vec3 second);
struct Vec3 vec3_sub(const struct Vec3 first, const struct Vec3 second);
struct Vec3 vec3_scale(const struct Vec3 vec, float factor);
struct Vec3 vec3_normalize(const struct Vec3 vec);
float vec3_length(const struct Vec3 vec);

struct Ray{
    struct Vec3 origin;
    struct Vec3 direction;
};
struct Vec3 ray_get_position(struct Ray ray, float t);

enum IntersectionState{
    INTESECT_NON_VALID,
    INTESECT_VALID,
    /* TODO, MAYBE ADD MORE STATES */
};
typedef enum IntersectionState (*IntersectionCallback)(struct Ray ray, const void *shape, struct Vec3 *intersection);

struct Shape{
    void *data;
    IntersectionCallback callback;
};

#endif //TRACER_H
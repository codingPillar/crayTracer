#ifndef TRACER_H
#define TRACER_H

#define VEC3_DIM_COUNT 3
#define MAX_NUM_MODELS 50

/* TRACER MATH */
struct Vec3{
    float x, y, z;
};
struct Plane{
    struct Vec3 first;
    struct Vec3 second;
};
struct Vec3 vec3_add(const struct Vec3 first, const struct Vec3 second);
struct Vec3 vec3_sub(const struct Vec3 first, const struct Vec3 second);
float vec3_dot(const struct Vec3 first, const struct Vec3 second);
struct Vec3 vec3_scale(const struct Vec3 vec, float factor);
struct Vec3 vec3_normalize(const struct Vec3 vec);
float vec3_length(const struct Vec3 vec);

struct Ray{
    struct Vec3 origin;
    struct Vec3 direction;
    struct Vec3 color;
};
struct Vec3 ray_get_position(struct Ray ray, float t);

enum IntersectionState{
    INTESECT_NON_VALID,
    INTESECT_VALID,
    /* TODO, MAYBE ADD MORE STATES */
};
typedef enum IntersectionState (*IntersectionCallback)(struct Ray ray, const void *shape, struct Vec3 *intersection);
typedef struct Ray (*ReflectionCallback)(struct Ray ray, const void *shape, struct Vec3 intersection);

struct Shape{
    void *data;
    IntersectionCallback intersectionCallback;
    ReflectionCallback reflectionCallback;
};

/* TODO, USE DYNAMIC ARRAY */
struct ModelArray{
    struct Shape shapes[MAX_NUM_MODELS];
    unsigned int count;
};
void models_push_back(struct ModelArray *models, struct Shape shape);
struct Vec3 tracer_get_pixel_color(struct Ray ray, const struct ModelArray *models, unsigned int reflectionCount);

struct Camera{
    struct Vec3 position;
    struct Vec3 direction;
};

void tracer_lookat(struct Camera *camera, struct Vec3 destination);
struct Plane tracer_get_projection_plane(const struct Camera *camera);

struct Scene{
    struct Camera camera;
    struct Vec3 projectionDomain;
    struct ModelArray models;
};

#endif //TRACER_H
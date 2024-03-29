#ifndef TRACER_SPHERE_H
#define TRACER_SPHERE_H

#include "tracer.h"

struct Sphere{
    float radius;
    struct Vec3 center;
    struct Vec3 color;
};

struct Shape tracer_get_sphere(struct Sphere *shere);
enum IntersectionState tracer_sphere_collision(struct Ray ray, const struct Sphere *sphere, struct Vec3 *intersection);
struct Ray tracer_sphere_reflect(struct Ray ray, const struct Sphere *sphere, struct Vec3 intersection);

#endif //TRACER_SPHERE_H
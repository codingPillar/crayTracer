#include <math.h>
#include <string.h>

#include "common.h"
#include "sphere.h"
#include "tracer.h"

#define VECTORIAL_METHOD

enum IntersectionState tracer_sphere_collision_helper(struct Ray ray, const void *sphere, struct Vec3 *intersection){
    return tracer_sphere_collision(ray, (const struct Sphere*) sphere, intersection);
}
struct Ray tracer_sphere_reflect_helper(struct Ray ray, const void *sphere, struct Vec3 intersection){
    return tracer_sphere_reflect(ray, (const struct Sphere*) sphere, intersection);
}

struct Shape tracer_get_sphere(struct Sphere *shere){
    return (struct Shape) {.data = shere, .intersectionCallback = tracer_sphere_collision_helper, .reflectionCallback = tracer_sphere_reflect_helper};
}

#ifndef VECTORIAL_METHOD
enum IntersectionState tracer_sphere_collision(struct Ray ray, const struct Sphere *sphere, struct Vec3 *intersection){
    float direction[] = {ray.direction.x, ray.direction.y, ray.direction.z};
    float origin[] = {ray.origin.x, ray.origin.y, ray.origin.z};
    float center[] = {sphere->center.x, sphere->center.y, sphere->center.z};
    float tFactor = 0.f, tsquaredFactor = 0.f, rest = 0.f; 
    for(unsigned int i = 0; i < VEC3_DIM_COUNT; i++){
        tFactor += 2 * direction[i] * (origin[i] - center[i]);
        tsquaredFactor += direction[i] * direction[i];
        rest += origin[i] * origin[i] - 2 * origin[i] * center[i] + center[i] * center[i];
    }
    rest -= sphere->radius * sphere->radius;
    /* SOLVE QUADRATIC */
    const float delta = tFactor * tFactor - 4 * tsquaredFactor * rest;
    if(delta < 0) return INTESECT_NON_VALID;
    const float t1 = (-tFactor - sqrtf(delta)) / (2 * tsquaredFactor);
    const float t2 = (-tFactor + sqrtf(delta)) / (2 * tsquaredFactor);
    if(t1 < 0 && t2 < 0) return INTESECT_NON_VALID;
    if(t1 < 0) *intersection = ray_get_position(ray, t2);
    else *intersection = ray_get_position(ray, t1);
    return INTESECT_VALID;
}
#else
enum IntersectionState tracer_sphere_collision(struct Ray ray, const struct Sphere *sphere, struct Vec3 *intersection){
    const float c = vec3_dot(ray.origin, ray.origin) - 2 * vec3_dot(ray.origin, sphere->center) + vec3_dot(sphere->center, sphere->center) - sphere->radius * sphere->radius;
    const float b = 2.f * (vec3_dot(ray.direction, ray.origin) - vec3_dot(ray.direction, sphere->center));
    const float a = vec3_dot(ray.direction, ray.direction);
    /* SOLVE QUADRATIC */
    const float delta = b * b - 4 * a * c;
    if(delta < 0) return INTESECT_NON_VALID;
    const float t1 = (-b - sqrtf(delta)) / (2 * a);
    const float t2 = (-b + sqrtf(delta)) / (2 * a);
    if(t1 < 0 && t2 < 0) return INTESECT_NON_VALID;
    if(t1 < 0) *intersection = ray_get_position(ray, t2);
    else *intersection = ray_get_position(ray, t1);
    return INTESECT_VALID;
}
#endif

struct Ray tracer_sphere_reflect(struct Ray ray, const struct Sphere *sphere, struct Vec3 intersection){
    /* GET THE NORMAL AT THE INTERSECTION POINT */
    const struct Vec3 normal = vec3_normalize(vec3_sub(intersection, sphere->center));
    const float dot = vec3_dot(vec3_normalize(ray.direction), normal);
    const struct Vec3 reflection = vec3_normalize(vec3_sub(ray.direction, vec3_scale(normal, 2 * dot)));
    return (struct Ray) {.origin = intersection, .direction = reflection, .color = sphere->color};
}

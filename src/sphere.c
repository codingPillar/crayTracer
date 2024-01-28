#include "sphere.h"
#include <math.h>

enum IntersectionState tracer_sphere_collision(struct Ray ray, const struct Sphere *sphere, struct Vec3 *intersection){
    float direction[] = {ray.direction.x, ray.direction.y, ray.direction.z};
    float origin[] = {ray.origin.x, ray.origin.y, ray.origin.z};
    float center[] = {sphere->center.x, sphere->center.y, sphere->center.z};
    float tFactor = 0.f, tsquaredFactor = 0.f, rest = 0.f; 
    for(unsigned int i = 0; i < VEC3_DIM_COUNT; i++){
        tFactor += 2 * direction[i] * (origin[i] - center[i]);
        tsquaredFactor += direction[i];
        rest += origin[i] * origin[i] - 2 * origin[i] * center[i] + center[i] * center[i];
    }
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
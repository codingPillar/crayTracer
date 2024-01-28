#include <math.h>

#include "tracer.h"

/* TRACER MATH */
struct Vec3 vec3_add(const struct Vec3 first, const struct Vec3 second){
    return (struct Vec3){.x = first.x + second.x, .y = first.y + second.y, .z = first.z + second.z};
}

struct Vec3 vec3_sub(const struct Vec3 first, const struct Vec3 second){
    return (struct Vec3){.x = first.x - second.x, .y = first.y - second.y, .z = first.z - second.z};
}

float vec3_dot(const struct Vec3 first, const struct Vec3 second){
    return first.x * second.x + first.y * second.y + first.z * second.z;
}

struct Vec3 vec3_scale(const struct Vec3 first, float factor){
    return (struct Vec3) {.x = first.x * factor, .y = first.y * factor, .z = first.z * factor};
}

struct Vec3 vec3_normalize(const struct Vec3 vec){
    const float length = vec3_length(vec);
    return (struct Vec3) {.x = vec.x / length, .y = vec.y / length, .z = vec.z / length};
}

float vec3_length(const struct Vec3 vec){
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

/* RAY FUNCTIONS */
struct Vec3 ray_get_position(struct Ray ray, float t){
    return vec3_add(ray.origin, vec3_scale(ray.direction, t));
}
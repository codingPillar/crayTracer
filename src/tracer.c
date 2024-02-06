#include "common.h"
#include "tracer.h"

#define EPSILON 0.0001f

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

void models_push_back(struct ModelArray *models, struct Shape shape){
    models->shapes[models->count] = shape;
    models->count++;
}

struct Vec3 tracer_get_pixel_color(struct Ray ray, const struct ModelArray *models, unsigned int reflectionCount){
    for(unsigned int i = 0; i < reflectionCount; i++){
        /* FIND THE NEAREST INTERSECTION WITH THE MODELS */
        enum IntersectionState intersectionState = INTESECT_NON_VALID;
        struct Vec3 intersection = {0};
        unsigned int shapeIndex;
        for(unsigned int j = 0; j < models->count; j++){
            struct Vec3 currentIntersection = {0};
            enum IntersectionState current = models->shapes[j].intersectionCallback(ray, models->shapes[j].data, &currentIntersection);
            if(current != INTESECT_VALID) continue;
            enum IntersectionState previousState = intersectionState;
            /* FIRST VALID INTERSECTION FOUND */
            intersectionState = INTESECT_VALID;
            if(previousState == INTESECT_NON_VALID) goto setMin;
            /* WE WANT TO KEEP THE CLOSEST INTERSECTION */
            const float currentLength = vec3_length(vec3_sub(ray.origin, currentIntersection));
            const float length = vec3_length(vec3_sub(ray.origin, intersection));
            if(currentLength > length) continue;
            setMin:
            intersection = currentIntersection;
            shapeIndex = j;
        }
        /* RAY WONT COLLIDE WITH ANYTHING ANYMORE */
        if(intersectionState == INTESECT_NON_VALID) break;
        /* WE WANT TO COMPUTE REFLECTION ONLY FOR THE CLOSEST INTERSECTION POINT */
        ray = models->shapes[shapeIndex].reflectionCallback(ray, models->shapes[shapeIndex].data, intersection);
    }
    return ray.color;
}

void tracer_lookat(struct Camera *camera, struct Vec3 destination){
    camera->direction = vec3_sub(destination, camera->position);
}

struct Plane tracer_get_projection_plane(const struct Camera *camera){
    /* WE WANT TO FIND IN PLANE UNIT VECTORS PERPENDICULAR */
    /* TODO, IMPLEMENT */
    struct Plane plane = {0};
    plane.first.z = (fabs(camera->direction.x) - fabs(camera->direction.z) < EPSILON) ? sqrtf(0.5f) : 
        fabs(camera->direction.x) / sqrtf(camera->direction.x * camera->direction.x + camera->direction.z * camera->direction.z);
    plane.first.x = (fabs(camera->direction.x) < EPSILON) ? 0.f : -camera->direction.z * plane.first.z / camera->direction.x;
    /* MISSING SOLVING SECOND VECTOR */
    return (struct Plane) {.first = {.x = 1.f}, .second = {.y = 1.f}};
}
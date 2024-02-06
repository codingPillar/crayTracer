#include "common.h"

#include "tracer.h"
#include "graphics.h"
#include "sphere.h"

struct Scene scene = {0};
struct VBuffer vbuffer = {0};
struct Sphere sphere1 = {.radius = 0.5f, .center = {.z = -1.f}, .color = {.x = 0.5f, .z = 1.f}};
struct Sphere sphere2 = {.radius = 0.5f, .center = {.x = -0.5f, .z = -2.f}, .color = {.x = 1.f, .z = .5f}};

void init_scene(unsigned int width, unsigned int height){
    tracer_init_buffer(&vbuffer, width, height);

    scene.camera.position.z = 1.f;
    scene.projectionDomain.x = (float)width / height;
    scene.projectionDomain.y = 1.0;

    tracer_lookat(&scene.camera, (struct Vec3){0});
    models_push_back(&scene.models, tracer_get_sphere(&sphere1));
    models_push_back(&scene.models, tracer_get_sphere(&sphere2));

    tracer_fill_rect(&vbuffer, (struct Vec3) {.x = 0.f, .y = 0.f}, width, height, GRAPHICS_BLACK);
    
    tracer_draw_scene(&scene, &vbuffer);
}

uint32_t* get_color_buffer(){
    return vbuffer.buffer;
}
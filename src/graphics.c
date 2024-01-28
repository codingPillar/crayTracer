#include "common.h"
#include "graphics.h"
#include "tracer.h"
#include <stdint.h>

void tracer_init_buffer(struct VBuffer *buffer, unsigned int width, unsigned int height){
    buffer->width = width;
    buffer->height = height;
    buffer->buffer = MALLOC(width * height * sizeof(uint32_t));
}

void tracer_fill_rect(struct VBuffer *buffer, struct Vec3 corner, unsigned int width, unsigned int height, uint32_t color){
    for(unsigned int j = 0; j < height; j++){
        int currentY = (int)corner.y + j;
        if(currentY < 0 || (unsigned int)currentY > buffer->height) continue;
        for(unsigned int i = 0; i < width; i++){
            int currentX = (int)corner.x + i;
            if(currentX < 0 || (unsigned int)currentX > buffer->width) continue;
            buffer->buffer[currentY * buffer->width + currentX] = color;
        }
    }
}

void tracer_fill_circle(struct VBuffer *buffer, struct Vec3 center, int radius, uint32_t color){
    struct Vec3 corner = vec3_sub(center, (struct Vec3) {.x = radius, .y = radius});
    for(int i = -radius; i < radius ; i++){
        int heightOffset = (int)corner.y + i + radius;
        if(0 > heightOffset || (unsigned int)heightOffset > buffer->height) continue;
        for(int j = -radius; j < radius; j++){
            if(i * i + j * j > radius * radius) continue;
            int widthOffset = (int)corner.x + j + radius;
            if(0 > widthOffset || (unsigned int)widthOffset > buffer->width) continue;
            buffer->buffer[heightOffset * buffer->width + widthOffset] = color;
        }
    }
}
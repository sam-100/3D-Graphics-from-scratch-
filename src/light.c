#include "light.h"

light_t light = {
    {0, -4, 10}
};

uint32_t light_apply_intensity(uint32_t color, float intensity) {
    uint32_t new_color;
    uint32_t a = (color & 0xff000000);
    uint32_t r = (color & 0x00ff0000) * intensity;
    uint32_t g = (color & 0x0000ff00) * intensity;
    uint32_t b = (color & 0x000000ff) * intensity;

    new_color = (a & 0xff000000) + (r & 0x00ff0000) + (g & 0x0000ff00) + (b & 0x000000ff);
    return new_color;
}
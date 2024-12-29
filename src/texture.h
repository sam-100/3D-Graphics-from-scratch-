#include <stdint.h>

extern int texture_width;
extern int texture_height;

extern uint32_t* mesh_texture;

extern const uint8_t REDBRICK_TEXTURE[];

typedef struct tex2_t {
    float u;
    float v;
} tex2_t;


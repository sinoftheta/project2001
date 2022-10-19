#ifndef __fgl_vertex_h__
#define __fgl_vertex_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include <libfixmath/fixmath.h>
//#include <fgl/fgl_color.h>

typedef struct {
	fix16_t x, y, z;
} fgl_vec3_t;

static inline fix16_t fix16_add3(fix16_t a, fix16_t b, fix16_t c){ return fix16_add(fix16_add(a,b),c); };

static inline fix16_t fgl_vec3_magnitude(fgl_vec3_t in){
	return fix16_sqrt(fix16_add3(fix16_sq(in.x), fix16_sq(in.y), fix16_sq(in.z)));
}

void fgl_vec3_print(fgl_vec3_t inVertex);

fgl_vec3_t fgl_vec3_add(fgl_vec3_t a, fgl_vec3_t b);

fgl_vec3_t fgl_vec3_sub(fgl_vec3_t a, fgl_vec3_t b);

fgl_vec3_t fgl_vec3_normalize(fgl_vec3_t in);

fix16_t fgl_vec3_dot(fgl_vec3_t a, fgl_vec3_t b);

fgl_vec3_t fgl_vec3_cross(fgl_vec3_t a, fgl_vec3_t b);


#ifdef __cplusplus
}
#endif

#endif // __fgl_vertex_h__

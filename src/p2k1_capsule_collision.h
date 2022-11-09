#ifndef __p2k1_capsule_collision_h__
#define __p2k1_capsule_collision_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include "raylib.h"        // raylib
#include "raymath.h"
#include "fixmath.h"       // libfixmath

//#include "fgl_transform.h" // fgl_transform, fgl_matrix, fgl_vertex
#include "fgl.h"

fgl_vec3_t closest_point_on_line_segment(fgl_vec3_t a, fgl_vec3_t b, fgl_vec3_t p);

bool capsule_collision(
    fgl_vec3_t a_A, fgl_vec3_t a_B, fix16_t a_rad, 
    fgl_vec3_t b_A, fgl_vec3_t b_B, fix16_t b_rad
);

#ifdef __cplusplus
}
#endif


#endif // __p2k1_capsule_collision_h__
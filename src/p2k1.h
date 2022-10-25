#ifndef __p2k1_h__
#define __p2k1_h__


#include <iostream>
#include <math.h>
#include "ggponet.h"       // ggpo
#include "platform_unix.h" // ggpo
#include "raylib.h"        // raylib
#include "raymath.h"
#include "fixmath.h"       // libfixmath
#include "GCAdapter.h"     // gcaplus
#include "fgl_transform.h" // fgl_transform, fgl_matrix, fgl_vertex
#include "p2k1_capsule_collision.h"

#define DEBUG_FRAME_INCRIMENT 5
#define DEBUG_ANIM_SPEED 20.0f

// fix16_to_float(fix16_t) is provided in fix16.h
static inline Vector3 fgl_vec3_to_float_vector3(fgl_vec3_t v){ return (Vector3) {fix16_to_float(v.x),fix16_to_float(v.y),fix16_to_float(v.z)}; }

#endif // __p2k1_h__
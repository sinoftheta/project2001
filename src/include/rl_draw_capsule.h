/* 
 * possible extension for raylib 
 *
 * based on DrawSphereEx() and DrawCylinderEx() in rmodels
 *
*/
#ifndef __rl_draw_capsule_h__
#define __rl_draw_capsule_h__

#include "raylib.h" 
//#include "utils.h"          // Required for: TRACELOG(), LoadFileData(), LoadFileText(), SaveFileText()
#include "rlgl.h"           // OpenGL abstraction layer to OpenGL 1.1, 2.1, 3.3+ or ES2
#include "raymath.h"        // Required for: Vector3, Quaternion and Matrix functionality

//#include <stdio.h>          // Required for: sprintf()
//#include <stdlib.h>         // Required for: malloc(), free()
//#include <string.h>         // Required for: memcmp(), strlen()
#include <math.h>           // Required for: sinf(), cosf(), sqrtf(), fabsf()


#ifdef __cplusplus
extern "C"
{
#endif

void DrawCapsule(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color); // Draw a capsule with the first radius at startPos, and the second radius at endPos
void DrawCapsuleWires(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color); // Draw capsule wires with the first radius at startPos, and the second radius at endPos

#ifdef __cplusplus
}
#endif


#endif // __rl_draw_capsule_h__
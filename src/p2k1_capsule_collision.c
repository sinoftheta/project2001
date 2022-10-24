#include "p2k1_capsule_collision.h"
/*
 Implementation of math found in the wicked engine: https://wickedengine.net/2020/04/26/capsule-collision-detection/ by turanszkij
*/


fgl_vec3_t closest_point_on_line_segment(fgl_vec3_t a, fgl_vec3_t b, fgl_vec3_t p){

    fgl_vec3_t ab = fgl_vec3_sub(b, a);

    fix16_t t = fix16_div(fgl_vec3_dot(fgl_vec3_sub(p, a), ab), fgl_vec3_dot(ab, ab));

    return fgl_vec3_add(a, fgl_vec3_scale(ab, fix16_clamp(t, 0, fix16_one)));
}

bool capsule_collision(
    fgl_vec3_t a_A, fgl_vec3_t a_B, fix16_t a_rad, 
    fgl_vec3_t b_A, fgl_vec3_t b_B, fix16_t b_rad
){

    // vectors between line endpoints:
    fgl_vec3_t v0 = fgl_vec3_sub(b_A, a_A);
    fgl_vec3_t v1 = fgl_vec3_sub(b_B, a_A);
    fgl_vec3_t v2 = fgl_vec3_sub(b_A, a_B);
    fgl_vec3_t v3 = fgl_vec3_sub(b_B, a_B);

    // squared distances:
    fix16_t d0 = fgl_vec3_dot(v0,v0);
    fix16_t d1 = fgl_vec3_dot(v1,v1);
    fix16_t d2 = fgl_vec3_dot(v2,v2);
    fix16_t d3 = fgl_vec3_dot(v3,v3);

    // select best potential endpoint on capsule A:
    fgl_vec3_t best_a;
    if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1)
    {
        best_a = a_B;
    }
    else
    {
        best_a = a_A;
    }

    // select point on capsule B line segment nearest to best potential endpoint on A capsule:
    fgl_vec3_t best_b = closest_point_on_line_segment(b_A, b_B, best_a);

    // now do the same for capsule A segment:
    best_a = closest_point_on_line_segment(a_A, a_B, best_b);

    // now that we have found the best candidate points, perform a sphere intersection test between those points with the capluse radii
    
    return true;
}


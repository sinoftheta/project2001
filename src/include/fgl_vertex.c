#include "fgl_vertex.h"
#include <stdio.h>

void fgl_vec3_print(fgl_vec3_t inVertex) {
	printf("(%+.4f, %+.4f, %+.4f)\n" // {, (%.4f, %.4f), (%.2f, %.2f, %.2f) }
	, fix16_to_dbl(inVertex.x), fix16_to_dbl(inVertex.y), fix16_to_dbl(inVertex.z)
	);
}

fix16_t fix16_add3(fix16_t a, fix16_t b, fix16_t c){ return fix16_add(fix16_add(a,b),c); }

fix16_t fgl_vec3_magnitude(fgl_vec3_t in){
	return fix16_sqrt(fix16_add3(fix16_sq(in.x), fix16_sq(in.y), fix16_sq(in.z)));
}

fgl_vec3_t fgl_vec3_scale(fgl_vec3_t in, fix16_t s){ return (fgl_vec3_t){fix16_mul(in.x,s),fix16_mul(in.y,s),fix16_mul(in.z,s)}; }

// TODO: make all these static inline?
fgl_vec3_t fgl_vec3_add(fgl_vec3_t a, fgl_vec3_t b) {
	return (fgl_vec3_t){
		fix16_add(a.x,b.x),
		fix16_add(a.y,b.y),
		fix16_add(a.z,b.z)
	};
}

fgl_vec3_t fgl_vec3_sub(fgl_vec3_t a, fgl_vec3_t b) {
	return (fgl_vec3_t){
		fix16_sub(a.x,b.x),
		fix16_sub(a.y,b.y),
		fix16_sub(a.z,b.z)
	};
}

fgl_vec3_t fgl_vec3_normalize(fgl_vec3_t in) {

	fix16_t m = fgl_vec3_magnitude(in);
	
	return (fgl_vec3_t){
		fix16_div(in.x, m),
		fix16_div(in.y, m),
		fix16_div(in.z, m)
	};
}

fix16_t fgl_vec3_dot(fgl_vec3_t a, fgl_vec3_t b) {
	return fix16_add3(
		fix16_mul(a.x, b.x),
		fix16_mul(a.y, b.y),
		fix16_mul(a.z, b.z)
	);
}

fgl_vec3_t fgl_vec3_cross(fgl_vec3_t a, fgl_vec3_t b){
 	return (fgl_vec3_t){
		fix16_sub(fix16_mul(a.y,b.z),fix16_mul(a.z,b.y)),
		fix16_sub(fix16_mul(a.z,b.x),fix16_mul(a.x,b.z)),
		fix16_sub(fix16_mul(a.x,b.y),fix16_mul(a.y,b.x))
	};
}
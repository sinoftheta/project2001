#include "fgl_vertex.h"
#include <stdio.h>

void fgl_vec3_print(fgl_vec3_t inVertex) {
	printf("(%+.4f, %+.4f, %+.4f)\n" // {, (%.4f, %.4f), (%.2f, %.2f, %.2f) }
	, fix16_to_dbl(inVertex.x), fix16_to_dbl(inVertex.y), fix16_to_dbl(inVertex.z)
	);
}


// make static inline ?
fgl_vec3_t fgl_vec3_add(fgl_vec3_t a, fgl_vec3_t b) {
	fgl_vec3_t out = (fgl_vec3_t){
		fix16_add(a.x,b.x),
		fix16_add(a.y,b.y),
		fix16_add(a.z,b.z)
	};
	return out;
}

fgl_vec3_t fgl_vec3_sub(fgl_vec3_t a, fgl_vec3_t b) {
	fgl_vec3_t out = (fgl_vec3_t){
		fix16_sub(a.x,b.x),
		fix16_sub(a.y,b.y),
		fix16_sub(a.z,b.z)
	};
	return out;
}

fgl_vec3_t fgl_vec3_normalize(fgl_vec3_t in) {
	fgl_vec3_t out = in;

	return out;
};

fix16_t fgl_vec3_dot(fgl_vec3_t a, fgl_vec3_t b) {
	fix16_t out = a.x;

	return out;
};
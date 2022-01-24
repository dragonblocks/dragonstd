#ifndef _DRAGONSTD_NUMBER_H_
#define _DRAGONSTD_NUMBER_H_

#include <stdint.h>
#include <stdbool.h>

#define DEFINE_CLAMP(type) \
	type type ## _clamp(type v, type min, type max);

#define DEFINE_MIX(scalar, type) \
	type type ## _mix(type a, type b, scalar f);

#define DEFINE_VECTOR(scalar, vector, box) vector; \
	typedef struct {vector min; vector max;} box; \
	bool   vector ## _equals(vector a, vector b); \
	vector vector ## _add(vector a, vector b); \
	vector vector ## _clamp(vector v, vector min, vector max); \
	DEFINE_CLAMP(vector)

#define DEFINE_VECTORS(scalar) \
	typedef struct {scalar x, y      ;} DEFINE_VECTOR(scalar, v2 ## scalar, aabb2 ## scalar) \
	typedef struct {scalar x, y, z   ;} DEFINE_VECTOR(scalar, v3 ## scalar, aabb3 ## scalar) \
	typedef struct {scalar x, y, z, w;} DEFINE_VECTOR(scalar, v4 ## scalar, aabb4 ## scalar)

#define DEFINE_SCALAR(scalar, origin) \
	typedef origin scalar; \
	scalar scalar ## _max(scalar a, scalar b); \
	scalar scalar ## _min(scalar a, scalar b); \
	DEFINE_CLAMP(scalar) \
	DEFINE_VECTORS(scalar)

#define DEFINE_INTEGER(bits) \
	DEFINE_SCALAR(s ## bits,  int ## bits ## _t) \
	DEFINE_SCALAR(u ## bits, uint ## bits ## _t)

#define DEFINE_FLOAT(type, origin) \
	DEFINE_SCALAR(type, origin) \
	DEFINE_MIX(type, type) \
	DEFINE_MIX(type, v2 ## type) \
	DEFINE_MIX(type, v3 ## type) \
	DEFINE_MIX(type, v4 ## type)

DEFINE_INTEGER(8)
DEFINE_INTEGER(16)
DEFINE_INTEGER(32)
DEFINE_INTEGER(64)

DEFINE_FLOAT(f32, float)
DEFINE_FLOAT(f64, double)

#undef DEFINE_CLAMP
#undef DEFINE_MIX
#undef DEFINE_VECTOR
#undef DEFINE_VECTORS
#undef DEFINE_SCALAR
#undef DEFINE_INTEGER
#undef DEFINE_FLOAT

#endif

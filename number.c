#include "number.h"

#define DEFINE_CLAMP(scalar) \
	scalar scalar ## _clamp(scalar v, scalar min, scalar max) \
	{ \
		return v < min ? min : v > max ? max : v; \
	}

#define DEFINE_VECTOR2(vector, clamp) \
	bool vector ## _equals(vector a, vector b) \
	{ \
		return a.x == b.x && a.y == b.y; \
	} \
	vector vector ## _add(vector a, vector b) \
	{ \
		return (vector) {a.x + b.x, a.y + b.y}; \
	} \
	vector vector ## _clamp(vector v, vector min, vector max) \
	{ \
		return (vector) {clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y)}; \
	}

#define DEFINE_VECTOR3(vector, clamp) \
	bool vector ## _equals(vector a, vector b) \
	{ \
		return a.x == b.x && a.y == b.y && a.z == b.z; \
	} \
	vector vector ## _add(vector a, vector b) \
	{ \
		return (vector) {a.x + b.x, a.y + b.y, a.z + b.z}; \
	} \
	vector vector ## _clamp(vector v, vector min, vector max) \
	{ \
		return (vector) {clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y), clamp(v.z, min.z, max.z)}; \
	}

#define DEFINE_VECTOR4(vector, clamp) \
	bool vector ## _equals(vector a, vector b) \
	{ \
		return a.x == b.x && a.y == b.y && a.z == b.z  && a.w == b.w; \
	} \
	vector vector ## _add(vector a, vector b) \
	{ \
		return (vector) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w}; \
	} \
	vector vector ## _clamp(vector v, vector min, vector max) \
	{ \
		return (vector) {clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y), clamp(v.z, min.z, max.z), clamp(v.w, min.w, max.w)}; \
	}

#define DEFINE_SCALAR(scalar) \
	DEFINE_CLAMP(scalar) \
	DEFINE_VECTOR2(v2 ## scalar, scalar ## _clamp) \
	DEFINE_VECTOR3(v3 ## scalar, scalar ## _clamp) \
	DEFINE_VECTOR4(v4 ## scalar, scalar ## _clamp)

#define DEFINE_INTEGER(bits) \
	DEFINE_SCALAR(s ## bits) \
	DEFINE_SCALAR(u ## bits)

#define DEFINE_MIX_VECTOR2(vector, scalar, mix) \
	vector vector ## _mix(vector a, vector b, scalar f) \
	{ \
		return (vector) {mix(a.x, b.x, f), mix(a.y, b.y, f)}; \
	}

#define DEFINE_MIX_VECTOR3(vector, scalar, mix) \
	vector vector ## _mix(vector a, vector b, scalar f) \
	{ \
		return (vector) {mix(a.x, b.x, f), mix(a.y, b.y, f), mix(a.z, b.z, f)}; \
	}

#define DEFINE_MIX_VECTOR4(vector, scalar, mix) \
	vector vector ## _mix(vector a, vector b, scalar f) \
	{ \
		return (vector) {mix(a.x, b.x, f), mix(a.y, b.y, f), mix(a.z, b.z, f), mix(a.w, b.w, f)}; \
	}

#define DEFINE_FLOAT(type) \
	DEFINE_SCALAR(type) \
	type type ## _mix(type a, type b, type f) \
	{ \
		return (1.0 - f) * a + b * f; \
	} \
	DEFINE_MIX_VECTOR2(v2 ## type, type, type ## _mix) \
	DEFINE_MIX_VECTOR3(v3 ## type, type, type ## _mix) \
	DEFINE_MIX_VECTOR4(v4 ## type, type, type ## _mix)

DEFINE_INTEGER(8)
DEFINE_INTEGER(16)
DEFINE_INTEGER(32)
DEFINE_INTEGER(64)

DEFINE_FLOAT(f32)
DEFINE_FLOAT(f64)

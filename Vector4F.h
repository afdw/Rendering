#ifndef VECTOR_4F
#define VECTOR_4F

#include "VectorTypes.h"

typedef struct Vector3F Vector3F;

typedef struct Vector4F {
    F x;
    F y;
    F z;
    F w;
} Vector4F;

Vector4F vector4FNew(F x, F y, F z, F w);

Vector4F vector4FAdd(Vector4F a, Vector4F b);

Vector4F vector4FSubstract(Vector4F a, Vector4F b);

Vector3F vector4FToVector3F(Vector4F vector);

#endif

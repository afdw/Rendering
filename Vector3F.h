#ifndef VECTOR_3F_H
#define VECTOR_3F_H

#include "VectorTypes.h"

typedef struct Vector2I Vector2I;
typedef struct Vector4F Vector4F;

typedef struct Vector3F {
    F x;
    F y;
    F z;
} Vector3F;

Vector3F vector3FNew(F x, F y, F z);

Vector3F vector3FAdd(Vector3F a, Vector3F b);

Vector3F vector3FSubstract(Vector3F a, Vector3F b);

F vector3FLength(Vector3F vector);

Vector3F vector3FNormalize(Vector3F vector);

F vector3FDotProduct(Vector3F a, Vector3F b);

Vector3F vector3FCrossProduct(Vector3F a, Vector3F b);

Vector4F vector3FToVector4F(Vector3F vector, F w);

Vector2I vector3FToVector2I(Vector3F vector);

#endif

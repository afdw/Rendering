#ifndef MATRIX_3F_H
#define MATRIX_3F_H

#include "VectorTypes.h"
#include "Vector3F.h"

typedef struct Matrix3F {
    F m00, m01, m02;
    F m10, m11, m12;
    F m20, m21, m22;
} Matrix3F;

Matrix3F matrix3FNew(F m00, F m01, F m02,
                     F m10, F m11, F m12,
                     F m20, F m21, F m22);

Matrix3F matrix3FMultiply(Matrix3F a, Matrix3F b);

Vector3F matrix3FMultiplyByVector3F(Matrix3F m, Vector3F v);

#endif

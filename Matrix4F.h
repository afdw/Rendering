#ifndef MATRIX_4F_H
#define MATRIX_4F_H

#include "VectorTypes.h"
#include "Vector3F.h"
#include "Vector4F.h"

typedef struct Matrix3F Matrix3F;

typedef struct Matrix4F {
    F m00, m01, m02, m03;
    F m10, m11, m12, m13;
    F m20, m21, m22, m23;
    F m30, m31, m32, m33;
} Matrix4F;

Matrix4F matrix4FNew(F m00, F m01, F m02, F m03,
                     F m10, F m11, F m12, F m13,
                     F m20, F m21, F m22, F m23,
                     F m30, F m31, F m32, F m33);

Matrix4F matrix4FNewIdentity();

Matrix4F matrix4FNewScaling(F x, F y, F z);

Matrix4F matrix4FNewTranslation(Vector3F v);

Matrix4F matrix4FNewRotation(F angle, F x, F y, F z);

Matrix4F matrix4FNewOrthographic(F left, F right, F bottom, F top, F near, F far);

Matrix4F matrix4FNewFrustum(F left, F right, F bottom, F top, F near, F far);

Matrix4F matrix4FNewPerspective(F fieldOfView, F aspectRatio, F near, F far);

Matrix4F matrix4FMultiply(Matrix4F a, Matrix4F b);

Vector4F matrix4FMultiplyByVector4F(Matrix4F m, Vector4F v);

Matrix3F matrix4FToMatrix3F(Matrix4F matrix);

#endif

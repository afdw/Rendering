#include "Matrix3F.h"

#include <math.h>

Matrix3F matrix3FNew(F m00, F m01, F m02,
                     F m10, F m11, F m12,
                     F m20, F m21, F m22) {
    Matrix3F matrix = {
        .m00 = m00, .m01 = m01, .m02 = m02,
        .m10 = m10, .m11 = m11, .m12 = m12,
        .m20 = m20, .m21 = m21, .m22 = m22
    };
    return matrix;
}

Matrix3F matrix3FMultiply(Matrix3F a, Matrix3F b) {
    return matrix3FNew(
        a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20, a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21, a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22,
        a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20, a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21, a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22,
        a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20, a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21, a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22
    );
}

Vector3F matrix3FMultiplyByVector3F(Matrix3F m, Vector3F v) {
    return vector3FNew(
        m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
        m.m10 * v.x + m.m11 * v.y + m.m12 * v.z,
        m.m20 * v.x + m.m21 * v.y + m.m22 * v.z
    );
}

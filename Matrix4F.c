#include "Matrix4F.h"

#include <math.h>

#include "Utils.h"

Matrix4F matrix4FNew(F m00, F m01, F m02, F m03,
                     F m10, F m11, F m12, F m13,
                     F m20, F m21, F m22, F m23,
                     F m30, F m31, F m32, F m33) {
    Matrix4F matrix = {
        .m00 = m00, .m01 = m01, .m02 = m02, .m03 = m03,
        .m10 = m10, .m11 = m11, .m12 = m12, .m13 = m13,
        .m20 = m20, .m21 = m21, .m22 = m22, .m23 = m23,
        .m30 = m30, .m31 = m31, .m32 = m32, .m33 = m33
    };
    return matrix;
}

Matrix4F matrix4FNewIdentity() {
    return matrix4FNew(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

Matrix4F matrix4FNewScaling(F x, F y, F z) {
    return matrix4FNew(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    );
}

Matrix4F matrix4FNewTranslation(Vector3F v) {
    return matrix4FNew(
        1, 0, 0, v.x,
        0, 1, 0, v.y,
        0, 0, 1, v.z,
        0, 0, 0, 1
    );
}

Matrix4F matrix4FNewRotation(F angle, F x, F y, F z) {
    return matrix4FNew(
        x * x * (1 - cos(angle)) + cos(angle) /*    */, x * y * (1 - cos(angle)) - z * sin(angle) /**/, x * z * (1 - cos(angle)) + y * sin(angle) /**/, 0,
        y * x * (1 - cos(angle)) + z * sin(angle) /**/, y * y * (1 - cos(angle)) + cos(angle) /*    */, y * z * (1 - cos(angle)) - x * sin(angle) /**/, 0,
        x * z * (1 - cos(angle)) - y * sin(angle) /**/, y * z * (1 - cos(angle)) + x * sin(angle) /**/, z * z * (1 - cos(angle)) + cos(angle) /*    */, 0,
        0 /*                                        */, 0 /*                                        */, 0 /*                                        */, 1
    );
}

Matrix4F matrix4FNewOrthographic(F left, F right, F bottom, F top, F near, F far) {
    return matrix4FNew(
        2 / (right - left), 0 /*            */, 0 /*           */, -(right + left) / (right - left),
        0 /*            */, 2 / (top - bottom), 0 /*           */, -(top + bottom) / (top - bottom),
        0 /*            */, 0 /*            */, -2 / (far - near), -(far + near) / (far - near),
        0 /*            */, 0 /*            */, 0 /*           */, 1
    );
}

Matrix4F matrix4FNewFrustum(F left, F right, F bottom, F top, F near, F far) {
    return matrix4FNew(
        2 * near / (right - left), 0 /*                   */, (right + left) / (right - left) /**/, 0,
        0 /*                   */, 2 * near / (top - bottom), (top + bottom) / (top - bottom) /**/, 0,
        0 /*                   */, 0 /*                   */, -(far + near) / (far - near) /*   */, -(2 * far * near) / (far - near),
        0 /*                   */, 0 /*                   */, -1 /*                             */, 0
    );
}

Matrix4F matrix4FNewPerspective(F fieldOfView, F aspectRatio, F near, F far) {
    F height = near * tan(atan(tan(toRadians(fieldOfView) * 0.5F) / aspectRatio));
    F width = height * aspectRatio;
    return matrix4FNewFrustum(-width, width, -height, height, near, far);
}

Matrix4F matrix4FMultiply(Matrix4F a, Matrix4F b) {
    return matrix4FNew(
        a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30, a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31, a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32, a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33,
        a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30, a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31, a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32, a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33,
        a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30, a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31, a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32, a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33,
        a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30, a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31, a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32, a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33
    );
}

Vector4F matrix4FMultiplyByVector4F(Matrix4F m, Vector4F v) {
    return vector4FNew(
        m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w,
        m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w,
        m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w,
        m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w
    );
}

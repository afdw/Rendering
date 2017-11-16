#include "Vector3F.h"

#include <math.h>

#include "Vector2I.h"
#include "Vector4F.h"

Vector3F vector3FNew(F x, F y, F z) {
    Vector3F vector = {.x = x, .y = y, .z = z};
    return vector;
}

Vector3F vector3FAdd(Vector3F a, Vector3F b) {
    return vector3FNew(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3F vector3FSubtract(Vector3F a, Vector3F b) {
    return vector3FNew(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3F vector3FMultiply(Vector3F vector, F n) {
    return vector3FNew(vector.x * n, vector.y * n, vector.z * n);
}

Vector3F vector3FOpposite(Vector3F vector) {
    return vector3FNew(-vector.x, -vector.y, -vector.z);
}

F vector3FLength(Vector3F vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

Vector3F vector3FNormalize(Vector3F vector) {
    F length = vector3FLength(vector);
    return vector3FNew(vector.x / length, vector.y / length, vector.z / length);
}

F vector3FDotProduct(Vector3F a, Vector3F b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3F vector3FCrossProduct(Vector3F a, Vector3F b) {
    return vector3FNew(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Vector4F vector3FToVector4F(Vector3F vector, F w) {
    return vector4FNew(vector.x, vector.y, vector.z, w);
}

Vector2I vector3FToVector2I(Vector3F vector) {
    return vector2INew(vector.x / vector.z, vector.y / vector.z);
}

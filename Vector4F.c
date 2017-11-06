#include "Vector4F.h"

#include "Vector3F.h"

Vector4F vector4FNew(F x, F y, F z, F w) {
    Vector4F vector = {.x = x, .y = y, .z = z, .w = w};
    return vector;
}

Vector4F vector4FAdd(Vector4F a, Vector4F b) {
    return vector4FNew(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vector4F vector4FSubstract(Vector4F a, Vector4F b) {
    return vector4FNew(a.x - b.x, a.y - b.y, a.z - b.z, a.w + b.w);
}

Vector3F vector4FToVector3F(Vector4F vector) {
    return vector3FNew(vector.x / vector.w, vector.y / vector.w, vector.z / vector.w);
}

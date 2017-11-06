#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef NO_RENDERING
#include <GLFW/glfw3.h>
#endif

#include "System.h"

#define structMember(type, member) (((type *) 0)->member)
#define getArrayElementsCount(array) (sizeof(array) / sizeof((array)[0]))

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define sign(n) ((n) == 0 ? 0 : ((n) > 0 ? 1 : -1))
#define abs(n) ((n) >= 0 ? (n) : (-n))
#define clamp(n, min, max) ((n) < (min) ? (min) : (n) > (max) ? (max) : (n))
#define toRadians(degrees) ((degrees) * M_PI / 180.0)
#define toDegrees(radians) ((radians) * 180.0 / M_PI)

typedef float ColorComponent;
typedef int64_t I;
typedef double F;

typedef struct {
    I x;
    I y;
} Vector2I;

Vector2I vector2INew(I x, I y) {
    Vector2I vector = {.x = x, .y = y};
    return vector;
}

Vector2I vector2IAdd(Vector2I a, Vector2I b) {
    return vector2INew(a.x + b.x, a.y + b.y);
}

Vector2I vector2ISubstract(Vector2I a, Vector2I b) {
    return vector2INew(a.x - b.x, a.y - b.y);
}

typedef struct {
    F x;
    F y;
    F z;
} Vector3F;

Vector3F vector3FNew(F x, F y, F z) {
    Vector3F vector = {.x = x, .y = y, .z = z};
    return vector;
}

Vector3F vector3FAdd(Vector3F a, Vector3F b) {
    return vector3FNew(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3F vector3FSubstract(Vector3F a, Vector3F b) {
    return vector3FNew(a.x - b.x, a.y - b.y, a.z - b.z);
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

typedef struct {
    F x;
    F y;
    F z;
    F w;
} Vector4F;

Vector4F vector4FNew(F x, F y, F z, F w) {
    Vector4F vector = {.x = x, .y = y, .z = z, .w = w};
    return vector;
}

Vector4F vector4DAdd(Vector4F a, Vector4F b) {
    return vector4FNew(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vector4F vector4DSubstract(Vector4F a, Vector4F b) {
    return vector4FNew(a.x - b.x, a.y - b.y, a.z - b.z, a.w + b.w);
}

Vector4F vector3FToVector4F(Vector3F vector, F w) {
    return vector4FNew(vector.x, vector.y, vector.z, w);
}

Vector2I vector3FToVector2I(Vector3F vector) {
    return vector2INew(vector.x / vector.z, vector.y / vector.z);
}

Vector3F vector4FToVector3F(Vector4F vector) {
    return vector3FNew(vector.x / vector.w, vector.y / vector.w, vector.z / vector.w);
}

typedef struct {
    F m00, m01, m02, m03;
    F m10, m11, m12, m13;
    F m20, m21, m22, m23;
    F m30, m31, m32, m33;
} Matrix4F;

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
        pow(x, 2) * (1 - cos(angle)) + cos(angle), x * y * (1 - cos(angle)) - z * sin(angle), x * z * (1 - cos(angle)) + y * sin(angle), 0,
        y * x * (1 - cos(angle)) + z * sin(angle), pow(y, 2) * (1 - cos(angle)) + cos(angle), y * z * (1 - cos(angle)) - x * sin(angle), 0,
        x * z * (1 - cos(angle)) - y * sin(angle), y * z * (1 - cos(angle)) + x * sin(angle), pow(z, 2) * (1 - cos(angle)) + cos(angle), 0,
        0 /*                                   */, 0 /*                                   */, 0 /*                                   */, 1
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

typedef struct {
    ColorComponent r;
    ColorComponent g;
    ColorComponent b;
    ColorComponent a;
} Color;

Color colorNew(ColorComponent r, ColorComponent g, ColorComponent b, ColorComponent a) {
    Color color = {.r = r, .g = g, .b = b, .a = a};
    return color;
}

typedef struct {
    Vector2I size;
    ColorComponent *colorBuffer;
    F *depthBuffer;
} Rasterizer;

#ifndef NO_RENDERING
Vector2I rasterizerGetWindowSize(GLFWwindow* window) {
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);
    return vector2INew((I) width, (I) height);
}
#endif

size_t rasterizerGetBufferSize(Vector2I size, size_t componentSize, size_t componentCount) {
    return componentSize * componentCount * (size_t) size.x * (size_t) size.y;
}

size_t rasterizerGetBufferOffset(Vector2I size, size_t componentSize, size_t componentCount, Vector2I position) {
    return componentSize * componentCount * ((size_t) size.x * (size_t) position.y + (size_t) position.x);
}

Rasterizer *rasterizerNew(Vector2I size) {
    Rasterizer *rasterizer = malloc(sizeof(Rasterizer));
    rasterizer->size = size;
    rasterizer->colorBuffer = calloc(rasterizerGetBufferSize(rasterizer->size, sizeof(ColorComponent), 4), 1);
    rasterizer->depthBuffer = calloc(rasterizerGetBufferSize(rasterizer->size, sizeof(F), 1), 1);
    return rasterizer;
}

void rasterizerSetSize(Rasterizer *rasterizer, Vector2I size) {
    if (size.x != rasterizer->size.x || size.y != rasterizer->size.y) {
        rasterizer->size = size;
        free(rasterizer->colorBuffer);
        free(rasterizer->depthBuffer);
        rasterizer->colorBuffer = calloc(rasterizerGetBufferSize(rasterizer->size, sizeof(ColorComponent), 4), 1);
        rasterizer->depthBuffer = calloc(rasterizerGetBufferSize(rasterizer->size, sizeof(F), 1), 1);
    }
}

void rasterizerClear(Rasterizer *rasterizer) {
    memset(rasterizer->colorBuffer, 0, rasterizerGetBufferSize(rasterizer->size, sizeof(ColorComponent), 4));
    memset(rasterizer->depthBuffer, 0, rasterizerGetBufferSize(rasterizer->size, sizeof(F), 1));
}

void rasterizerSetColor(Rasterizer *rasterizer, Vector2I position, Color color) {
    if (position.x >= 0 && position.y >= 0 && position.x < rasterizer->size.x && position.y < rasterizer->size.y) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->size, sizeof(ColorComponent), 4, position);
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset) = color.r;
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent)) = color.g;
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent) * 2) = color.b;
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent) * 3) = color.a;
    }
}

void rasterizerSetDepth(Rasterizer *rasterizer, Vector2I position, F depth) {
    if (position.x >= 0 && position.y >= 0 && position.x < rasterizer->size.x && position.y < rasterizer->size.y) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->size, sizeof(F), 1, position);
        *(F *)((char *) rasterizer->depthBuffer + offset) = depth;
    }
}

Color rasterizerGetColor(Rasterizer *rasterizer, Vector2I position) {
    if (position.x >= 0 && position.y >= 0 && position.x < rasterizer->size.x && position.y < rasterizer->size.y) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->size, sizeof(ColorComponent), 4, position);
        return colorNew(
            *(ColorComponent *)((char *) rasterizer->colorBuffer + offset),
            *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent)),
            *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent) * 2),
            *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent) * 3)
        );
    }
    return colorNew(0, 0, 0, 0);
}

F rasterizerGetDepth(Rasterizer *rasterizer, Vector2I position) {
    if (position.x >= 0 && position.y >= 0 && position.x < rasterizer->size.x && position.y < rasterizer->size.y) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->size, sizeof(F), 1, position);
        return *(F *)((char *) rasterizer->depthBuffer + offset);
    }
    return 0;
}

Vector2I rasterizerNDCToScreen(Rasterizer *rasterizer, Vector3F vector) {
    return vector2INew((I) ((vector.x + 1) / 2 * rasterizer->size.x), (I) ((vector.y + 1) / 2 * rasterizer->size.y));
}

Vector3F rasterizerScreenToNDC(Rasterizer *rasterizer, Vector2I vector) {
    return vector3FNew((F) vector.x / rasterizer->size.x * 2 - 1, (F) vector.y / rasterizer->size.y * 2 - 1, 0);
}

void rasterizerDrawLine(Rasterizer *rasterizer, Vector3F p0, Vector3F p1, Color color) {
    Vector2I p0Screen = rasterizerNDCToScreen(rasterizer, p0);
    Vector2I p1Screen = rasterizerNDCToScreen(rasterizer, p1);
    Vector2I delta = vector2ISubstract(p1Screen, p0Screen);
    Vector2I absDelta = vector2INew(abs(delta.x), abs(delta.y));
    int error = 0;
    if (absDelta.x >= absDelta.y) {
        for (int x = p0Screen.x, y = p0Screen.y; delta.x >= 0 ? (x <= p1Screen.x) : (x >= p1Screen.x); x += sign(delta.x)) {
            rasterizerSetColor(rasterizer, vector2INew(x, y), color);
            error += absDelta.y;
            if (error * 2 >= absDelta.x) {
                y += sign(delta.y);
                error -= absDelta.x;
            }
            if (delta.x == 0) {
                break;
            }
        }
    } else {
        for (int x = p0Screen.x, y = p0Screen.y; delta.y >= 0 ? (y <= p1Screen.y) : (y >= p1Screen.y); y += sign(delta.y)) {
            rasterizerSetColor(rasterizer, vector2INew(x, y), color);
            error += absDelta.x;
            if (error * 2 >= absDelta.y) {
                x += sign(delta.x);
                error -= absDelta.y;
            }
            if (delta.y == 0) {
                break;
            }
        }
    }
}

void rasterizerDrawTriangleWireframe(Rasterizer *rasterizer, Vector3F a, Vector3F b, Vector3F c, Color color) {
    rasterizerDrawLine(rasterizer, a, b, color);
    rasterizerDrawLine(rasterizer, b, c, color);
    rasterizerDrawLine(rasterizer, a, c, color);
}

void rasterizerDrawTriangle(Rasterizer *rasterizer, Vector3F a, Vector3F b, Vector3F c, Color color) {
    Vector2I aScreen = rasterizerNDCToScreen(rasterizer, a);
    Vector2I bScreen = rasterizerNDCToScreen(rasterizer, b);
    Vector2I cScreen = rasterizerNDCToScreen(rasterizer, c);
    Vector2I minVertex = vector2INew(min(min(aScreen.x, bScreen.x), cScreen.x), min(min(aScreen.y, bScreen.y), cScreen.y));
    Vector2I maxVertex = vector2INew(max(max(aScreen.x, bScreen.x), cScreen.x), max(max(aScreen.y, bScreen.y), cScreen.y));
    for (I y = minVertex.y; y <= maxVertex.y; y++) {
        for (I x = minVertex.x; x <= maxVertex.x; x++) {
            Vector2I pScreen = vector2INew(x, y);
            Vector3F p = rasterizerScreenToNDC(rasterizer, pScreen);
            F m = ((b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y)) / ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y));
            if (m >= 0 && m <= 1) {
                F l = (b.x != a.x) ? (((p.x - a.x) - m * (c.x - a.x)) / (b.x - a.x)) : (((p.y - a.y) - m * (c.y - a.y)) / (b.y - a.y));
                if (l >= 0 && l <= 1 && m + l <= 1) {
                    F depth = a.z + l * (b.z - a.z) + m * (c.z - a.z);
                    F oldDepth = rasterizerGetDepth(rasterizer, pScreen);
                    if (oldDepth == 0 || depth < oldDepth) {
                        rasterizerSetColor(rasterizer, pScreen, color);
                        // rasterizerSetColor(rasterizer, pScreen, colorNew((1 + depth) / 2, (1 + depth) / 2, (1 + depth) / 2, 1));
                        rasterizerSetDepth(rasterizer, pScreen, depth);
                    }
                }
            }
        }
    }
}

#ifndef NO_RENDERING
void rasterizerDraw(Rasterizer *rasterizer) {
    glDrawPixels(rasterizer->size.x, rasterizer->size.y, GL_RGBA, GL_FLOAT, rasterizer->colorBuffer);
}
#endif

void rasterizerDelete(Rasterizer *rasterizer) {
    free(rasterizer->colorBuffer);
    free(rasterizer->depthBuffer);
    free(rasterizer);
}

#include <pthread.h>
void *threadFunction(void *arg) {
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, &threadFunction, NULL);
    //pthread_join(tid, NULL);
    Vector3F triangles[2 * 2 * 3][3] = {
        {vector3FNew(-0.5, -0.5, -0.5), vector3FNew(-0.5, -0.5, +0.5), vector3FNew(-0.5, +0.5, +0.5)},
        {vector3FNew(-0.5, +0.5, +0.5), vector3FNew(-0.5, +0.5, -0.5), vector3FNew(-0.5, -0.5, -0.5)},
        {vector3FNew(+0.5, +0.5, +0.5), vector3FNew(+0.5, -0.5, +0.5), vector3FNew(+0.5, -0.5, -0.5)},
        {vector3FNew(+0.5, -0.5, -0.5), vector3FNew(+0.5, +0.5, -0.5), vector3FNew(+0.5, +0.5, +0.5)},

        {vector3FNew(-0.5, -0.5, -0.5), vector3FNew(-0.5, -0.5, +0.5), vector3FNew(+0.5, -0.5, +0.5)},
        {vector3FNew(+0.5, -0.5, +0.5), vector3FNew(+0.5, -0.5, -0.5), vector3FNew(-0.5, -0.5, -0.5)},
        {vector3FNew(+0.5, +0.5, +0.5), vector3FNew(-0.5, +0.5, +0.5), vector3FNew(-0.5, +0.5, -0.5)},
        {vector3FNew(-0.5, +0.5, -0.5), vector3FNew(+0.5, +0.5, -0.5), vector3FNew(+0.5, +0.5, +0.5)},

        {vector3FNew(-0.5, -0.5, -0.5), vector3FNew(-0.5, +0.5, -0.5), vector3FNew(+0.5, +0.5, -0.5)},
        {vector3FNew(+0.5, +0.5, -0.5), vector3FNew(+0.5, -0.5, -0.5), vector3FNew(-0.5, -0.5, -0.5)},
        {vector3FNew(+0.5, +0.5, +0.5), vector3FNew(-0.5, +0.5, +0.5), vector3FNew(-0.5, -0.5, +0.5)},
        {vector3FNew(-0.5, -0.5, +0.5), vector3FNew(+0.5, -0.5, +0.5), vector3FNew(+0.5, +0.5, +0.5)}
    };
    Color colors[2 * 2 * 3] = {
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),

        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),

        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1),
        colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1)
    };
    Vector3F lightDirection = vector3FNormalize(vector3FNew(0, -0.5, 1));
    Rasterizer *rasterizer = rasterizerNew(vector2INew(1000, 1000));
#ifndef NO_RENDERING
    if (!glfwInit()) {
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(640, 480, "Rendering", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        rasterizerSetSize(rasterizer, rasterizerGetWindowSize(window));
#else
    while (1) {
#endif
        rasterizerClear(rasterizer);
        double time = (double) (systemGetCurrentMilliseconds() % 3000) / 3000;
        double angle = time * M_PI * 2;
        Matrix4F projectionMatrix = matrix4FNewPerspective(85, (F) rasterizer->size.x / rasterizer->size.y, 1, 100);
        Matrix4F modelViewMatrix = matrix4FNewIdentity();
        modelViewMatrix = matrix4FMultiply(modelViewMatrix, matrix4FNewTranslation(vector3FNew(0, sin(angle) / 2, -2)));
        modelViewMatrix = matrix4FMultiply(modelViewMatrix, matrix4FNewRotation(angle, 0, 1, 0));
        modelViewMatrix = matrix4FMultiply(modelViewMatrix, matrix4FNewScaling(0.5, 0.5, 0.5));
        Matrix4F modelViewProjectionMatrix = matrix4FMultiply(projectionMatrix, modelViewMatrix);
        for (size_t i = 0; i < getArrayElementsCount(triangles); i++) {
            Vector3F a = vector4FToVector3F(matrix4FMultiplyByVector4F(modelViewProjectionMatrix, vector3FToVector4F(triangles[i][0], 1)));
            Vector3F b = vector4FToVector3F(matrix4FMultiplyByVector4F(modelViewProjectionMatrix, vector3FToVector4F(triangles[i][1], 1)));
            Vector3F c = vector4FToVector3F(matrix4FMultiplyByVector4F(modelViewProjectionMatrix, vector3FToVector4F(triangles[i][2], 1)));
            Vector3F normal = vector3FNormalize(vector3FCrossProduct(vector3FSubstract(b, a), vector3FSubstract(c, a)));
            Vector3F lightDirectionTransformed = vector3FNormalize(vector4FToVector3F(matrix4FMultiplyByVector4F(projectionMatrix, vector3FToVector4F(lightDirection, 0))));
            ColorComponent illumination = 0.1 + clamp((ColorComponent) vector3FDotProduct(normal, lightDirectionTransformed), 0, 1) * 0.9;
            Color color = colorNew(
                colors[i].r * illumination,
                colors[i].g * illumination,
                colors[i].b * illumination,
                colors[i].a
            );
            rasterizerDrawTriangle(rasterizer, a, b, c, color);
        }
#ifndef NO_RENDERING
        rasterizerDraw(rasterizer);
        GLenum error = glGetError();
        if (error != 0) {
            printf("%x\n", glGetError());
            fflush(stdout);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
#else
        break;
    }
#endif
    rasterizerDelete(rasterizer);
    return 0;
}

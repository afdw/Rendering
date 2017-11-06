#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#ifndef NO_RENDERING
#include <GLFW/glfw3.h>
#endif

#include "Utils.h"
#include "VectorTypes.h"
#include "Vector2I.h"
#include "Vector3F.h"
#include "Vector4F.h"
#include "Matrix4F.h"
#include "Color.h"
#include "Rasterizer.h"
#include "System.h"

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

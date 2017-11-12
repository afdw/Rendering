#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Utils.h"
#include "VectorTypes.h"
#include "Vector2I.h"
#include "Vector3F.h"
#include "Vector4F.h"
#include "Matrix4F.h"
#include "Color.h"
#include "Rasterizer.h"
#include "Interface.h"
#include "System.h"

typedef struct RenderData {
    size_t trianglesCount;
    Vector3F *positions;
    Color *colors;
    Vector3F lightDirection;
} RenderData;

void fragmentShader() {

}

void render(Interface *interface, Vector2I size, Rasterizer *rasterizer) {
    RenderData *renderData = (RenderData *) interfaceGetUserPointer(interface);
    rasterizerClear(rasterizer);
    F time = (F) (systemGetCurrentMilliseconds() % 3000) / 3000;
    F angle = time * M_PI * 2;
    Matrix4F projectionMatrix = matrix4FNewPerspective(85, (F) size.x / size.y, 1, 100);
    Matrix4F modelViewMatrix = matrix4FNewIdentity();
    modelViewMatrix = matrix4FMultiply(modelViewMatrix, matrix4FNewTranslation(vector3FNew(0, sin(angle) / 2, -2)));
    modelViewMatrix = matrix4FMultiply(modelViewMatrix, matrix4FNewRotation(angle, 0, 1, 0));
    modelViewMatrix = matrix4FMultiply(modelViewMatrix, matrix4FNewScaling(0.5, 0.5, 0.5));
    Matrix4F modelViewProjectionMatrix = matrix4FMultiply(projectionMatrix, modelViewMatrix);
    for (size_t i = 0; i < renderData->trianglesCount; i++) {
        Vector3F a = vector4FToVector3F(matrix4FMultiplyByVector4F(modelViewProjectionMatrix, vector3FToVector4F(renderData->positions[i * 3], 1)));
        Vector3F b = vector4FToVector3F(matrix4FMultiplyByVector4F(modelViewProjectionMatrix, vector3FToVector4F(renderData->positions[i * 3 + 1], 1)));
        Vector3F c = vector4FToVector3F(matrix4FMultiplyByVector4F(modelViewProjectionMatrix, vector3FToVector4F(renderData->positions[i * 3 + 2], 1)));
        Vector3F normal = vector3FNormalize(vector3FCrossProduct(vector3FSubstract(b, a), vector3FSubstract(c, a)));
        Vector3F lightDirectionTransformed = vector3FNormalize(vector4FToVector3F(matrix4FMultiplyByVector4F(projectionMatrix, vector3FToVector4F(renderData->lightDirection, 0))));
        ColorComponent illumination = 0.1 + clamp((ColorComponent) vector3FDotProduct(normal, lightDirectionTransformed), 0, 1) * 0.9;
        rasterizerDrawTriangle(rasterizer, a, b, c, colorMultiply(renderData->colors[i * 3], illumination), colorMultiply(renderData->colors[i * 3 + 1], illumination), colorMultiply(renderData->colors[i * 3 + 2], illumination));
    }
}

int main(void) {
    int trianglesCount = 12;
    Vector3F positions[12 * 3] = {
        vector3FNew(-0.5, -0.5, -0.5), vector3FNew(-0.5, -0.5, +0.5), vector3FNew(-0.5, +0.5, +0.5),
        vector3FNew(-0.5, +0.5, +0.5), vector3FNew(-0.5, +0.5, -0.5), vector3FNew(-0.5, -0.5, -0.5),
        vector3FNew(+0.5, +0.5, +0.5), vector3FNew(+0.5, -0.5, +0.5), vector3FNew(+0.5, -0.5, -0.5),
        vector3FNew(+0.5, -0.5, -0.5), vector3FNew(+0.5, +0.5, -0.5), vector3FNew(+0.5, +0.5, +0.5),

        vector3FNew(-0.5, -0.5, -0.5), vector3FNew(-0.5, -0.5, +0.5), vector3FNew(+0.5, -0.5, +0.5),
        vector3FNew(+0.5, -0.5, +0.5), vector3FNew(+0.5, -0.5, -0.5), vector3FNew(-0.5, -0.5, -0.5),
        vector3FNew(+0.5, +0.5, +0.5), vector3FNew(-0.5, +0.5, +0.5), vector3FNew(-0.5, +0.5, -0.5),
        vector3FNew(-0.5, +0.5, -0.5), vector3FNew(+0.5, +0.5, -0.5), vector3FNew(+0.5, +0.5, +0.5),

        vector3FNew(-0.5, -0.5, -0.5), vector3FNew(-0.5, +0.5, -0.5), vector3FNew(+0.5, +0.5, -0.5),
        vector3FNew(+0.5, +0.5, -0.5), vector3FNew(+0.5, -0.5, -0.5), vector3FNew(-0.5, -0.5, -0.5),
        vector3FNew(+0.5, +0.5, +0.5), vector3FNew(-0.5, +0.5, +0.5), vector3FNew(-0.5, -0.5, +0.5),
        vector3FNew(-0.5, -0.5, +0.5), vector3FNew(+0.5, -0.5, +0.5), vector3FNew(+0.5, +0.5, +0.5)
    };
    Color colors[12 * 3];
    for (size_t i = 0; i < trianglesCount; i++) {
        colors[i * 3] = colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1);
        colors[i * 3 + 1] = colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1);
        colors[i * 3 + 2] = colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1);
    }
    Vector3F lightDirection = vector3FNormalize(vector3FNew(0, -0.5, 1));
    RenderData *renderData = malloc(sizeof(RenderData));
    renderData->trianglesCount = trianglesCount;
    renderData->positions = positions;
    renderData->colors = colors;
    renderData->lightDirection = lightDirection;
    Interface *interface = interfaceNew();
    interfaceSetUserPointer(interface, renderData);
    interfaceSetRender(interface, &render);
    int result = interfaceMain(interface);
    free(renderData);
    return result;
}

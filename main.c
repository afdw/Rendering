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
#include "Matrix3F.h"
#include "Matrix4F.h"
#include "Color.h"
#include "List.h"
#include "ShaderProgram.h"
#include "RasterizationTask.h"
#include "Rasterizer.h"
#include "Interface.h"
#include "System.h"

typedef struct UniformData {
    Matrix4F modelViewMatrix;
    Matrix4F modelViewProjectionMatrix;
    Vector3F lightDirection;
} UniformData;

typedef struct VertexBuffer {
    Vector3F *positions;
    Color *colors;
    Vector3F *normals;
} VertexBuffer;

typedef struct VertexPixelData {
    Color vertexColor;
} VertexPixelData;

typedef struct UserData {
    size_t primitivesCount;
    PrimitivesType primitivesType;
    VertexBuffer *vertexBuffer;
    Vector3F lightDirection;
    ShaderProgram *shaderProgram;
} UserData;

void vertexShader(void *uniformData,
                  void *vertexBuffer,
                  size_t vertexIndex,
                  Vector3F *outPosition,
                  void *vertexPixelData) {
    *outPosition = vector4FToVector3F(matrix4FMultiplyByVector4F(((UniformData *) uniformData)->modelViewProjectionMatrix, vector3FToVector4F(((VertexBuffer *) vertexBuffer)->positions[vertexIndex], 1)));
    Vector3F normalTransformed = vector3FNormalize(matrix3FMultiplyByVector3F(matrix4FToMatrix3F(((UniformData *) uniformData)->modelViewMatrix), ((VertexBuffer *) vertexBuffer)->normals[vertexIndex]));
    ColorComponent illumination = 0.1 + clamp((ColorComponent) vector3FDotProduct(vector3FOpposite(normalTransformed), ((UniformData *) uniformData)->lightDirection), 0, 1) * 0.9;
    ((VertexPixelData *) vertexPixelData)->vertexColor = colorMultiply(((VertexBuffer *) vertexBuffer)->colors[vertexIndex], illumination);
}

void pixelShader(void *vertexPixelData,
                 Color *outColor) {
    *outColor = ((VertexPixelData *) vertexPixelData)->vertexColor;
}

void vertexPixelDataAdd(void *a, void *b, void *out) {
    ((VertexPixelData *) out)->vertexColor = colorAdd(((VertexPixelData *) a)->vertexColor, ((VertexPixelData *) b)->vertexColor);
}

void vertexPixelDataSubtract(void *a, void *b, void *out) {
    ((VertexPixelData *) out)->vertexColor = colorSubtract(((VertexPixelData *) a)->vertexColor, ((VertexPixelData *) b)->vertexColor);
}

void vertexPixelDataMultiply(void *vertexPixelData, F n, void *out) {
    ((VertexPixelData *) out)->vertexColor = colorMultiply(((VertexPixelData *) vertexPixelData)->vertexColor, n);
}

void render(Interface *interface, Vector2I size, List *rasterizationTasksList) {
    UserData *userData = (UserData *) interfaceGetUserPointer(interface);
    RasterizationTask *clearTask = clearTaskNew();
    listAdd(rasterizationTasksList, &clearTask);
    F time = (F) (systemGetCurrentMilliseconds() % 3000) / 3000;
    F angle = time * M_PI * 2;
    Matrix4F projectionMatrix = matrix4FNewPerspective(85, (F) size.x / size.y, 1, 100);
    Matrix4F modelViewMatrix = matrix4FNewIdentity();
    modelViewMatrix = matrix4FMultiply(modelViewMatrix, matrix4FNewTranslation(vector3FNew(0, sin(angle) / 2, -2)));
    modelViewMatrix = matrix4FMultiply(modelViewMatrix, matrix4FNewRotation(angle, 0, 1, 0));
    modelViewMatrix = matrix4FMultiply(modelViewMatrix, matrix4FNewScaling(0.5, 0.5, 0.5));
    Matrix4F modelViewProjectionMatrix = matrix4FMultiply(projectionMatrix, modelViewMatrix);
    UniformData *uniformData = malloc(sizeof(UniformData));
    uniformData->lightDirection = userData->lightDirection;
    uniformData->modelViewMatrix = modelViewMatrix;
    uniformData->modelViewProjectionMatrix = modelViewProjectionMatrix;
    RasterizationTask *drawTask = drawTaskNew(
        userData->primitivesType,
        userData->primitivesCount,
        userData->vertexBuffer,
        uniformData,
        userData->shaderProgram
    );
    listAdd(rasterizationTasksList, &drawTask);
}

int main(void) {
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
    for (size_t i = 0; i < 12; i++) {
        colors[i * 3] = colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1);
        colors[i * 3 + 1] = colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1);
        colors[i * 3 + 2] = colorNew((F) (rand() % 255) / 255, (F) (rand() % 255) / 255, (F) (rand() % 255) / 255, 1);
    }
    Vector3F normals[12 * 3] = {
        vector3FNew(-1, 0, 0), vector3FNew(-1, 0, 0), vector3FNew(-1, 0, 0),
        vector3FNew(-1, 0, 0), vector3FNew(-1, 0, 0), vector3FNew(-1, 0, 0),
        vector3FNew(+1, 0, 0), vector3FNew(+1, 0, 0), vector3FNew(+1, 0, 0),
        vector3FNew(+1, 0, 0), vector3FNew(+1, 0, 0), vector3FNew(+1, 0, 0),

        vector3FNew(0, -1, 0), vector3FNew(0, -1, 0), vector3FNew(0, -1, 0),
        vector3FNew(0, -1, 0), vector3FNew(0, -1, 0), vector3FNew(0, -1, 0),
        vector3FNew(0, +1, 0), vector3FNew(0, +1, 0), vector3FNew(0, +1, 0),
        vector3FNew(0, +1, 0), vector3FNew(0, +1, 0), vector3FNew(0, +1, 0),

        vector3FNew(0, 0, -1), vector3FNew(0, 0, -1), vector3FNew(0, 0, -1),
        vector3FNew(0, 0, -1), vector3FNew(0, 0, -1), vector3FNew(0, 0, -1),
        vector3FNew(0, 0, +1), vector3FNew(0, 0, +1), vector3FNew(0, 0, +1),
        vector3FNew(0, 0, +1), vector3FNew(0, 0, +1), vector3FNew(0, 0, +1)
    };
    VertexBuffer *vertexBuffer = malloc(sizeof(VertexBuffer));
    vertexBuffer->positions = positions;
    vertexBuffer->colors = colors;
    vertexBuffer->normals = normals;
    ShaderProgram *shaderProgram = shaderProgramNew(
        &vertexShader,
        &pixelShader,
        sizeof(VertexPixelData),
        &vertexPixelDataAdd,
        &vertexPixelDataSubtract,
        &vertexPixelDataMultiply
    );
    UserData *userData = malloc(sizeof(UserData));
    userData->primitivesCount = 12;
    userData->primitivesType = TRIANGLES;
    userData->vertexBuffer = vertexBuffer;
    userData->lightDirection = vector3FNormalize(vector3FNew(0, -0.5, -1));
    userData->shaderProgram = shaderProgram;
    Interface *interface = interfaceNew();
    interfaceSetUserPointer(interface, userData);
    interfaceSetRender(interface, &render);
    int result = interfaceMain(interface);
    free(vertexBuffer);
    free(shaderProgram);
    free(userData);
    return result;
}

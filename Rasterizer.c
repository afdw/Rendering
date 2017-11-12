#include "Rasterizer.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "Utils.h"

size_t rasterizerGetBufferSize(Vector2I size, size_t componentSize, size_t componentCount) {
    return componentSize * componentCount * (size_t) size.x * (size_t) size.y;
}

size_t rasterizerGetBufferOffset(Vector2I offset, Vector2I size, size_t componentSize, size_t componentCount, Vector2I position) {
    return componentSize * componentCount * ((size_t) size.x * (size_t) (position.y - offset.y) + (size_t) (position.x - offset.x));
}

Rasterizer *rasterizerNew(Vector2I wholeSize, Vector2I offset, Vector2I size) {
    Rasterizer *rasterizer = malloc(sizeof(Rasterizer));
    rasterizer->wholeSize = wholeSize;
    rasterizer->offset = offset;
    rasterizer->size = size;
    rasterizer->colorBuffer = calloc(rasterizerGetBufferSize(rasterizer->size, sizeof(ColorComponent), 4), 1);
    rasterizer->depthBuffer = calloc(rasterizerGetBufferSize(rasterizer->size, sizeof(F), 1), 1);
    return rasterizer;
}

Vector2I rasterizerGetOffset(Rasterizer *rasterizer) {
    return rasterizer->offset;
}

Vector2I rasterizerGetSize(Rasterizer *rasterizer) {
    return rasterizer->size;
}

ColorComponent *rasterizerGetColorBuffer(Rasterizer *rasterizer) {
    return rasterizer->colorBuffer;
}

void rasterizerSetWholeSize(Rasterizer *rasterizer, Vector2I wholeSize) {
    rasterizer->wholeSize = wholeSize;
}

void rasterizerSetOffset(Rasterizer *rasterizer, Vector2I offset) {
    rasterizer->offset = offset;
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
    for (I x = rasterizer->offset.x; x <= rasterizer->offset.x + rasterizer->size.x; x++) {
        rasterizerSetColor(rasterizer, vector2INew(x, rasterizer->offset.y), colorNew(1, 0, 0, 1));
        rasterizerSetColor(rasterizer, vector2INew(x, rasterizer->offset.y + rasterizer->size.y - 1), colorNew(1, 0, 0, 1));
    }
    for (I y = rasterizer->offset.y; y <= rasterizer->offset.y + rasterizer->size.y; y++) {
        rasterizerSetColor(rasterizer, vector2INew(rasterizer->offset.x, y), colorNew(1, 0, 0, 1));
        rasterizerSetColor(rasterizer, vector2INew(rasterizer->offset.x + rasterizer->size.x - 1, y), colorNew(1, 0, 0, 1));
    }
}

bool rasterizerIsInside(Rasterizer *rasterizer, Vector2I position) {
    return
        position.x >= rasterizer->offset.x &&
        position.y >= rasterizer->offset.y &&
        position.x < (rasterizer->offset.x + rasterizer->size.x) &&
        position.y < (rasterizer->offset.y + rasterizer->size.y);
}

Color rasterizerGetColor(Rasterizer *rasterizer, Vector2I position) {
    if (rasterizerIsInside(rasterizer, position)) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->offset, rasterizer->size, sizeof(ColorComponent), 4, position);
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
    if (rasterizerIsInside(rasterizer, position)) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->offset, rasterizer->size, sizeof(F), 1, position);
        return *(F *)((char *) rasterizer->depthBuffer + offset);
    }
    return 0;
}

void rasterizerSetColor(Rasterizer *rasterizer, Vector2I position, Color color) {
    if (rasterizerIsInside(rasterizer, position)) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->offset, rasterizer->size, sizeof(ColorComponent), 4, position);
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset) = color.r;
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent)) = color.g;
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent) * 2) = color.b;
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent) * 3) = color.a;
    }
}

void rasterizerSetDepth(Rasterizer *rasterizer, Vector2I position, F depth) {
    if (rasterizerIsInside(rasterizer, position)) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->offset, rasterizer->size, sizeof(F), 1, position);
        *(F *)((char *) rasterizer->depthBuffer + offset) = depth;
    }
}

Vector2I rasterizerNDCToScreen(Rasterizer *rasterizer, Vector3F vector) {
    return vector2INew((I) ((vector.x + 1) / 2 * rasterizer->wholeSize.x), (I) ((vector.y + 1) / 2 * rasterizer->wholeSize.y));
}

Vector3F rasterizerScreenToNDC(Rasterizer *rasterizer, Vector2I vector) {
    return vector3FNew((F) vector.x / rasterizer->wholeSize.x * 2 - 1, (F) vector.y / rasterizer->wholeSize.y * 2 - 1, 0);
}

void rasterizerDrawLine(Rasterizer *rasterizer, Vector3F *positions, void **vertexPixelDatas, ShaderProgram *shaderProgram) {
    PixelShaderFunction pixelShader = shaderProgramGetPixelShader(shaderProgram);
    size_t vertexPixelDataSize = shaderProgramGetVertexPixelDataSize(shaderProgram);
    VertexPixelDataAddFunction vertexPixelDataAdd = shaderProgramGetVertexPixelDataAdd(shaderProgram);
    VertexPixelDataSubtractFunction vertexPixelDataSubtract = shaderProgramGetVertexPixelDataSubtract(shaderProgram);
    VertexPixelDataMultiplyFunction vertexPixelDataMultiply = shaderProgramGetVertexPixelDataMultiply(shaderProgram);
    Vector3F a = positions[0];
    Vector3F b = positions[1];
    Vector2I aScreen = rasterizerNDCToScreen(rasterizer, a);
    Vector2I bScreen = rasterizerNDCToScreen(rasterizer, b);
    void *vertexPixelDataTemp1 = alloca(vertexPixelDataSize);
    void *vertexPixelDataTemp2 = alloca(vertexPixelDataSize);
    void *vertexPixelDataTemp3 = alloca(vertexPixelDataSize);
    Vector2I delta = vector2ISubtract(bScreen, aScreen);
    Vector2I absDelta = vector2INew(abs(delta.x), abs(delta.y));
    int error = 0;
    if (absDelta.x >= absDelta.y) {
        for (int x = aScreen.x, y = aScreen.y; delta.x >= 0 ? (x <= bScreen.x) : (x >= bScreen.x); x += sign(delta.x)) {
            vertexPixelDataSubtract(vertexPixelDatas[1], vertexPixelDatas[0], vertexPixelDataTemp1);
            vertexPixelDataMultiply(vertexPixelDataTemp1, (ColorComponent) abs(x - aScreen.x) / abs(bScreen.x - aScreen.x), vertexPixelDataTemp2);
            vertexPixelDataAdd(vertexPixelDatas[0], vertexPixelDataTemp2, vertexPixelDataTemp3);
            Color color;
            pixelShader(vertexPixelDataTemp3, &color);
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
        for (int x = aScreen.x, y = aScreen.y; delta.y >= 0 ? (y <= bScreen.y) : (y >= bScreen.y); y += sign(delta.y)) {
            vertexPixelDataSubtract(vertexPixelDatas + 1, vertexPixelDatas, vertexPixelDataTemp1);
            vertexPixelDataMultiply(vertexPixelDataTemp1, (ColorComponent) abs(y - aScreen.y) / abs(bScreen.y - aScreen.y), vertexPixelDataTemp2);
            vertexPixelDataAdd(vertexPixelDatas, vertexPixelDataTemp2, vertexPixelDataTemp3);
            Color color;
            pixelShader(vertexPixelDataTemp3, &color);
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

void rasterizerDrawTriangleWireframe(Rasterizer *rasterizer, Vector3F *positions, void **vertexPixelDatas, ShaderProgram *shaderProgram) {
    Vector3F positions1[] = {positions[0], positions[1]};
    Vector3F positions2[] = {positions[1], positions[2]};
    Vector3F positions3[] = {positions[0], positions[2]};
    void *vertexPixelDatas1[] = {vertexPixelDatas[0], vertexPixelDatas[1]};
    void *vertexPixelDatas2[] = {vertexPixelDatas[1], vertexPixelDatas[2]};
    void *vertexPixelDatas3[] = {vertexPixelDatas[0], vertexPixelDatas[2]};
    rasterizerDrawLine(rasterizer, positions1, vertexPixelDatas1, shaderProgram);
    rasterizerDrawLine(rasterizer, positions2, vertexPixelDatas2, shaderProgram);
    rasterizerDrawLine(rasterizer, positions3, vertexPixelDatas3, shaderProgram);
}

void rasterizerDrawTriangle(Rasterizer *rasterizer, Vector3F *positions, void **vertexPixelDatas, ShaderProgram *shaderProgram) {
    PixelShaderFunction pixelShader = shaderProgramGetPixelShader(shaderProgram);
    size_t vertexPixelDataSize = shaderProgramGetVertexPixelDataSize(shaderProgram);
    VertexPixelDataAddFunction vertexPixelDataAdd = shaderProgramGetVertexPixelDataAdd(shaderProgram);
    VertexPixelDataSubtractFunction vertexPixelDataSubtract = shaderProgramGetVertexPixelDataSubtract(shaderProgram);
    VertexPixelDataMultiplyFunction vertexPixelDataMultiply = shaderProgramGetVertexPixelDataMultiply(shaderProgram);
    Vector3F a = positions[0];
    Vector3F b = positions[1];
    Vector3F c = positions[2];
    Vector2I aScreen = rasterizerNDCToScreen(rasterizer, a);
    Vector2I bScreen = rasterizerNDCToScreen(rasterizer, b);
    Vector2I cScreen = rasterizerNDCToScreen(rasterizer, c);
    void *vertexPixelDataTemp1 = alloca(vertexPixelDataSize);
    void *vertexPixelDataTemp2 = alloca(vertexPixelDataSize);
    void *vertexPixelDataTemp3 = alloca(vertexPixelDataSize);
    void *vertexPixelDataTemp4 = alloca(vertexPixelDataSize);
    void *vertexPixelDataTemp5 = alloca(vertexPixelDataSize);
    void *vertexPixelDataTemp6 = alloca(vertexPixelDataSize);
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
                    vertexPixelDataSubtract(vertexPixelDatas[1], vertexPixelDatas[0], vertexPixelDataTemp1);
                    vertexPixelDataSubtract(vertexPixelDatas[2], vertexPixelDatas[0], vertexPixelDataTemp2);
                    vertexPixelDataMultiply(vertexPixelDataTemp1, l, vertexPixelDataTemp3);
                    vertexPixelDataMultiply(vertexPixelDataTemp2, m, vertexPixelDataTemp4);
                    vertexPixelDataAdd(vertexPixelDatas[0], vertexPixelDataTemp3, vertexPixelDataTemp5);
                    vertexPixelDataAdd(vertexPixelDataTemp5, vertexPixelDataTemp4, vertexPixelDataTemp6);
                    Color color;
                    pixelShader(vertexPixelDataTemp6, &color);
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

void rasterizerDraw(Rasterizer *rasterizer, RasterizationTask *rasterizationTask) {
    PrimitivesType primitivesType = rasterizationTaskGetPrimitivesType(rasterizationTask);
    size_t primitivesCount = rasterizationTaskGetPrimitivesCount(rasterizationTask);
    void *vertexBuffer = rasterizationTaskGetVertexBuffer(rasterizationTask);
    void *uniformData = rasterizationTaskGetUniformData(rasterizationTask);
    ShaderProgram *shaderProgram = rasterizationTaskGetShaderProgram(rasterizationTask);
    size_t vertexPixelDataSize = shaderProgramGetVertexPixelDataSize(shaderProgram);
    VertexShaderFunction vertexShader = shaderProgramGetVertexShader(shaderProgram);
    size_t vertexesInPrimitiveCount;
    switch (primitivesType) {
    case LINES:
        vertexesInPrimitiveCount = 2;
        break;
    case TRIANGLES_WIREFRAME:
    case TRIANGLES:
        vertexesInPrimitiveCount = 3;
        break;
    };
    for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; primitiveIndex++) {
        Vector3F *positions = alloca(sizeof(Vector3F) * vertexesInPrimitiveCount);
        void **vertexPixelDatas = alloca(sizeof(void *) * vertexesInPrimitiveCount);
        for (size_t vertexInPrimitiveIndex = 0; vertexInPrimitiveIndex < vertexesInPrimitiveCount; vertexInPrimitiveIndex++) {
            size_t vertexIndex = primitiveIndex * vertexesInPrimitiveCount + vertexInPrimitiveIndex;
            vertexPixelDatas[vertexInPrimitiveIndex] = alloca(vertexPixelDataSize);
            vertexShader(
                uniformData,
                vertexBuffer,
                vertexIndex,
                &positions[vertexInPrimitiveIndex],
                vertexPixelDatas[vertexInPrimitiveIndex]
            );
        }
        switch (primitivesType) {
        case LINES:
            rasterizerDrawLine(rasterizer, positions, vertexPixelDatas, shaderProgram);
            break;
        case TRIANGLES_WIREFRAME:
            rasterizerDrawTriangleWireframe(rasterizer, positions, vertexPixelDatas, shaderProgram);
            break;
        case TRIANGLES:
            rasterizerDrawTriangle(rasterizer, positions, vertexPixelDatas, shaderProgram);
            break;
        };
    }
}

void rasterizerDelete(Rasterizer *rasterizer) {
    free(rasterizer->colorBuffer);
    free(rasterizer->depthBuffer);
    free(rasterizer);
}

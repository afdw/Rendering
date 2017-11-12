#ifndef RASTRTIZATIION_TASK_H
#define RASTRTIZATIION_TASK_H

#include <stddef.h>

#include "ShaderProgram.h"

typedef enum PrimitivesType {
    LINES,
    TRIANGLES_WIREFRAME,
    TRIANGLES
} PrimitivesType;

typedef struct RasterizationTask {
    PrimitivesType primitivesType;
    size_t primitivesCount;
    void *vertexBuffer;
    void *uniformData;
    ShaderProgram *shaderProgram;
} RasterizationTask;

RasterizationTask *rasterizationTaskNew(PrimitivesType primitivesType,
                                        size_t primitivesCount,
                                        void *vertexBuffer,
                                        void *uniformData,
                                        ShaderProgram *shaderProgram);

PrimitivesType rasterizationTaskGetPrimitivesType(RasterizationTask *rasterizationTask);

size_t rasterizationTaskGetPrimitivesCount(RasterizationTask *rasterizationTask);

void *rasterizationTaskGetVertexBuffer(RasterizationTask *rasterizationTask);

void *rasterizationTaskGetUniformData(RasterizationTask *rasterizationTask);

ShaderProgram *rasterizationTaskGetShaderProgram(RasterizationTask *rasterizationTask);

void rasterizationTaskDelete(RasterizationTask *rasterizationTask);

#endif

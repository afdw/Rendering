#ifndef RASTRTIZATIION_TASK_H
#define RASTRTIZATIION_TASK_H

#include <stddef.h>

#include "ShaderProgram.h"

typedef enum RasterizationTaskType {
    DRAW,
    CLEAR
} RasterizationTaskType;

typedef enum PrimitivesType {
    LINES,
    TRIANGLES_WIREFRAME,
    TRIANGLES
} PrimitivesType;

typedef struct DrawTaskData {
    PrimitivesType primitivesType;
    size_t primitivesCount;
    void *vertexBuffer;
    void *uniformData;
    ShaderProgram *shaderProgram;
} DrawTaskData;

typedef struct ClearTaskData {
} ClearTaskData;

typedef struct RasterizationTask {
    RasterizationTaskType type;
    union {
        DrawTaskData drawTaskData;
        ClearTaskData clearTaskData;
    };
} RasterizationTask;

RasterizationTaskType rasterizationTaskGetType(RasterizationTask *rasterizationTask);

RasterizationTask *drawTaskNew(PrimitivesType primitivesType,
                               size_t primitivesCount,
                               void *vertexBuffer,
                               void *uniformData,
                               ShaderProgram *shaderProgram);

PrimitivesType drawTaskGetPrimitivesType(RasterizationTask *drawTask);

size_t drawTaskGetPrimitivesCount(RasterizationTask *drawTask);

void *drawTaskGetVertexBuffer(RasterizationTask *drawTask);

void *drawTaskGetUniformData(RasterizationTask *drawTask);

ShaderProgram *drawTaskGetShaderProgram(RasterizationTask *drawTask);

RasterizationTask *clearTaskNew();

void rasterizationTaskDelete(RasterizationTask *rasterizationTask);

#endif

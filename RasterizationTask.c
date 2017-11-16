#include "RasterizationTask.h"

#include <stdlib.h>

RasterizationTaskType rasterizationTaskGetType(RasterizationTask *rasterizationTask) {
    return rasterizationTask->type;
}

RasterizationTask *drawTaskNew(PrimitivesType primitivesType,
                               size_t primitivesCount,
                               void *vertexBuffer,
                               void *uniformData,
                               ShaderProgram *shaderProgram) {
    RasterizationTask *drawTask = malloc(sizeof(RasterizationTask));
    drawTask->type = DRAW;
    drawTask->drawTaskData.primitivesType = primitivesType;
    drawTask->drawTaskData.primitivesCount = primitivesCount;
    drawTask->drawTaskData.vertexBuffer = vertexBuffer;
    drawTask->drawTaskData.uniformData = uniformData;
    drawTask->drawTaskData.shaderProgram = shaderProgram;
    return drawTask;
}

PrimitivesType drawTaskGetPrimitivesType(RasterizationTask *drawTask) {
    return drawTask->drawTaskData.primitivesType;
}

size_t drawTaskGetPrimitivesCount(RasterizationTask *drawTask) {
    return drawTask->drawTaskData.primitivesCount;
}

void *drawTaskGetVertexBuffer(RasterizationTask *drawTask) {
    return drawTask->drawTaskData.vertexBuffer;
}

void *drawTaskGetUniformData(RasterizationTask *drawTask) {
    return drawTask->drawTaskData.uniformData;
}

ShaderProgram *drawTaskGetShaderProgram(RasterizationTask *drawTask) {
    return drawTask->drawTaskData.shaderProgram;
}

void drawTaskDelete(RasterizationTask *drawTask) {
    free(drawTask->drawTaskData.uniformData);
    free(drawTask);
}

RasterizationTask *clearTaskNew() {
    RasterizationTask *clearTask = malloc(sizeof(RasterizationTask));
    clearTask->type = CLEAR;
    return clearTask;
}

void clearTaskDelete(RasterizationTask *clearTask) {
    free(clearTask);
}

void rasterizationTaskDelete(RasterizationTask *rasterizationTask) {
    switch (rasterizationTaskGetType(rasterizationTask)) {
    case DRAW:
        drawTaskDelete(rasterizationTask);
        break;
    case CLEAR:
        clearTaskDelete(rasterizationTask);
        break;
    }
}

#include "RasterizationTask.h"

#include <stdlib.h>

RasterizationTask *rasterizationTaskNew(PrimitivesType primitivesType,
                                        size_t primitivesCount,
                                        void *vertexBuffer,
                                        void *uniformData,
                                        ShaderProgram *shaderProgram) {
    RasterizationTask *rasterizationTask = malloc(sizeof(RasterizationTask));
    rasterizationTask->primitivesType = primitivesType;
    rasterizationTask->primitivesCount = primitivesCount;
    rasterizationTask->vertexBuffer = vertexBuffer;
    rasterizationTask->uniformData = uniformData;
    rasterizationTask->shaderProgram = shaderProgram;
    return rasterizationTask;
}

PrimitivesType rasterizationTaskGetPrimitivesType(RasterizationTask *rasterizationTask) {
    return rasterizationTask->primitivesType;
}

size_t rasterizationTaskGetPrimitivesCount(RasterizationTask *rasterizationTask) {
    return rasterizationTask->primitivesCount;
}

void *rasterizationTaskGetVertexBuffer(RasterizationTask *rasterizationTask) {
    return rasterizationTask->vertexBuffer;
}

void *rasterizationTaskGetUniformData(RasterizationTask *rasterizationTask) {
    return rasterizationTask->uniformData;
}

ShaderProgram *rasterizationTaskGetShaderProgram(RasterizationTask *rasterizationTask) {
    return rasterizationTask->shaderProgram;
}

void rasterizationTaskDelete(RasterizationTask *rasterizationTask) {
    free(rasterizationTask);
}

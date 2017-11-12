#include "ShaderProgram.h"

#include <stdlib.h>

ShaderProgram *shaderProgramNew(VertexShaderFunction vertexShader,
                                PixelShaderFunction pixelShader,
                                size_t vertexPixelDataSize,
                                VertexPixelDataAddFunction vertexPixelDataAdd,
                                VertexPixelDataSubtractFunction vertexPixelDataSubtract,
                                VertexPixelDataMultiplyFunction vertexPixelDataMultiply) {
    ShaderProgram *shaderProgram = malloc(sizeof(ShaderProgram));
    shaderProgram->vertexShader = vertexShader;
    shaderProgram->pixelShader = pixelShader;
    shaderProgram->vertexPixelDataSize = vertexPixelDataSize;
    shaderProgram->vertexPixelDataAdd = vertexPixelDataAdd;
    shaderProgram->vertexPixelDataSubtract = vertexPixelDataSubtract;
    shaderProgram->vertexPixelDataMultiply = vertexPixelDataMultiply;
    return shaderProgram;
}

VertexShaderFunction shaderProgramGetVertexShader(ShaderProgram *shaderProgram) {
    return shaderProgram->vertexShader;
}

PixelShaderFunction shaderProgramGetPixelShader(ShaderProgram *shaderProgram) {
    return shaderProgram->pixelShader;
}

size_t shaderProgramGetVertexPixelDataSize(ShaderProgram *shaderProgram) {
    return shaderProgram->vertexPixelDataSize;
}

VertexPixelDataAddFunction shaderProgramGetVertexPixelDataAdd(ShaderProgram *shaderProgram) {
    return shaderProgram->vertexPixelDataAdd;
}

VertexPixelDataSubtractFunction shaderProgramGetVertexPixelDataSubtract(ShaderProgram *shaderProgram) {
    return shaderProgram->vertexPixelDataSubtract;
}

VertexPixelDataMultiplyFunction shaderProgramGetVertexPixelDataMultiply(ShaderProgram *shaderProgram) {
    return shaderProgram->vertexPixelDataMultiply;
}

void shaderProgramDelete(ShaderProgram *shaderProgram) {
    free(shaderProgram);
}

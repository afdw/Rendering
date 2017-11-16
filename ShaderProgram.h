#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <stddef.h>

#include "VectorTypes.h"
#include "Vector3F.h"
#include "Color.h"

typedef void (*VertexShaderFunction)(void *uniformData,
                                     void *vertexBuffer,
                                     size_t vertexIndex,
                                     Vector3F *outPosition,
                                     void *vertexPixelData);

typedef void (*PixelShaderFunction)(void *vertexPixelData,
                                    Color *outColor);

typedef void (*VertexPixelDataAddFunction)(void *a, void *b, void *out);

typedef void (*VertexPixelDataSubtractFunction)(void *a, void *b, void *out);

typedef void (*VertexPixelDataMultiplyFunction)(void *vertexPixelData, F n, void *out);

typedef struct ShaderProgram {
    VertexShaderFunction vertexShader;
    PixelShaderFunction pixelShader;
    size_t vertexPixelDataSize;
    VertexPixelDataAddFunction vertexPixelDataAdd;
    VertexPixelDataSubtractFunction vertexPixelDataSubtract;
    VertexPixelDataMultiplyFunction vertexPixelDataMultiply;
} ShaderProgram;

ShaderProgram *shaderProgramNew(VertexShaderFunction vertexShader,
                                PixelShaderFunction pixelShader,
                                size_t vertexPixelDataSize,
                                VertexPixelDataAddFunction vertexPixelDataAdd,
                                VertexPixelDataSubtractFunction vertexPixelDataSubtract,
                                VertexPixelDataMultiplyFunction vertexPixelDataMultiply);

VertexShaderFunction shaderProgramGetVertexShader(ShaderProgram *shaderProgram);

PixelShaderFunction shaderProgramGetPixelShader(ShaderProgram *shaderProgram);

size_t shaderProgramGetVertexPixelDataSize(ShaderProgram *shaderProgram);

VertexPixelDataAddFunction shaderProgramGetVertexPixelDataAdd(ShaderProgram *shaderProgram);

VertexPixelDataSubtractFunction shaderProgramGetVertexPixelDataSubtract(ShaderProgram *shaderProgram);

VertexPixelDataMultiplyFunction shaderProgramGetVertexPixelDataMultiply(ShaderProgram *shaderProgram);

void shaderProgramDelete(ShaderProgram *shaderProgram);

#endif

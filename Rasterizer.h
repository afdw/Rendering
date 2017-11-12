#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <string.h>

#ifndef NO_RENDERING
#include <GLFW/glfw3.h>
#endif

#include "VectorTypes.h"
#include "Vector2I.h"
#include "Vector3F.h"
#include "Color.h"
#include "RasterizationTask.h"

typedef struct Rasterizer {
    Vector2I wholeSize;
    Vector2I offset;
    Vector2I size;
    ColorComponent *colorBuffer;
    F *depthBuffer;
} Rasterizer;

Rasterizer *rasterizerNew(Vector2I wholeSize, Vector2I offset, Vector2I size);

Vector2I rasterizerGetOffset(Rasterizer *rasterizer);

Vector2I rasterizerGetSize(Rasterizer *rasterizer);

ColorComponent *rasterizerGetColorBuffer(Rasterizer *rasterizer);

void rasterizerSetWholeSize(Rasterizer *rasterizer, Vector2I wholeSize);

void rasterizerSetOffset(Rasterizer *rasterizer, Vector2I offset);

void rasterizerSetSize(Rasterizer *rasterizer, Vector2I size);

void rasterizerClear(Rasterizer *rasterizer);

Color rasterizerGetColor(Rasterizer *rasterizer, Vector2I position);

F rasterizerGetDepth(Rasterizer *rasterizer, Vector2I position);

void rasterizerSetColor(Rasterizer *rasterizer, Vector2I position, Color color);

void rasterizerSetDepth(Rasterizer *rasterizer, Vector2I position, F depth);

void rasterizerDraw(Rasterizer *rasterizer, RasterizationTask *rasterizationTask);

void rasterizerDelete(Rasterizer *rasterizer);

#endif

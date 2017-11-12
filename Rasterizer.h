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

typedef struct Rasterizer {
    Vector2I size;
    ColorComponent *colorBuffer;
    F *depthBuffer;
} Rasterizer;

Rasterizer *rasterizerNew(Vector2I size);

Vector2I rasterizerGetSize(Rasterizer *rasterizer);

ColorComponent *rasterizerGetColorBuffer(Rasterizer *rasterizer);

void rasterizerSetSize(Rasterizer *rasterizer, Vector2I size);

void rasterizerClear(Rasterizer *rasterizer);

void rasterizerSetColor(Rasterizer *rasterizer, Vector2I position, Color color);

void rasterizerSetDepth(Rasterizer *rasterizer, Vector2I position, F depth);

Color rasterizerGetColor(Rasterizer *rasterizer, Vector2I position);

F rasterizerGetDepth(Rasterizer *rasterizer, Vector2I position);

Vector2I rasterizerNDCToScreen(Rasterizer *rasterizer, Vector3F vector);

Vector3F rasterizerScreenToNDC(Rasterizer *rasterizer, Vector2I vector);

void rasterizerDrawLine(Rasterizer *rasterizer, Vector3F p0, Vector3F p1, Color color);

void rasterizerDrawTriangleWireframe(Rasterizer *rasterizer, Vector3F a, Vector3F b, Vector3F c, Color color);

void rasterizerDrawTriangle(Rasterizer *rasterizer, Vector3F a, Vector3F b, Vector3F c, Color color);

void rasterizerDelete(Rasterizer *rasterizer);

#endif

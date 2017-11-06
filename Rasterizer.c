#include "Rasterizer.h"

#include <stdlib.h>

#include "Utils.h"

#ifndef NO_RENDERING
Vector2I rasterizerGetWindowSize(GLFWwindow* window) {
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);
    return vector2INew((I) width, (I) height);
}
#endif

size_t rasterizerGetBufferSize(Vector2I size, size_t componentSize, size_t componentCount) {
    return componentSize * componentCount * (size_t) size.x * (size_t) size.y;
}

size_t rasterizerGetBufferOffset(Vector2I size, size_t componentSize, size_t componentCount, Vector2I position) {
    return componentSize * componentCount * ((size_t) size.x * (size_t) position.y + (size_t) position.x);
}

Rasterizer *rasterizerNew(Vector2I size) {
    Rasterizer *rasterizer = malloc(sizeof(Rasterizer));
    rasterizer->size = size;
    rasterizer->colorBuffer = calloc(rasterizerGetBufferSize(rasterizer->size, sizeof(ColorComponent), 4), 1);
    rasterizer->depthBuffer = calloc(rasterizerGetBufferSize(rasterizer->size, sizeof(F), 1), 1);
    return rasterizer;
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
}

void rasterizerSetColor(Rasterizer *rasterizer, Vector2I position, Color color) {
    if (position.x >= 0 && position.y >= 0 && position.x < rasterizer->size.x && position.y < rasterizer->size.y) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->size, sizeof(ColorComponent), 4, position);
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset) = color.r;
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent)) = color.g;
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent) * 2) = color.b;
        *(ColorComponent *)((char *) rasterizer->colorBuffer + offset + sizeof(ColorComponent) * 3) = color.a;
    }
}

void rasterizerSetDepth(Rasterizer *rasterizer, Vector2I position, F depth) {
    if (position.x >= 0 && position.y >= 0 && position.x < rasterizer->size.x && position.y < rasterizer->size.y) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->size, sizeof(F), 1, position);
        *(F *)((char *) rasterizer->depthBuffer + offset) = depth;
    }
}

Color rasterizerGetColor(Rasterizer *rasterizer, Vector2I position) {
    if (position.x >= 0 && position.y >= 0 && position.x < rasterizer->size.x && position.y < rasterizer->size.y) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->size, sizeof(ColorComponent), 4, position);
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
    if (position.x >= 0 && position.y >= 0 && position.x < rasterizer->size.x && position.y < rasterizer->size.y) {
        size_t offset = rasterizerGetBufferOffset(rasterizer->size, sizeof(F), 1, position);
        return *(F *)((char *) rasterizer->depthBuffer + offset);
    }
    return 0;
}

Vector2I rasterizerNDCToScreen(Rasterizer *rasterizer, Vector3F vector) {
    return vector2INew((I) ((vector.x + 1) / 2 * rasterizer->size.x), (I) ((vector.y + 1) / 2 * rasterizer->size.y));
}

Vector3F rasterizerScreenToNDC(Rasterizer *rasterizer, Vector2I vector) {
    return vector3FNew((F) vector.x / rasterizer->size.x * 2 - 1, (F) vector.y / rasterizer->size.y * 2 - 1, 0);
}

void rasterizerDrawLine(Rasterizer *rasterizer, Vector3F p0, Vector3F p1, Color color) {
    Vector2I p0Screen = rasterizerNDCToScreen(rasterizer, p0);
    Vector2I p1Screen = rasterizerNDCToScreen(rasterizer, p1);
    Vector2I delta = vector2ISubstract(p1Screen, p0Screen);
    Vector2I absDelta = vector2INew(abs(delta.x), abs(delta.y));
    int error = 0;
    if (absDelta.x >= absDelta.y) {
        for (int x = p0Screen.x, y = p0Screen.y; delta.x >= 0 ? (x <= p1Screen.x) : (x >= p1Screen.x); x += sign(delta.x)) {
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
        for (int x = p0Screen.x, y = p0Screen.y; delta.y >= 0 ? (y <= p1Screen.y) : (y >= p1Screen.y); y += sign(delta.y)) {
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

void rasterizerDrawTriangleWireframe(Rasterizer *rasterizer, Vector3F a, Vector3F b, Vector3F c, Color color) {
    rasterizerDrawLine(rasterizer, a, b, color);
    rasterizerDrawLine(rasterizer, b, c, color);
    rasterizerDrawLine(rasterizer, a, c, color);
}

void rasterizerDrawTriangle(Rasterizer *rasterizer, Vector3F a, Vector3F b, Vector3F c, Color color) {
    Vector2I aScreen = rasterizerNDCToScreen(rasterizer, a);
    Vector2I bScreen = rasterizerNDCToScreen(rasterizer, b);
    Vector2I cScreen = rasterizerNDCToScreen(rasterizer, c);
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

#ifndef NO_RENDERING
void rasterizerDraw(Rasterizer *rasterizer) {
    glDrawPixels(rasterizer->size.x, rasterizer->size.y, GL_RGBA, GL_FLOAT, rasterizer->colorBuffer);
}
#endif

void rasterizerDelete(Rasterizer *rasterizer) {
    free(rasterizer->colorBuffer);
    free(rasterizer->depthBuffer);
    free(rasterizer);
}

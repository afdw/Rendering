#ifndef INTERFACE_H
#define INTERFACE_H

#include "Rasterizer.h"

typedef struct Interface Interface;

typedef void (*RenderFunction)(Interface *interface, Rasterizer *rasterizer);

typedef struct Interface {
    RenderFunction render;
    void *userPointer;
} Interface;

Interface *interfaceNew();

void *interfaceGetUserPointer(Interface *interface);

void interfaceSetUserPointer(Interface *interface, void *userPointer);

void interfaceSetRender(Interface *interface, RenderFunction render);

int interfaceMain(Interface *interface);

#endif

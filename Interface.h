#ifndef INTERFACE_H
#define INTERFACE_H

#include "Vector2I.h"
#include "List.h"
#include "Rasterizer.h"

typedef struct Interface Interface;

typedef void (*RenderFunction)(Interface *interface, Vector2I size, List *rasterizationTasksList);

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

#include "Interface.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef NO_RENDERING
#include <GLFW/glfw3.h>
#endif

Interface *interfaceNew() {
    Interface *interface = malloc(sizeof(Interface));
    interface->render = NULL;
    interface->userPointer = NULL;
    return interface;
}

void *interfaceGetUserPointer(Interface *interface) {
    return interface->userPointer;
}

void interfaceSetUserPointer(Interface *interface, void *userPointer) {
    interface->userPointer = userPointer;
}

void interfaceSetRender(Interface *interface, RenderFunction render) {
    interface->render = render;
}

int interfaceMain(Interface *interface) {
    Rasterizer *rasterizer = rasterizerNew(vector2INew(1000, 1000), vector2INew(0, 0), vector2INew(1000, 1000));
#ifndef NO_RENDERING
    if (!glfwInit()) {
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(640, 480, "Rendering", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        int width;
        int height;
        glfwGetWindowSize(window, &width, &height);
        Vector2I size = vector2INew((I) width, (I) height);
        rasterizerSetWholeSize(rasterizer, size);
        rasterizerSetOffset(rasterizer, vector2INew(200, 200));
        rasterizerSetSize(rasterizer, vector2ISubstract(size, rasterizerGetOffset(rasterizer)));
#else
    while (true) {
#endif
        interface->render(interface, size, rasterizer);
#ifndef NO_RENDERING
        glViewport(0, 0, (GLsizei) width, (GLsizei) height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glRasterPos2i(rasterizerGetOffset(rasterizer).x, rasterizerGetOffset(rasterizer).y);
        glDrawPixels(rasterizerGetSize(rasterizer).x, rasterizerGetSize(rasterizer).y, GL_RGBA, GL_FLOAT, rasterizerGetColorBuffer(rasterizer));
        GLenum error = glGetError();
        if (error != 0) {
            printf("%x\n", glGetError());
            fflush(stdout);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
#else
        break;
    }
#endif
    rasterizerDelete(rasterizer);
    free(interface);
    return 0;
}

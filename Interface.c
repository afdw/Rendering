#include "Interface.h"

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
    Rasterizer *rasterizer = rasterizerNew(vector2INew(1000, 1000));
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
        rasterizerSetSize(rasterizer, vector2INew((I) width, (I) height));
#else
    while (1) {
#endif
        interface->render(interface, rasterizer);
#ifndef NO_RENDERING
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

#include "Interface.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

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

typedef struct SharedData {
    pthread_mutex_t shouldWorkMutex;
    bool shouldWork;
    List *rasterizationTasksList;
} SharedData;

typedef struct WorkerData {
    SharedData *sharedData;
    pthread_mutex_t rasterizationTasksListFilledMutex;
    pthread_cond_t rasterizationTasksListFilledCondition;
    bool rasterizationTasksListFilled;
    pthread_mutex_t doneMutex;
    pthread_cond_t doneCondition;
    bool done;
    pthread_mutex_t rasterizerMutex;
    Rasterizer *rasterizer;
} WorkerData;

void *workerMain(void *arg) {
    WorkerData *workerData = (WorkerData *) arg;
    while (true) {
        pthread_mutex_lock(&workerData->sharedData->shouldWorkMutex);
        bool shouldWork = workerData->sharedData->shouldWork;
        pthread_mutex_unlock(&workerData->sharedData->shouldWorkMutex);
        if (!shouldWork) {
            break;
        }
        pthread_mutex_lock(&workerData->doneMutex);
        while (!(!workerData->done)) {
            pthread_cond_wait(&workerData->doneCondition, &workerData->doneMutex);
        }
        pthread_mutex_unlock(&workerData->doneMutex);
        pthread_mutex_lock(&workerData->rasterizationTasksListFilledMutex);
        while (!workerData->rasterizationTasksListFilled) {
            pthread_cond_wait(&workerData->rasterizationTasksListFilledCondition, &workerData->rasterizationTasksListFilledMutex);
        }
        pthread_mutex_unlock(&workerData->rasterizationTasksListFilledMutex);
        for (size_t i = 0; i < listGetSize(workerData->sharedData->rasterizationTasksList); i++) {
            RasterizationTask *rasterizationTask;
            listGet(workerData->sharedData->rasterizationTasksList, i, &rasterizationTask);
            pthread_mutex_lock(&workerData->rasterizerMutex);
            rasterizerDoTask(workerData->rasterizer, rasterizationTask);
            pthread_mutex_unlock(&workerData->rasterizerMutex);
        }
        pthread_mutex_lock(&workerData->doneMutex);
        workerData->done = true;
        pthread_cond_signal(&workerData->doneCondition);
        pthread_mutex_unlock(&workerData->doneMutex);
    }
    return NULL;
}

int interfaceMain(Interface *interface) {
    SharedData *sharedData = alloca(sizeof(SharedData));
    pthread_mutex_init(&sharedData->shouldWorkMutex, NULL);
    sharedData->shouldWork = true;
    sharedData->rasterizationTasksList = NULL;
    size_t workersCount = 16;
    pthread_t *workersThreads = alloca(sizeof(pthread_t) * workersCount);
    WorkerData **workersData = alloca(sizeof(WorkerData *) * workersCount);
    for (size_t i = 0; i < workersCount; i++) {
        workersData[i] = alloca(sizeof(WorkerData));
        workersData[i]->sharedData = sharedData;
        workersData[i]->rasterizationTasksListFilled = false;
        pthread_mutex_init(&workersData[i]->rasterizationTasksListFilledMutex, NULL);
        pthread_cond_init(&workersData[i]->rasterizationTasksListFilledCondition, NULL);
        workersData[i]->done = false;
        pthread_mutex_init(&workersData[i]->doneMutex, NULL);
        pthread_cond_init(&workersData[i]->doneCondition, NULL);
        pthread_mutex_init(&workersData[i]->rasterizerMutex, NULL);
        workersData[i]->rasterizer = rasterizerNew(vector2INew(0, 0), vector2INew(0, 0), vector2INew(0, 0));
        pthread_create(&workersThreads[i], NULL, &workerMain, workersData[i]);
    }
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
        for (size_t i = 0; i < workersCount; i++) {
            pthread_mutex_lock(&workersData[i]->rasterizerMutex);
            rasterizerSetWholeSize(workersData[i]->rasterizer, size);
            // FIXME: better zone allocation
            rasterizerSetOffset(workersData[i]->rasterizer, vector2INew(0, (size.y / workersCount + 1) * i));
            rasterizerSetSize(workersData[i]->rasterizer, vector2INew(size.x, size.y / workersCount + 1));
            pthread_mutex_unlock(&workersData[i]->rasterizerMutex);
        }
#else
    Vector2I size = vector2INew(1024, 1024);
    for (size_t i = 0; i < workersCount; i++) {
        pthread_mutex_lock(&workersData[i]->rasterizerMutex);
        rasterizerSetWholeSize(workersData[i]->rasterizer, size);
        rasterizerSetOffset(workersData[i]->rasterizer, vector2INew(0, 0));
        rasterizerSetSize(workersData[i]->rasterizer, size);
        pthread_mutex_unlock(&workersData[i]->rasterizerMutex);
    }
    while (true) {
#endif
        if (interface->render != NULL) {
            for (size_t i = 0; i < workersCount; i++) {
                pthread_mutex_lock(&workersData[i]->rasterizationTasksListFilledMutex);
            }
            if (sharedData->rasterizationTasksList != NULL) {
                for (size_t i = 0; i < listGetSize(sharedData->rasterizationTasksList); i++) {
                    RasterizationTask *rasterizationTask;
                    listGet(sharedData->rasterizationTasksList, i, &rasterizationTask);
                    rasterizationTaskDelete(rasterizationTask);
                }
                listDelete(sharedData->rasterizationTasksList);
            }
            sharedData->rasterizationTasksList = listNew(sizeof(RasterizationTask *));
            interface->render(interface, size, sharedData->rasterizationTasksList);
            for (size_t i = 0; i < workersCount; i++) {
                workersData[i]->rasterizationTasksListFilled = true;
                pthread_cond_signal(&workersData[i]->rasterizationTasksListFilledCondition);
                pthread_mutex_unlock(&workersData[i]->rasterizationTasksListFilledMutex);
            }
            for (size_t i = 0; i < workersCount; i++) {
                pthread_mutex_lock(&workersData[i]->doneMutex);
                while (!workersData[i]->done) {
                    pthread_cond_wait(&workersData[i]->doneCondition, &workersData[i]->doneMutex);
                }
                workersData[i]->done = false;
                pthread_mutex_lock(&workersData[i]->rasterizationTasksListFilledMutex);
                workersData[i]->rasterizationTasksListFilled = false;
                pthread_mutex_unlock(&workersData[i]->rasterizationTasksListFilledMutex);
                pthread_cond_signal(&workersData[i]->doneCondition);
                pthread_mutex_unlock(&workersData[i]->doneMutex);
            }
        }
#ifndef NO_RENDERING
        glViewport(0, 0, (GLsizei) width, (GLsizei) height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        for (size_t i = 0; i < workersCount; i++) {
            pthread_mutex_lock(&workersData[i]->rasterizerMutex);
            glRasterPos2i(rasterizerGetOffset(workersData[i]->rasterizer).x, rasterizerGetOffset(workersData[i]->rasterizer).y);
            glDrawPixels(rasterizerGetSize(workersData[i]->rasterizer).x, rasterizerGetSize(workersData[i]->rasterizer).y, GL_RGBA, GL_FLOAT, rasterizerGetColorBuffer(workersData[i]->rasterizer));
            pthread_mutex_unlock(&workersData[i]->rasterizerMutex);
        }
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
    if (sharedData->rasterizationTasksList != NULL) {
        for (size_t i = 0; i < listGetSize(sharedData->rasterizationTasksList); i++) {
            RasterizationTask *rasterizationTask;
            listGet(sharedData->rasterizationTasksList, i, &rasterizationTask);
            rasterizationTaskDelete(rasterizationTask);
        }
        listDelete(sharedData->rasterizationTasksList);
    }
    sharedData->rasterizationTasksList = listNew(sizeof(RasterizationTask *));
    for (size_t i = 0; i < workersCount; i++) {
        pthread_mutex_lock(&sharedData->shouldWorkMutex);
        sharedData->shouldWork = false;
        pthread_mutex_unlock(&sharedData->shouldWorkMutex);
        pthread_mutex_lock(&workersData[i]->rasterizationTasksListFilledMutex);
        workersData[i]->rasterizationTasksListFilled = true;
        pthread_cond_signal(&workersData[i]->rasterizationTasksListFilledCondition);
        pthread_mutex_unlock(&workersData[i]->rasterizationTasksListFilledMutex);
        pthread_mutex_lock(&workersData[i]->rasterizerMutex);
        rasterizerDelete(workersData[i]->rasterizer);
        pthread_mutex_unlock(&workersData[i]->rasterizerMutex);
        pthread_join(workersThreads[i], NULL);
    }
    listDelete(sharedData->rasterizationTasksList);
    free(interface);
    return 0;
}

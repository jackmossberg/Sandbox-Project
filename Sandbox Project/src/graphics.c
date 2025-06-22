#include "graphics.h"

#define _FL "graphics.c"

typedef struct {
    gl_heaprenderer** heaprenderables;
    size_t heaprenderable_objcount;
    gl_mesh** heapmeshes;
    size_t heapmesh_objcount;
    globject_tcouple* openglobjects;
    size_t openglobjects_objcount;
} app_resources;

void error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

gl_app* glapi_CreateApp(uint16_t window_width, uint16_t window_height, uint16_t frame_width, uint16_t frame_height, const char* title, bool resizable) {
    gl_app* app = (gl_app*)calloc(1, sizeof(gl_app));
    if (!app) {
        fprintf(stderr, "[%s] - Failure to allocate 'app' to heap in glapi_CreateApp\n", _FL); 
        exit(1);
    }

    app->window = (gl_window*)calloc(1, sizeof(gl_window));
    if (!app->window) {
        fprintf(stderr, "[%s] - Failure to allocate 'app->window' to heap in glapi_CreateApp\n", _FL); 
        exit(1);
    }

    app->resources = (void*)calloc(1, sizeof(app_resources));
    if (!app->resources) {
        fprintf(stderr, "[%s] - Failure to allocate 'app->resources' to heap in glapi_CreateApp\n", _FL); 
        exit(1);
    }

    glfwSetErrorCallback(error_callback);
    printf("[%s] - Starting program\n", _FL);

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("[%s] - GLFW version: %d.%d.%d\n", _FL, major, minor, revision);

    printf("[%s] - Initializing GLFW\n", _FL);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }
    printf("[%s] - GLFW initialized\n", _FL);

    printf("[%s] - Setting window hints\n", _FL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_RESIZABLE, resizable);

    printf("[%s] - Attempting to create window\n", _FL);
    app->window->pointer = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
    if (!app->window->pointer) {
        fprintf(stderr, "[%s] - Failed to create window\n", _FL);
        glfwTerminate();
        exit(1);
    }
    printf("[%s] - Window created\n", _FL);

    glfwMakeContextCurrent(app->window->pointer);
    printf("[%s] - Context made current\n", _FL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwDestroyWindow(app->window->pointer);
        glfwTerminate();
        exit(1);
    }
    printf("[%s] - GLAD initialized\n", _FL);

    const GLubyte* version = glGetString(GL_VERSION);
    if (!version) {
        fprintf(stderr, "[%s] - Failed to get OpenGL version\n", _FL);
    } else {
        printf("[%s] - OpenGL version: %s\n", _FL, version);
    }

    glfwShowWindow(app->window->pointer);
    fprintf(stderr, "[%s] - Window shown\n", _FL);

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);

    return app;
}

void glapi_RenderApp(gl_app* app, float r, float g, float b) {
    glClearColor(r,g,b,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(app->window->pointer);
    glfwPollEvents();
}

int glapi_DestroyApp(gl_app* app) {
    app_resources* graphics_resources = (app_resources*)app->resources;
    for (int i = 0; i < graphics_resources->openglobjects_objcount; i++) {
        // free all opengl objects according to their types
    }
    glfwDestroyWindow(app->window->pointer);
    glfwTerminate();
    free(app->window);
    free(app->resources);
    free(app);
    return 0;
}

int glapi_ShouldAppClose(gl_app* app) {
    return glfwWindowShouldClose(app->window->pointer);
}
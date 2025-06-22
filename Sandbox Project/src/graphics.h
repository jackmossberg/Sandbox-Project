#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define API extern

typedef GLuint gl_shader;
typedef GLuint gl_texture;
typedef GLuint gl_vao;
typedef GLuint gl_framebuffer;
typedef GLFWwindow gl_apiwindow;

#define T (globject_tcouple)
typedef struct globject_tcouple { void* address; int objtype; } globject_tcouple;

typedef struct gl_mesh {
    GLfloat* positions;
    GLuint* indices;
    GLfloat* uvs;
    GLfloat* normals;
    size_t positions_size;
    size_t indices_size;
    size_t uvs_size;
    size_t normals_size;
} gl_mesh;

typedef struct gl_heaprenderer {
    gl_mesh* meshes;
    gl_vao* vaos;
    gl_shader* shaders;
    gl_texture* textures;
} gl_heaprenderer;

typedef struct gl_stackrenderer {
    gl_vao vao;
    gl_shader shader;
    gl_texture texture;
} gl_stackrenderer;

typedef struct gl_buffer {
    GLuint framebuffer;
    gl_stackrenderer mesh;
} gl_buffer;

typedef struct gl_window {
    gl_apiwindow* pointer;
    uint16_t window_width;
    uint16_t window_height;
    uint16_t frame_width;
    uint16_t frame_height;

    gl_buffer frame;
} gl_window;

typedef struct gl_app {
    gl_window* window;
    void* resources;
} gl_app;

API gl_app* glapi_CreateApp(uint16_t window_width, uint16_t window_height, uint16_t frame_width, uint16_t frame_height, const char* title, bool resizable);
API int glapi_DestroyApp(gl_app* app);
API void glapi_RenderApp(gl_app* app, float r, float g, float b);
API int glapi_ShouldAppClose(gl_app* app);

API void glapi_CreateBuffer();
API void glapi_DestroyBuffer();
API void glapi_DrawBuffer();

API void glapi_CreateStackRenderer();
API void glapi_DestroyStackRenderer();
API void glapi_DrawStackRenderer();

API void glapi_CreateHeapRenderer(gl_app* app, gl_heaprenderer* hrenderer, gl_mesh* meshes, size_t mcount);
API void glapi_DestroyHeapRenderer();
API void glapi_DrawHeapRenderer();
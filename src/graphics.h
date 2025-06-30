#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define API extern

#define VAO 0
#define SHADER 1
#define TEXTURE 2
#define FRAMEBUFFER 3

typedef GLuint gl_shader;
typedef GLuint gl_texture;
typedef GLuint gl_vao;
typedef GLuint gl_framebuffer;
typedef GLFWwindow gl_apiwindow;

#define T (globject_tcouple)
typedef struct globject_tcouple { GLuint* globject; unsigned int objtype; } globject_tcouple;

#define U (gl_uniform)
typedef struct gl_uniform { void* data; unsigned int type; } gl_uniform;
    
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

typedef struct gl_window {
    gl_apiwindow* pointer;
    uint16_t window_width;
    uint16_t window_height;
    float r, g, b;
} gl_window;

typedef struct gl_app {
    gl_window* window;
    void* resources;
} gl_app;

API void glapi_AppendOpenGLObjects(gl_app* app, globject_tcouple tcouple);

API gl_app* glapi_CreateApp(uint16_t window_width, uint16_t window_height, const char* title, bool resizable, float r, float g, float b);
API int glapi_DestroyApp(gl_app* app);
API void glapi_BindApp(gl_app* app);
API void glapi_UnbindApp(gl_app* app);
API int glapi_ShouldAppClose(gl_app* app);

API GLuint glapi_GenShaderProgram_f(gl_app* app, const char* v_fpath, const char* f_fpath, GLuint* address);
API GLuint glapi_GenShaderProgram_s(gl_app* app, const char* v_source, const char* f_source, GLuint* address);
API GLuint glapi_GenVertexBufferObjectFromMesh(gl_app* app, gl_mesh* mesh, GLuint* address);
API GLuint glapi_GenTextureFromFpath(gl_app* app, const char* fpath, GLuint* address);

API void glapi_BindVertexBufferObject(gl_vao* vao);
API void glapi_UnbindVertexBufferObject();
API void glapi_BindShader(gl_shader* shader);
API void glapi_UnbindShader();
API void glapi_DrawVertexBufferObject(size_t isize);
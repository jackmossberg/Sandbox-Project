#include "graphics.h"

#define _FL "graphics.c"

#define APIC static
#define GL_ADDRESS_ARR_APPEND_AMOUNT 25

typedef struct {
    gl_heaprenderer** heaprenderables;
    size_t heaprenderable_objcount;
    size_t heaprenderable_objcapacity;
    gl_mesh** heapmeshes;
    size_t heapmesh_objcount;
    size_t heapmesh_objcapacity;
    globject_tcouple* openglobjects;
    size_t openglobjects_objcount;
    size_t openglobjects_objcapacity;
} app_resources;

APIC char* load_raw_txt(const char* fpath);
APIC GLuint compile_shader_code(const char* source, GLenum type);
APIC GLuint gen_shader_program(gl_app* app, const char* v_fpath, const char* f_fpath, GLuint* address);
APIC GLuint gen_vertex_buffer_object_from_mesh(gl_app* app, gl_mesh* mesh, GLuint* address);
APIC GLuint gen_texture_from_fpath(gl_app* app, const char* fpath, GLuint* address);

void resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

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
    app_resources* graphics_resources = (app_resources*)app->resources;
    graphics_resources->openglobjects_objcount = 0;
    graphics_resources->heapmesh_objcount = 0;
    graphics_resources->heaprenderable_objcount = 0;

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

    glViewport(0, 0, app->window->window_width, app->window->window_height);
    printf("[%s] - GLViewport initialized\n", _FL);
    
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
    if (graphics_resources->openglobjects) {
        globject_tcouple* clist = graphics_resources->openglobjects;
        size_t clistlen = graphics_resources->openglobjects_objcount;

        for (size_t i = 0; i < clistlen; i++) {
            if (!clist[i].globject) {
                fprintf(stderr, "[%s] - Null globject at index [%zu] in glapi_DestroyApp\n", _FL, i);
                continue;
            }
            switch (clist[i].objtype) {
                case VAO:
                    glDeleteVertexArrays(1, (GLuint*)clist[i].globject);
                    break;
                case SHADER:
                    glDeleteProgram(*(GLuint*)clist[i].globject);
                    break;
                case TEXTURE:
                    glDeleteTextures(1, (GLuint*)clist[i].globject);
                    break;
                case FRAMEBUFFER:
                    glDeleteFramebuffers(1, (GLuint*)clist[i].globject);
                    break;
                default:
                    fprintf(stderr, "[%s] - Invalid globject type [%i] in glapi_DestroyApp\n", _FL, clist[i].objtype);
                    break;
            }
        }
        graphics_resources->openglobjects_objcount = 0;
        free(clist);
    }

heapmeshes:
    if (graphics_resources->heapmeshes) {
        for (size_t i = 0; i < graphics_resources->heapmesh_objcount; i++) {
            if (graphics_resources->heapmeshes[i]->positions)
                free(graphics_resources->heapmeshes[i]->positions);
            if (graphics_resources->heapmeshes[i]->indices)
                free(graphics_resources->heapmeshes[i]->indices);
            if (graphics_resources->heapmeshes[i]->uvs)
                free(graphics_resources->heapmeshes[i]->uvs);
            if (graphics_resources->heapmeshes[i]->normals)
                free(graphics_resources->heapmeshes[i]->normals);
        }
        graphics_resources->heapmesh_objcount = 0;
        free(graphics_resources->heapmeshes);
    }

heaprenderables:
    if (graphics_resources->heaprenderables) {
        for (size_t i = 0; i < graphics_resources->heaprenderable_objcount; i++) {
            if (graphics_resources->heaprenderables[i]->meshes)
                free(graphics_resources->heaprenderables[i]->meshes);
            if (graphics_resources->heaprenderables[i]->shaders)
                free(graphics_resources->heaprenderables[i]->shaders);
            if (graphics_resources->heaprenderables[i]->textures)
                free(graphics_resources->heaprenderables[i]->textures);
            if (graphics_resources->heaprenderables[i]->vaos)
                free(graphics_resources->heaprenderables[i]->vaos);
        }
        graphics_resources->heaprenderable_objcount = 0;
        free(graphics_resources->heaprenderables);
    }

freeappl:
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

void glapi_CreateStackRenderer(gl_app* app, gl_stackrenderer* srenderer, gl_mesh mesh, size_t mcount) {
    app_resources* graphics_resources = (app_resources*)app->resources;
    
}

void glapi_DestroyStackRenderer(gl_app* app, gl_stackrenderer* srenderer) {
    app_resources* graphics_resources = (app_resources*)app->resources;
}

void glapi_DrawStackRenderer(gl_window* window, gl_stackrenderer* srenderer) {

}

void glapi_CreateHeapRenderer(gl_app* app, gl_heaprenderer* hrenderer, gl_mesh* meshes, size_t mcount) {
    app_resources* graphics_resources = (app_resources*)app->resources;
}

void glapi_DestroyHeapRenderer(gl_app* app, gl_heaprenderer* hrenderer) {
    app_resources* graphics_resources = (app_resources*)app->resources;
}

void glapi_DrawHeapRenderer(gl_window* window, gl_heaprenderer* hrenderer) {
    
}

void glapi_AppendOpenGLObjects(gl_app* app, globject_tcouple tcouple) {
    app_resources* graphics_resources = (app_resources*)app->resources;
    globject_tcouple* clist = graphics_resources->openglobjects;
    size_t clistlen = graphics_resources->openglobjects_objcount;
    size_t clistsize = graphics_resources->openglobjects_objcapacity;

    if (!clist) {
        clistsize = GL_ADDRESS_ARR_APPEND_AMOUNT;
        clist = (globject_tcouple*)calloc(clistsize, sizeof(globject_tcouple));
        if (!clist) {
            fprintf(stderr, "[%s] - Failure to allocate 'app->resources->openglobjects' to heap in glapi_AppendOpenglObjects\n", _FL); 
            return;
        }
        graphics_resources->openglobjects = clist;
        graphics_resources->openglobjects_objcapacity = clistsize;
    }
    else if (clistlen >= clistsize) {
        clistsize *= 2;
        globject_tcouple* new_clist = (globject_tcouple*)realloc(clist, clistsize * sizeof(globject_tcouple));
        if (!new_clist) {
            fprintf(stderr, "[%s] - Failure to reallocate 'app->resources->openglobjects' in glapi_AppendOpenglObjects\n", _FL); 
            return;
        }
        clist = new_clist;
        graphics_resources->openglobjects = clist;
        graphics_resources->openglobjects_objcapacity = clistsize;
    }
    clist[clistlen] = tcouple;
    graphics_resources->openglobjects_objcount = clistlen + 1;
}

API void glapi_AppendHeapMeshes(gl_app* app, gl_mesh* mesh) {
    app_resources* graphics_resources = (app_resources*)app->resources;
    gl_mesh** clist = graphics_resources->heapmeshes;
    size_t clistlen = graphics_resources->heapmesh_objcount;
    size_t clistsize = graphics_resources->heapmesh_objcapacity;

    if (!clist) {
        clistsize = GL_ADDRESS_ARR_APPEND_AMOUNT;
        clist = (gl_mesh**)calloc(clistsize, sizeof(gl_mesh*));
        if (!clist) {
            fprintf(stderr, "[%s] - Failure to allocate 'app->resources->heapmeshes' to heap in glapi_AppendHeapMeshes\n", _FL); 
            return;
        }
        graphics_resources->heapmeshes = clist;
        graphics_resources->heapmesh_objcapacity = clistsize;
    }
    else if (clistlen >= clistsize) {
        clistsize *= 2;
        gl_mesh** new_clist = (gl_mesh**)realloc(clist, clistsize * sizeof(gl_mesh*));
        if (!new_clist) {
            fprintf(stderr, "[%s] - Failure to reallocate 'app->resources->heapmeshes' in glapi_AppendHeapMeshes\n", _FL); 
            return;
        }
        clist = new_clist;
        graphics_resources->heapmeshes = clist;
        graphics_resources->heapmesh_objcapacity = clistsize;
    }
    clist[clistlen] = mesh;
    graphics_resources->heapmesh_objcount = clistlen + 1;
}

API void glapi_AppendHeapRenderer(gl_app* app, gl_heaprenderer* renderer){
    app_resources* graphics_resources = (app_resources*)app->resources;
    gl_heaprenderer** clist = graphics_resources->heaprenderables;
    size_t clistlen = graphics_resources->heaprenderable_objcount;
    size_t clistsize = graphics_resources->heaprenderable_objcapacity;

    if (!clist) {
        clistsize = GL_ADDRESS_ARR_APPEND_AMOUNT;
        clist = (gl_heaprenderer**)calloc(clistsize, sizeof(gl_heaprenderer*));
        if (!clist) {
            fprintf(stderr, "[%s] - Failure to allocate 'app->resources->heaprenderables' to heap in glapi_AppendHeapRenderer\n", _FL); 
            return;
        }
        graphics_resources->heaprenderables = clist;
        graphics_resources->heaprenderable_objcapacity = clistsize;
    }
    else if (clistlen >= clistsize) {
        clistsize *= 2;
        gl_heaprenderer** new_clist = (gl_heaprenderer**)realloc(clist, clistsize * sizeof(gl_heaprenderer*));
        if (!new_clist) {
            fprintf(stderr, "[%s] - Failure to reallocate 'app->resources->heaprenderables' in glapi_AppendHeapRenderer\n", _FL); 
            return;
        }
        clist = new_clist;
        graphics_resources->heaprenderables = clist;
        graphics_resources->heaprenderable_objcapacity = clistsize;
    }
    clist[clistlen] = renderer;
    graphics_resources->heaprenderable_objcount = clistlen + 1;
}

APIC char* load_raw_txt(const char* fpath) {
    FILE* file = fopen(fpath, "r");
    if (!file) {
        fprintf(stderr, "[%s] - Failure to read file in 'load_raw_txt(const char* fpath)': %s\n", _FL, fpath);
        return NULL;
    }  
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char* source = (char*)malloc(size + 1);
    if (!source) {
        fprintf(stderr, "[%s] - Memory allocation failed for file in 'load_raw_txt(const char* fpath)': %s\n", _FL, fpath);
        fclose(file);
        return NULL;
    }
    size_t read = fread(source, 1, size, file);
    if (read != size) {
        fprintf(stderr, "[%s] - Failure to read file in 'load_raw_txt(const char* fpath)': %s\n", _FL, fpath);
        free(source);
        fclose(file);
        return NULL;
    }
    source[size] = '\0';
    fclose(file);
    return source;
}

APIC GLuint compile_shader_code(const char* source, GLenum type) {
    GLuint shaderp = glCreateShader(type);
    glShaderSource(shaderp, 1, &source, NULL);
    glCompileShader(shaderp);
    int errcode;
    glGetShaderiv(shaderp, GL_COMPILE_STATUS, &errcode);
    if (!errcode) {
        char log[512];
        glGetShaderInfoLog(shaderp, 512, NULL, log);
        fprintf(stderr, "[%s] - Shader compilation error: %s\n", _FL, log);
    }
    return shaderp;
}

APIC GLuint gen_shader_program(gl_app* app, const char* v_fpath, const char* f_fpath, GLuint* address) {
    char* vsource = load_raw_txt(v_fpath);
    char* fsource = load_raw_txt(f_fpath);
    GLuint vshader = compile_shader_code(vsource, GL_VERTEX_SHADER);
    GLuint fshader = compile_shader_code(fsource, GL_FRAGMENT_SHADER);
    GLuint sprogram = glCreateProgram();
    glAttachShader(sprogram, vshader);
    glAttachShader(sprogram, fshader);
    int errcode;
    glGetProgramiv(sprogram, GL_LINK_STATUS, &errcode);
    if (!errcode) {
        char log[512];
        glGetProgramInfoLog(sprogram, 512, NULL, log);
        fprintf(stderr, "[%s] - Shader linking error: %s\n", _FL, log);
    }
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    free(vsource);
    free(fsource);
    glapi_AppendOpenGLObjects(app, T{address, SHADER});
    return sprogram;
}

APIC GLuint gen_vertex_buffer_object_from_mesh(gl_app* app, gl_mesh* mesh, GLuint* address) {
    
    glapi_AppendOpenGLObjects(app, T{address, VAO});
    return 0;
}

APIC GLuint gen_texture_from_fpath(gl_app* app, const char* fpath, GLuint* address) {
    glapi_AppendOpenGLObjects(app, T{address, TEXTURE});
    return 0;
}
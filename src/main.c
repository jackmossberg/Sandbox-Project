#include "graphics.h"
#include "maths.h"

int main() {
    gl_app* app = glapi_CreateApp(900, 800, "window", true, 0.04, 0.04, 0.17);
    
    GLfloat positions[] = {
        0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    GLuint indices[] = {0, 1, 2};

    gl_mesh mesh = {
        .positions = positions,
        .positions_size = sizeof(positions),
        .indices = indices,
        .indices_size = sizeof(indices),
        .uvs = NULL,
        .uvs_size = 0,
        .normals = NULL,
        .normals_size = 0
    };

    gl_vao vao = glapi_GenVertexBufferObjectFromMesh(app, &mesh, &vao);
    gl_shader shader = glapi_GenShaderProgram_s(app, "default.vert", "default.frag", &shader);

    matrix4x4 transform_matrix = MAT4_INIT
    matrix4x4 view_matrix = MAT4_INIT
    matrix4x4 projection_matrix = MAT4_INIT

    while (!glapi_ShouldAppClose(app)) {
        glapi_BindApp(app);
        glapi_BindShader(&shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, "transform_matrix"), 1, GL_FALSE, (float*)transform_matrix);
        glUniformMatrix4fv(glGetUniformLocation(shader, "view_matrix"), 1, GL_FALSE, (float*)view_matrix);
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection_matrix"), 1, GL_FALSE, (float*)projection_matrix);
        glapi_BindVertexBufferObject(&vao);
        glapi_DrawVertexBufferObject(mesh.indices_size);
        glapi_UnbindVertexBufferObject();
        glapi_UnbindShader();

        glapi_UnbindApp(app);
    }

    glapi_DestroyApp(app);
    return 0;
}
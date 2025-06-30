#version 330 core
layout(location = 0) in vec3 position;
out vec3 i_position;
uniform mat4 transform_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
void main() {
    i_position = position;
    gl_Position = projection_matrix * transform_matrix * view_matrix *vec4(position, 1.0f);
}
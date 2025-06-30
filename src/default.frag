#version 330 core
out vec4 final_color;
in vec3 i_position;
void main() {
    final_color = vec4(i_position.x, i_position.y, 1.0f, 1.0f);
}
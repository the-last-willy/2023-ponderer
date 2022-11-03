#version 460

layout(location = 1) in vec3 a_color;
layout(location = 0) in vec3 a_position;

out vec3 v_color;

layout(std430, binding = 0) readonly buffer camera {
    mat4 global_to_view;
    mat4 view_to_clip;
};

layout(std430, binding = 1) readonly buffer model {
  mat4 local_to_global;
};

void main() {
    v_color = a_color;

    gl_Position = view_to_clip * global_to_view * vec4(a_position, 1.);
}

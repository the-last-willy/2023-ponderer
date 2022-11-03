#version 450

struct Camera {
    mat4 world_to_view;
    mat4 view_to_clip;
};

struct Billboard {
    vec4 horizontal_axis;
    vec4 vertical_axis;

    vec4 position;

    vec4 color;

    float scale;
};

layout(std430, binding = 0) readonly buffer camera {
    mat4 world_to_view;
    mat4 view_to_clip;
};

layout(std140, binding = 1) readonly buffer billboard_buffer {
    Billboard billboards[];
};

layout(location = 0) in vec3 a_position;

out vec4 v2f_color;
out vec2 v2f_position;

void main() {
    v2f_color = billboards[gl_InstanceID].color;
    v2f_position = a_position.xy;

    vec4 world_position = billboards[gl_InstanceID].position;
    vec4 view_position = world_to_view * world_position + vec4(a_position / 600., 1.);
    // vec4 view_position = world_to_view * world_position + vec4(a_position, 1.);
    gl_Position = view_to_clip * view_position;
}
#version 450

struct Camera {
    mat4 world_to_view;
    mat4 view_to_clip;
};

struct Billboard {
    vec4 horizontal_axis;
    vec4 vertical_axis;

    vec4 position;

    float scale;
};

layout(std430, binding = 0) readonly buffer billboard_buffer {
    Billboard billboards[];
};

void main() {

}
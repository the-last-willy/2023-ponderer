#version 450

struct LightDirection {
    vec4 view_direction;
};

layout(std430, binding = 2) readonly buffer light_direction_buffer {
    LightDirection light_direction;
};

in vec4 v2f_color;
in vec2 v2f_position;

layout(location = 0) out vec4 f_color;

void main() {
    float d = length(v2f_position);
    if(d > 1.) {
        discard;
    }
    float z = sqrt(1. - d * d);
    vec3 normal = vec3(v2f_position, z);

    float diffuse_lighting = dot(normal, -light_direction.view_direction.xyz) * .5 + .5;

    gl_FragDepth = gl_FragCoord.z - z / 600.f;

    f_color = vec4(v2f_color.rgb * diffuse_lighting, 1.);
    f_color = vec4(v2f_color.rgb, 1.);
    // f_color = vec4(normal * .5 + .5, 1.);
}

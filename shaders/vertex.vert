#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 normal;

out VS_OUT
{
    float diffuse;
    vec2 uv;
    vec3 view_dir;
    vec3 reflection;

    vec3 normal;
} vs_out;

uniform mat4 model_matrix;
uniform mat4 vp_matrix;

uniform vec3 world_light_position;
uniform vec3 world_camera_position;


void main(void)
{
    vec4 world_position = model_matrix * position;

    vec3 light_dir = world_light_position - world_position.xyz;
    light_dir = normalize(light_dir);

    vec3 view_dir = world_position.xyz - world_camera_position;

    gl_Position = vp_matrix * world_position;

    vec3 world_normal = normalize(mat3x3(model_matrix) * normal.xyz);

    vs_out.diffuse = dot(light_dir, world_normal.xyz);
    vs_out.uv = uv;
    vs_out.view_dir = normalize(view_dir);
    vs_out.reflection = reflect(-light_dir, world_normal);

    vs_out.normal = world_normal;
}

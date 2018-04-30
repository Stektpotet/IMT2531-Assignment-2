#prop ZTest on
#prop Cull On Back
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;


uniform mat4 m2w;
uniform float time = 0;

layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};

out vec4 pos;
out vec2 texCoord;
out vec3 fragVert;
out vec3 fragNormal;

vec4 MVP(in vec4 position) {
    return projection * view * m2w * position;
}

void main() {
    texCoord = uv;
    fragNormal = mat3(transpose(inverse(m2w))) * normal;
    gl_Position = MVP(position);
    fragVert = vec3(m2w * position);
}

#shader fragment
#version 140

#define MAX_LIGHTS 8

in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 out_color;

uniform float time = 0;

uniform sampler2D mainTex;

uniform float opacity = 1;
uniform float specularity = 1;
uniform float intensity = 1;
uniform float bumpiness = 1;
uniform float distortion = 1;
uniform float scatterPower = 1;
uniform float scatterScale = 1;
uniform float scatterThickness = 1;

uniform mat4 m2w;

layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};

void main() {


    vec3 diff = texture(mainTex, texCoord).rgb;

    out_color = vec4(diff, 1);
}

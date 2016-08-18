#version 400

layout(location = 0) in vec3 vertex;
layout(location = 1) in highp vec3 iColor;

out highp vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    color = iColor;

    gl_Position = projection * view * model * vec4(vertex, 1.0f);
}

#version 400

layout(location = 0) in vec2 vertex;
layout(location = 1) in highp vec3 iColor;

out highp vec3 color;

uniform mat4 mvp;

void main()
{
    color = iColor;

    gl_Position = mvp * vec4(vertex, 0.0f, 1.0f);
}

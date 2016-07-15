#version 400

in highp vec3 color;
out highp vec4 fragColor;

void main()
{
    fragColor = vec4(color, 1.0f);
}

#version 330 core
out vec4 fragColor;

uniform vec4 objColor;
uniform vec4 lightColor;

void main()
{
    fragColor = vec4(lightColor * objColor);
}
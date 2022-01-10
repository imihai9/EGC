#version 330

// Input
// Getting values from vertex shader
in vec3 fragColor;

// Output
layout(location = 0) out vec4 out_color;
uniform float time;

void main()
{
    // Write pixel out color
    out_color = vec4(fragColor, 1);
}

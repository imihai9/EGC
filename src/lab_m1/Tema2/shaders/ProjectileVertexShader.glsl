#version 330

// Input
// Getting vertex attributes from each location

// 0, 1, 2, 3 - pipes
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texCoords;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
out vec3 fragColor; // pasam culoarea -> vertex shader (varyings)

void main()
{
    // Send output to fragment shader
    fragColor = glm::vec3(0.28f, 0.46f, 0.45f);
    // Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position, 1);
}

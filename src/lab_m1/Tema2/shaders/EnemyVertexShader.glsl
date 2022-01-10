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
uniform float time;
// Output
out vec3 fragColor; // pasam culoarea -> fragment shader (varyings)

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    // Send output to fragment shader
    fragColor = glm::vec3(0.54f, 0.f, 0.f); // v_color;
    // Compute gl_Position

    // move the position along the normal and transform it
    float rnd = rand(vec2(v_position.x, v_position.z));
    vec3 newPosition = v_position + v_normal * rnd * rnd * 1.5f * time;
    gl_Position = Projection * View * Model * vec4(newPosition, 1);
}

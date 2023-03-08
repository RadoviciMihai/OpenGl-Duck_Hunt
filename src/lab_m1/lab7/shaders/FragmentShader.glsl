#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 color;
in vec3 world_pos;
in vec3 world_normal;


// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(color,1);
}
    
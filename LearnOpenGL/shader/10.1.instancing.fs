#version 420 core // note the GLSL version!
out vec4 FragColor;

in vec3 fColor;

void main()
{             
    FragColor = vec4(fColor, 1.0);
}  
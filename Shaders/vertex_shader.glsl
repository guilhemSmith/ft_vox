#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in int aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 norm;
out vec2 texcoords;
flat out int tex;

void main()
{
	norm = vec4(aNorm, 1.0);
	gl_Position =  projection * view * model * vec4(aPos, 1.0);
    texcoords = vec2(aTexCoords.x, aTexCoords.y);
    
    tex = aTex;
}
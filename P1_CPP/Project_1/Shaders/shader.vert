#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float textureRotation;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vec2 centeredTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y) - vec2(0.5);
	float c = cos(textureRotation);
	float s = sin(textureRotation);
	TexCoord = mat2(c, -s, s, c) * centeredTexCoord + vec2(0.5);
}
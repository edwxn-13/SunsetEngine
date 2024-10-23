#version 330 core

in vec2 tex;

uniform sampler2D Texture;

out vec4 fragColour;

void main()
{
	fragColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

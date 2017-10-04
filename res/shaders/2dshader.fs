#version 400 core

out vec4 out_Color;

in vec3 color;
in vec2 texCoords;

uniform sampler2D diffuse;

void main(){
	//out_Color = vec4(texture(diffuse, texCoords).rgb, 1.0);
	out_Color = vec4(color, 1.0);
}
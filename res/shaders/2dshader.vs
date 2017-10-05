#version 400 core

in vec3 in_Vertex;
in vec2 in_TexCoord;

out vec3 color;
out vec2 texCoords;

uniform mat4 pMatrix;
uniform mat4 vMatrix;

void main(){
	gl_Position = pMatrix * vMatrix * vec4(in_Vertex, 1.0);
	color = in_Vertex * 0.5 + 0.5;
	//texCoords = in_Vertex.xz * 0.5 + 0.5;
	texCoords = in_TexCoord;
}

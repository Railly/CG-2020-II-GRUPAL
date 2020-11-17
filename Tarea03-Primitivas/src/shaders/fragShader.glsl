#version 330 core

out vec4 fColor;
uniform vec3 u_color; 

void main() {  
	fColor = vec4(u_color,1.0);
}
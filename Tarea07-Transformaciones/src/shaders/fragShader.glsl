#version 330 core
#define PI 3.14159265359

out vec4 fColor;
uniform vec3 u_color;
uniform float u_time;


void main() {
	fColor =  vec4(u_color,1.0);
}
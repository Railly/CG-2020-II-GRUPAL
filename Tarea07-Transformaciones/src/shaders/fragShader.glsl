#version 430
#define PI 3.14159265359

out vec4 fColor;
in vec2 tc;
uniform vec3 u_color;
uniform float u_time;

layout(binding = 0) uniform sampler2D samp;

void main() {
	fColor =  vec4(u_color,1.0);
	fColor += texture(samp, tc);
}
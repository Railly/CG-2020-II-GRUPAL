#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float u_time;

mat4 buildRotateX(float rad) {
	mat4 rotX = mat4 (1.0,    0.0,       0.0,   0.0,
					  0.0,  cos(rad), -sin(rad), 0.0,
					  0.0,  sin(rad),  cos(rad), 0.0,
					  0.0,    0.0,       0.0,   1.0 );
	return rotX;
}

mat4 buildScale(float x, float y, float z) {
	 mat4 scale = mat4(x, 0.0, 0.0, 0.0,
					   0.0, y, 0.0, 0.0,
					   0.0, 0.0, z, 0.0,
					   0.0, 0.0, 0.0, 1.0 );
	return scale;
}

void main() {
	float i = sin(u_time)*2 - 1.5;
	mat4 localScale = buildScale(1.2 *i, 1.2 * i, 1.2 * i);
	mat4 newM_matrix = m_matrix *localScale;
	mat4 mv_matrix = v_matrix * newM_matrix;
	gl_Position =  mv_matrix * vec4(position, 1.0f);
}
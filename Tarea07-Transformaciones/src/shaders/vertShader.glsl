#version 330                                                                  
                                                                              
layout (location = 0) in vec3 pos;								  
                                                                              
uniform mat4 u_mv; 
uniform float u_time;
const float toRadians = 3.14159265f / 180.0f;

// funtions to Transformations(Rotate, Translate, Scale)
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);
mat4 buildScale(float sx, float sy, float sz);

// Global Variables
const float radius = 0.2;
const float radius2 = 0.3;
const float pi = 3.141592653589793;
const float twicePi = 2*pi;
const float numberOfSides = 12; //Numero de punto donde ser trasladan los triangulos

void main() {
    if(gl_InstanceID == 1) {
        float i = gl_InstanceID;
        float x_vertex = 0.5 + radius * cos(i * twicePi / numberOfSides);
        float y_vertex = 0.5 + radius * sin(i * twicePi / numberOfSides);

        mat4 localRotaZ = buildRotateZ(u_time * 30 * toRadians);
        mat4 localTrans = buildTranslate(x_vertex + 0.4, y_vertex - 2.0, 0);
        localTrans += buildTranslate(-cos(u_time / 23.f) * 1.6 - 1.0, 0 , 0); //Translate in x and y axis.
        mat4 localScal = buildScale(6 * cos(u_time/46 + 1.6), 6 * cos(u_time/46 + 1.6), 0); //Scale in 10% y 6%.
        gl_Position =  localTrans * localRotaZ * localScal * vec4(pos, 1.0);  // right-to-left

    } else if (gl_InstanceID == 2){
        float i = gl_InstanceID;
        float x_vertex = 0.5 + radius2 * cos(i * twicePi / numberOfSides);
        float y_vertex = 0.5 + radius2 * sin(i * twicePi / numberOfSides);

        mat4 localRotaZ = buildRotateZ(u_time * 30 * toRadians);
        mat4 localTrans = buildTranslate(x_vertex + 0.42, y_vertex - 2.16, 0);
        localTrans += buildTranslate(-cos(u_time / 23.f) * 1.6 - 1.0, 0 , 0); //Translate in x and y axis.
        mat4 localScal = buildScale(8.5 * cos(u_time/46 + 1.6), 8.5 * cos(u_time/46 + 1.6), 0); //Scale in 10% y 6%.
        gl_Position =  localTrans * localRotaZ * localScal * vec4(pos, 1.0);  // right-to-left

    }  else {
        gl_Position = u_mv * vec4(pos, 1.0);
    }
}
// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad) {
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                     sin(rad), cos(rad),  0.0, 0.0,
                     0.0,      0.0,       1.0, 0.0,
                     0.0,      0.0,       0.0, 1.0);
    return zrot;
}

// builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z) {
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      x,   y,   z,   1.0);
    return trans;
}

// builds and returns a Scale matrix
mat4 buildScale(float sx, float sy, float sz) {
    mat4 scale = mat4( sx, 0.0, 0.0, 0.0,
                      0.0,  sy, 0.0, 0.0,
                      0.0, 0.0,  sz, 0.0,
                      0.0, 0.0, 0.0, 1.0);
    return scale;
}
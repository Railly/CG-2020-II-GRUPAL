//============================================================================
// Name        :	Mouse
// Student     :	Hugo Quispe, Railly Angelo
// Version     :
// Description :
//============================================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "Utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846
#define HEIGHT 1000
#define WIDTH 1000
#define numVAOs 1
#define numVBOs 12

/*--------------- VARIABLES GLOBALES --------------- */
const float toRadians = 3.14159265f / 180.0f;
GLuint renderingProgram;

GLfloat radius = 1;
GLfloat radius2 = 0.02;
const GLint numberOfSides = 50;
const GLint numberOfVertices = numberOfSides + 2;
const GLint numberOfSides2 = 8;
const GLint numberOfVertices2 = numberOfSides + 2;
//GLfloat m_Vertices3[dimVertices2*2];
GLfloat twicePi = 2.0f * M_PI;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

float curAngle = 192.0f;
float curAngle2 = 90.0f;
float curAngle3 = 0.0f;
float curAngleS = 180.0f;
float curAngleM = 180.0f;
float offset = 0.1f;
float increment = 0.105f;
int pendulo = 0;

// location of triangle on x axis
float x = 0.0f;
// offset for moving the triangle
float inc = 0.01f;

void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("shaders/vertShader.glsl", "shaders/fragShader.glsl");

	//Circulo Normal
	GLfloat verticesX[numberOfVertices];
	GLfloat verticesY[numberOfVertices];
	GLfloat verticesZ[numberOfVertices];

	verticesX[0] = 0;
	verticesY[0] = 0;
	verticesZ[0] = 0;

	for (int i = 1; i < numberOfVertices; i++) {
		verticesX[i] = (radius * cos((i-1) * twicePi / (numberOfSides) + twicePi/4));
		verticesY[i] = (radius * sin((i-1) * twicePi / (numberOfSides) + twicePi/4));
		verticesZ[i] = 0;
	}

	GLfloat vPositions[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++) {
		vPositions[i * 3] = verticesX[i];
		vPositions[i * 3 + 1] = verticesY[i];
		vPositions[i * 3 + 2] = verticesZ[i];
	}

	//Circulo Pequeño?
	GLfloat verticesX2[numberOfVertices2];
	GLfloat verticesY2[numberOfVertices2];
	GLfloat verticesZ2[numberOfVertices2];

	verticesX2[0] = 0;
	verticesY2[0] = 0;
	verticesZ2[0] = 0;

	for (int i = 1; i < numberOfVertices2; i++) {
		verticesX2[i] = (radius2 * cos((i-1) * twicePi / (numberOfSides2) + twicePi/4));
		verticesY2[i] = (radius2 * sin((i-1) * twicePi / (numberOfSides2) + twicePi/4));
		verticesZ2[i] = 0;
	}

	GLfloat vPositions2[(numberOfVertices2) * 3];

	for (int i = 0; i < numberOfVertices2; i++) {
		vPositions2[i * 3] = verticesX2[i];
		vPositions2[i * 3 + 1] = verticesY2[i];
		vPositions2[i * 3 + 2] = verticesZ2[i];
	}


	float vSegundero[27] = {
		-0.01f, 0.0f, 1.0f,
		 0.01f, 0.0f, 1.0f,
		-0.035f, -0.8f, 1.0f,
		-0.035f, -0.8f, 1.0f,
		 0.01f, 0.0f, 1.0f,
		 0.035f, -0.8f, 1.0f,
		-0.09f, -0.8f, 1.0f,
		 0.09f, -0.8f, 1.0f,
		 0.0f, -0.9f, 1.0f
	};

	float vMinutero[27] = {
		-0.01f, 0.0f, 1.0f,
		 0.01f, 0.0f, 1.0f,
		-0.035f, -0.5f, 1.0f,
		-0.035f, -0.5f, 1.0f,
		 0.01f, 0.0f, 1.0f,
		 0.035f, -0.5f, 1.0f,
		-0.09f, -0.5f, 1.0f,
		 0.09f, -0.5f, 1.0f,
		 0.0f, -0.6f, 1.0f
	};

	float vPendulo[18] = {
		-0.04f, 0.0f, 1.0f,
		 0.04f, 0.0f, 1.0f,
		-0.04f, -0.6f, 1.0f,
		-0.04f, -0.6f, 1.0f,
		 0.04f, 0.0f, 1.0f,
		 0.04f, -0.6f, 1.0f
	};

	float vBGRectangle[18] = {
		-1.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		 1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f
	};

	float vCorbata[27] = {
		-0.5f, 0.0f, 1.0f,
		0.5f, 0.0f, 1.0f,
		-0.7f, -0.5f, 1.0f,
		-0.5f, 0.0f, 1.0f,
		0.5f, 0.0f, 1.0f,
		0.7f, -0.5f, 1.0f,
		-0.5f, 0.0f, 1.0f,
		0.5f, 0.0f, 1.0f,
		0.0f, -0.5f, 1.0f
	};

	float vBrazoIzquierdo[27] = {
		0.0f, 0.0f, 1.0f,
		-0.1f, -0.3f, 1.0f,
		-0.3f, -0.3f, 1.0f,
		-0.1f, -0.3f, 1.0f,
		-0.3f, -0.3f, 1.0f,
		0.0f, -0.6f, 1.0f,
		0.0f, -0.6f, 1.0f,
		-0.1f, -0.5f, 1.0f,
		-0.2f, -0.7f, 1.0f
	};

	float vBrazoDerecho[27] = {
		0.0f, 0.0f, 1.0f,
		0.1f, -0.3f, 1.0f,
		0.3f, -0.3f, 1.0f,
		0.1f, -0.3f, 1.0f,
		0.3f, -0.3f, 1.0f,
		0.0f, -0.6f, 1.0f,
		0.0f, -0.6f, 1.0f,
		0.1f, -0.5f, 1.0f,
		0.2f, -0.7f, 1.0f
	};

	float vBasePies[18] = {
		-0.3, 0.0f, 1.0f,
		0.3, 0.0f, 1.0f,
		-0.4f, -0.3f, 1.0f,
		0.3, 0.0f, 1.0f,
		-0.4f, -0.3f, 1.0f,
		0.4f, -0.3f, 1.0f
	};

	float vPieIzquierdo[18] = {
		-0.3, 0.0f, 1.0f,
		0.5, 0.0f, 1.0f,
		-0.5f, -0.5f, 1.0f,
		0.5, 0.0f, 1.0f,
		-0.5f, -0.5f, 1.0f,
		0.3f, -0.5f, 1.0f
	};

	float vPieDerecho[18] = {
		-0.7, 0.0f, 1.0f,
		0.5, 0.0f, 1.0f,
		-0.5f, -0.5f, 1.0f,
		0.5, 0.0f, 1.0f,
		-0.5f, -0.5f, 1.0f,
		0.7f, -0.5f, 1.0f
	};

	float vTriangulo[9] {
		-0.02f, -0.02f, 0.0f,
		 0.02f, -0.02f, 0.0f,
		 0.0f,  0.2f, 0.0f
	};
	
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vSegundero), vSegundero, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vMinutero), vMinutero, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPendulo), vPendulo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vBGRectangle), vBGRectangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vCorbata), vCorbata, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vBrazoDerecho), vBrazoDerecho, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vBrazoIzquierdo), vBrazoIzquierdo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vBasePies), vBasePies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPieIzquierdo), vPieIzquierdo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPieDerecho), vPieDerecho, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions2), vPositions2, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(88./255.0f, 73./255.0f, 49./255.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);
	glBindVertexArray(vao[0]);

	// se obtienen los uniforms del vertex y el fragment shader
	GLuint uniformColor = glGetUniformLocation(renderingProgram, "u_color");
	GLuint uniformModelView = glGetUniformLocation(renderingProgram, "u_mv");
	//time
    GLuint uniform_time = glGetUniformLocation(renderingProgram, "u_time");
    
    glUniform1f(uniform_time,(float)currentTime);

	curAngle -= increment;

	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.62f ,0.0f));
	glm::mat4 model(1.0f);

	//Circulo manejado en vertex shader - BORDE
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 mv = view * model;

	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glUniform3f(uniformColor, 243./255.0f, 183./255.0f, 7./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, numberOfVertices2, 3);
	
	//Circulo manejado en vertex shader
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	mv = view * model;

	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glUniform3f(uniformColor, 245./255.0f, 252./255.0f, 237./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, numberOfVertices2, 2);

	//Borde de la cabeza
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.23f,0.23f,1.0f));
	model = glm::rotate(model, curAngle * toRadians,glm::vec3(0,0,1));
	model = glm::translate(model,glm::vec3(0,0,0)); //position = 0,0,0
	mv = view * model;
	
	glUniform3f(uniformColor, 186./255.0f, 70./255.0f, 32./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Pie Izquierdo - Borde
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.3f, 0.5f,1.0f));
	model = glm::translate(model, glm::vec3(-0.52f, -1.6f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 237./255.0f, 191./255.0f, 71./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	//Pie Izquierdo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.3f, 0.5f,1.0f));
	model = glm::translate(model, glm::vec3(-0.5f, -1.55f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 186./255.0f, 70./255.0f, 32./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Pie Derecho Borde
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.2f, 0.5f,1.0f));
	model = glm::translate(model, glm::vec3(0.82f, -1.6f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 237./255.0f, 191./255.0f, 71./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Pie Derecho
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.2f, 0.5f,1.0f));
	model = glm::translate(model, glm::vec3(0.8f, -1.55f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 186./255.0f, 70./255.0f, 32./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Base de los pies - Borde
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.835f,0.7f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.05f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 237./255.0f, 191./255.0f, 71./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Base de los pies
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.8f,0.7f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 158./255.0f, 42./255.0f, 21./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Cuerpo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(1.0f,0.8f,1.0f));
	model = glm::rotate(model, 159.0f * toRadians,glm::vec3(0,0,1));
	model = glm::translate(model,glm::vec3(0,0,0)); //position = 0,0,0

	mv = view * model;
	
	glUniform3f(uniformColor, 186./255.0f, 70./255.0f, 32./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices/6);

	//Cuerpo Fondo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.7f,0.6f,1.0f));
	model = glm::rotate(model, 159.0f * toRadians,glm::vec3(0,0,1));
	model = glm::translate(model,glm::vec3(-0.08f, 0.2f,0.0f)); //position = 0,0,0

	mv = view * model;
	
	glUniform3f(uniformColor, 89./255.0f, 44./255.0f, 35./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices/6);

	//Brazo Izquierdo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.39f, -0.5f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 212./255.0f, 145./255.0f, 10./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 9);

	//Brazo Derecho
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	model = glm::translate(model, glm::vec3(0.41f, -0.5f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 212./255.0f, 145./255.0f, 10./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 9);

	//Hombro Izquierdo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.09f,0.09f,1.0f));
	model = glm::translate(model, glm::vec3(-2.3f, -2.8f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 237./255.0f, 191./255.0f, 71./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Hombro Derecho
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.09f,0.09f,1.0f));
	model = glm::translate(model, glm::vec3(2.3f, -2.8f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 237./255.0f, 191./255.0f, 71./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	
	//Cabeza
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.21f,0.21f,1.0f));
	model = glm::rotate(model, curAngle * toRadians,glm::vec3(0,0,1));//rotation x = 0.0 degrees
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 247./255.0f, 179./255.0f, 148./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Oreja Izquierda
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.07f,0.07f,1.0f));
	model = glm::translate(model, glm::vec3(-3.0f, 2.8f, 0.0f));
	mv = view * model;
	
	glUniform3f(uniformColor, 186./255.0f, 70./255.0f, 32./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Oreja Derecha
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.07f,0.07f,1.0f));
	model = glm::translate(model, glm::vec3(3.0f, 2.8f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 186./255.0f, 70./255.0f, 32./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Corona??
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(3.0f,3.0f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.095f, 0.0f));

	mv = view * model;

	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glUniform3f(uniformColor, 237./255.0f, 191./255.0f, 71./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Ojo Izquierdo parte blanca 
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.04f,0.05f,1.0f));
	model = glm::translate(model, glm::vec3(-1.8f, 1.0f, 0.0f));

	mv = view * model;

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glUniform3f(uniformColor, 250./255.0f, 244./255.0f, 240./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Ojo Izquierdo pupila 
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.02f,0.03f,1.0f));
	model = glm::translate(model, glm::vec3(-3.6f - sin(currentTime * 3.15), 1.5f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 20./255.0f, 58./255.0f, 122./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Ojo Derecho parte blanca 
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.04f,0.05f,1.0f));
	model = glm::translate(model, glm::vec3(1.8f, 1.0f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 250./255.0f, 244./255.0f, 240./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Ojo Derecho pupila 
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.02f,0.03f,1.0f));
	model = glm::translate(model, glm::vec3(3.6f - sin(currentTime * 3.15), 1.5f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 20./255.0f, 58./255.0f, 122./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Nariz
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.025f,0.025f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 212./255.0f, 145./255.0f, 10./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Segundero
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.20f,0.20f,1.0f));
	model = glm::rotate(model, (180 -(float)currentTime * 6) * toRadians,glm::vec3(0,0,1));//rotation x = 0.0 degrees
	//model = glm::rotate(model, curAngleS * -1 * toRadians,glm::vec3(0,0,1));//rotation x = 0.0 degrees
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 180./255.0f, 50./255.0f, 195./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 9);

	//Minutero
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.20f,0.20f,1.0f));
	model = glm::rotate(model, curAngle / 1.055f / 60 * toRadians ,glm::vec3(0,0,1));//rotation x = 0.0 degrees
	//model = glm::rotate(model, curAngleM * (-1) * toRadians,glm::vec3(0,0,1));//rotation x = 0.0 degrees
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 9);

	//Corbata?? fondo marron
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.15f,0.15f,1.0f));
	model = glm::translate(model, glm::vec3(0.05f, -1.39f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 186./255.0f, 70./255.0f, 32./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 9);

	//Corbata(?)
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.12f,0.12f,1.0f));
	model = glm::translate(model, glm::vec3(0.06f, -1.75f, 0.0f));

	mv = view * model;

	glUniform3f(uniformColor, 237./255.0f, 191./255.0f, 71./255.0f);	
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 9);

	//Pendulo Cuerpo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.60f,0.60f,1.0f));
	model = glm::rotate(model, (float)sin(curAngle/2) * 9 * toRadians,glm::vec3(0,0,1));//rotation x = 0.0 degrees
	model = glm::translate(model, glm::vec3(0.0f, -0.35f, 0.0f));

	mv = view * model;
	
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Pendulo Cabeza
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.08f,0.08f,1.0f));
	model = glm::rotate(model, (float)sin(curAngle/2) * 9 * toRadians,glm::vec3(0,0,1));//rotation x = 0.0 degrees
	model = glm::translate(model, glm::vec3(0.0f, -7.8f, 0.0f));

	mv = view * model;

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glUniform3f(uniformColor, 237./255.0f, 191./255.0f, 71./255.0f);
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, glm::value_ptr(mv));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	if(currentTime > 60){
		glfwSetTime(0);
	}
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 	// Resizable option.
	//Antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);  

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Reloj", NULL, NULL);
    glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
    glfwSwapInterval(1);
    
    init(window);
    while (!glfwWindowShouldClose(window)) {
		display(window, (double)glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
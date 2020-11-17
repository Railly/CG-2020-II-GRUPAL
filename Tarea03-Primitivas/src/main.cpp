//============================================================================
// Name        :	Tarea 03- Primitivasa
// Students    :	Hugo Quispe, Railly Angelo
//					Quispe Cadillo, Andrés
//					Romero Yañac, Luis Enrique
// Version     :	Final
// Description :	4 cuadrantes con 4 dibujos usando primitivas
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
/*---------------Defines------------------------------*/
#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846
#define HEIGHT 800
#define WIDTH 800
#define numVAOs 1
#define numVBOs 4

/*--------------- VARIABLES GLOBALES --------------- */
const float toRadians = 3.14159265f / 180.0f;
GLuint renderingProgram;
GLfloat radius = 1;
GLint numberOfSides = 50;
GLint numberOfVertices = numberOfSides + 2;
GLfloat twicePi = 2.0f * M_PI;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

/*---------------Funciones--------------------------*/
void init(GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);

int main(void) {
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//Antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);  

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tarea CV", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("shaders/vertShader.glsl", "shaders/fragShader.glsl");

	/*------------------GENERADOR DE CIRCUNFERENCIA-------------*/
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

	GLfloat circlePositions[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++) {
		circlePositions[i * 3] = verticesX[i];
		circlePositions[i * 3 + 1] = verticesY[i];
		circlePositions[i * 3 + 2] = verticesZ[i];	
	};

	/*------------------VERTICES DE ASPAS DEL SIMBOLO NAZI------------*/
	
	float aspasPositionsA[] = { 
		0.0f, 0.46f, 0.0f,
		0.0f, 0.30f, 0.0f,
		0.46f, 0.46f, 0.0f,
		0.46f, 0.46f, 0.0f,
		0.0f, 0.30f, 0.0f,
		0.46f, 0.30f, 0.0f,

		0.0f, 0.46f, 0.0f,
		0.16f, 0.46f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.16f, 0.46f, 0.0f,
		0.16f, 0.0f, 0.0f
	};
	/*------------------VERTICES DE ASPAS DE LA CHACANA-------------*/
	float aspasPositionsB[] = { 
		0.0f, 0.0f, 0.0f,
		0.0f, 0.16f, 0.0f,
		0.46f, 0.0f, 0.0f,
		0.46f, 0.0f, 0.0f,
		0.0f, 0.16f, 0.0f,
		0.46f, 0.16f, 0.0f,

		0.0f, 0.46f, 0.0f,
		0.16f, 0.46f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.16f, 0.46f, 0.0f,
		0.16f, 0.0f, 0.0f
	};

	/*------------------VERTICES DEL CUADRADO-------------*/
	float quadPositions[] = { 
		0.30f, 0.30f, 0.0f,
		-0.30f, 0.30f, 0.0f,
		0.30f, -0.30f, 0.0f,
		0.30f, -0.30f, 0.0f,
		-0.30f, 0.30f, 0.0f,
		-0.30f,-0.30f, 0.0f,
	};
	/*------------------SE CREA VAO Y VBOS------------*/

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	
	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circlePositions), circlePositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aspasPositionsB), aspasPositionsB, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadPositions), quadPositions, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aspasPositionsA), aspasPositionsA, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(202./255.0f, 202./255.0f, 202./255.0f, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	glBindVertexArray(vao[0]);
	GLuint uniformColor = glGetUniformLocation(renderingProgram, "u_color");
	GLuint uniformModel = glGetUniformLocation(renderingProgram, "u_model");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/***********Chacana*************/
    glViewport(WIDTH * 0.5, 0, WIDTH*0.5, HEIGHT*0.5);
	//cuadrado - fondo
	glm::mat4 model(1.0f);

	model = glm::scale(model, glm::vec3(1.15f, 1.15f,1.0f));

	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Arriba - fondo	
	model = glm::mat4(1.0f);

	model = glm::rotate(model,  0 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 12);
	//Aspa Arriba

	model = glm::mat4(1.0f);

	model = glm::rotate(model,  0 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniform3f(uniformColor, 203./255.0f, 9./255.0f, 53./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Izquierdo - fondo
	model = glm::mat4(1.0f);

	model = glm::rotate(model,  90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Izquierda
	model = glm::mat4(1.0f);
	model = glm::rotate(model,  90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniform3f(uniformColor, 203./255.0f, 9./255.0f, 53./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	
	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Abajo - fondo
	model = glm::mat4(1.0f);

	model = glm::rotate(model,  180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Abajo
	model = glm::mat4(1.0f);
	model = glm::rotate(model,  180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniform3f(uniformColor, 203./255.0f, 9./255.0f, 53./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Derecha - fondo
	model = glm::mat4(1.0f);

	model = glm::rotate(model,  270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Derecha
	model = glm::mat4(1.0f);
	model = glm::rotate(model,  270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniform3f(uniformColor, 203./255.0f, 9./255.0f, 53./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 12);

	//cuadrado
	model = glm::mat4(1.0f);

	glUniform3f(uniformColor, 203./255.0f, 9./255.0f, 53./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Borde negro
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.21f,0.21f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Borde cara
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.18f,0.18f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	
	glUniform3f(uniformColor, 202./255.0f, 202./255.0f, 202./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);


	/****************SIMBOLO NAZI***********/
	glViewport(0, 0, WIDTH*0.5, HEIGHT*0.5);

	//Borde cara
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.65f,0.65f,1.0f));
	model = glm::translate(model, glm::vec3(0.07f, 0.02f, 0.0f));
	
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Borde cara
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.615f,0.615f,1.0f));
	model = glm::translate(model, glm::vec3(0.07f, 0.02f, 0.0f));
	
	glUniform3f(uniformColor, 255./255.0f, 255./255.0f, 255./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);


	//Aspa Arriba
	model = glm::mat4(1.0f);
	model = glm::rotate(model,  45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,1.0f));
	model = glm::translate(model, glm::vec3(-0.05f, -0.1f, 0.0f));
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Izquierda
	model = glm::mat4(1.0f);
	model = glm::rotate(model,  135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,1.0f));
	model = glm::translate(model, glm::vec3(-0.1f, -0.1f, 0.0f));
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Abajo
	model = glm::mat4(1.0f);
	model = glm::rotate(model,  225 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,1.0f));
	model = glm::translate(model, glm::vec3(-0.12f, -0.05f, 0.0f));
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Aspa Derecha
	model = glm::mat4(1.0f);
	model = glm::rotate(model,  315 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,1.0f));
	model = glm::translate(model, glm::vec3(-0.06f, 0.0f, 0.0f));
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 12);

	/*-------------------MOUSE----------------------------------*/
    glViewport(0, HEIGHT*0.5, WIDTH*0.5, HEIGHT*0.5);

	//Borde de la cabeza
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.213f,0.213f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 1.28f, 0.0f));
	
	glUniform3f(uniformColor, 20./255.0f, 25./255.0f, 14./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Cabeza
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.2f,0.2f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 1.36f, 0.0f));
	
	glUniform3f(uniformColor, 136./255.0f, 193./255.0f, 59./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Borde Oreja izquierda
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.088f,0.088f,1.0f));
	model = glm::translate(model, glm::vec3(-1.82f, 5.2f, 0.0f));

	glUniform3f(uniformColor, 20./255.0f, 25./255.0f, 14./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Oreja izquierda
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.078f,0.078f,1.0f));
	model = glm::translate(model, glm::vec3(-2.06f, 5.86f, 0.0f));


	glUniform3f(uniformColor, 136./255.0f, 193./255.0f, 59./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Borde Oreja derecha
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.088f,0.088f,1.0f));
	model = glm::translate(model, glm::vec3(1.82f, 5.2f, 0.0f));

	glUniform3f(uniformColor, 20./255.0f, 25./255.0f, 14./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Oreja derecha
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.078f,0.078f,1.0f));
	model = glm::translate(model, glm::vec3(2.06f, 5.86f, 0.0f));

	glUniform3f(uniformColor, 136./255.0f, 193./255.0f, 59./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	// Cola linea
	model = glm::mat4 (1.0f);

	model = glm::rotate(model,  135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.36f,0.36f,1.0f));
	model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 0.0f));

	glUniform3f(uniformColor, 0./255.0f, 0./255.0f, 0./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices/2 + 1);
	// Cola fondo
	model = glm::mat4 (1.0f);

	model = glm::rotate(model,  135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.34f,0.34f,1.0f));
	model = glm::translate(model, glm::vec3(-1.0f, -0.51f, 0.0f));

	glUniform3f(uniformColor, 202./255.0f, 202./255.0f, 202./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices/2 + 1);

	//Borde del Cuerpo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.363f,0.363f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, -0.815f, 0.0f));
	
	glUniform3f(uniformColor, 30./255.0f, 12./255.0f, 63./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	// Cuerpo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.35f,0.35f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, -0.84f, 0.0f));
	
	glUniform3f(uniformColor, 136./255.0f, 193./255.0f, 59./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
    
/*------------------------------------SMILE------------------------*/
	glViewport(WIDTH*0.5, HEIGHT*0.5, WIDTH*0.5, HEIGHT*0.5);

	//Borde cara
	model = glm::mat4 (1.0f);

	model = glm::scale(model, glm::vec3(0.615f,0.615f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	//Cara
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.6f,0.6f,1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	
	glUniform3f(uniformColor, 230./255.0f, 208./255.0f, 52./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Borde Ojo izquierdo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.095f,0.095f,1.0f));
	model = glm::translate(model, glm::vec3(-2.5f, 1.4f, 1.0f));
	
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Ojo izquierdo
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.08f,0.08f,1.0f));
	model = glm::translate(model, glm::vec3(-2.96f, 1.65f, 1.0f));
	
	glUniform3f(uniformColor, 146./255.0f, 132./255.0f, 35./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Borde Ojo Derecho
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.095f,0.095f,1.0f));
	model = glm::translate(model, glm::vec3(2.5f, 1.4f, 1.0f));

	
	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Ojo Derecho
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.08f,0.08f,1.0f));
	model = glm::translate(model, glm::vec3(2.96f, 1.65f, 1.0f));

	
	glUniform3f(uniformColor, 146./255.0f, 132./255.0f, 35./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	// Sonrisa Linea
	model = glm::mat4 (1.0f);

	model = glm::rotate(model,  122 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.4f,0.4f,1.0f));
	model = glm::translate(model, glm::vec3(0.0, 0.0f, 1.0f));

	glUniform3f(uniformColor, 25./255.0f, 25./255.0f, 25./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices/3 + 1);
	// Sonrisa Fondo
	model = glm::mat4 (1.0f);

	model = glm::rotate(model,  122 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.39f,0.39f,1.0f));
	model = glm::translate(model, glm::vec3(0.03f, -0.02f, 1.0f));

	glUniform3f(uniformColor, 230./255.0f, 208./255.0f, 52./255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices/3 + 1);
}
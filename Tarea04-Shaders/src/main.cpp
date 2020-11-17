#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <math.h>
#include "Utils.hpp"

#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846
#define HEIGHT 800
#define WIDTH 800
#define numVAOs 1
#define numVBOs 2

/*--------------- VARIABLES GLOBALES --------------- */
GLfloat x = WIDTH / 2;
GLfloat y = HEIGHT / 2; 
GLfloat z = 0.0f;
GLfloat radius;
GLint numberOfSides;
GLint numberOfVertices;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint renderingProgram;
GLFWwindow* window;
GLuint mLoc, vLoc, projLoc, factorTime;

glm::mat4 mMat, vMat, pMat, mvMat;
int width, height;
float aspect;
/* ------------------------------------------------- */

/*--------------- FUNCIONES ---------------*/
void init(GLFWwindow *window);
void display(GLFWwindow* window, double currenTime);
void clean();
void setupPolygon(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);
/* ---------------------------------------- */
int main(void) {
	if (!glfwInit()) exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow( WIDTH, HEIGHT, "Tarea 04 - GLSL - HUGO QUISPE, RAILLY", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
    std::cout<<"GL_VERSION: "<<glGetString(GL_VERSION)<<std::endl;
    glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	init(window);

	do {
		glfwPollEvents();
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
	} while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);

	clean();
    exit(EXIT_SUCCESS);
}

void init(GLFWwindow *window) {
	renderingProgram = Utils::createShaderProgram( "./shaders/vertShader.glsl", "./shaders/fragShader.glsl" );
	glUseProgram(renderingProgram);
    setupPolygon(x, y, 0, 1.0f, 5);
}

void setupPolygon(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides) {
	glGenVertexArrays(1, vao);
	glBindVertexArray (vao[0]);

	numberOfVertices = numberOfSides + 2;

	GLfloat polyX[numberOfVertices];
	GLfloat polyY[numberOfVertices];
	GLfloat polyZ[numberOfVertices];
	GLfloat vPositions[(numberOfVertices) * 3];

	polyX[0] = 0.0f;
	polyY[0] = 0.0f;
	polyZ[0] = 0.0f;

	for (int i = 1; i < numberOfVertices; i++) {
        polyX[i] = (radius * cos(2.0f * M_PI / numberOfSides * i));
        polyY[i] = (radius * sin(2.0f * M_PI / numberOfSides * i));
        polyZ[i] = 0.0f;
    }

	for (int i = 0; i < numberOfVertices; i++) {
        vPositions[i * 3] = polyX[i];
        vPositions[( i * 3 ) + 1] = polyY[i];
        vPositions[( i * 3 ) + 2] = polyZ[i];
    }

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW);
}

void display(GLFWwindow* window, double currentTime) {
	glClear( GL_COLOR_BUFFER_BIT );
	glClearColor(25.f/255.f, 25.f/255.f, 25.f/255.f, 0.0f);

	mLoc = glGetUniformLocation(renderingProgram, "m_matrix");
	vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	factorTime = glGetUniformLocation(renderingProgram, "u_time");


	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees


	mMat = glm::rotate(glm::mat4(1.0f), (float)(cos(currentTime) * M_PI), glm::vec3(0.0f, 0.0f, 1.0f));
	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	vMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), 
  		   glm::vec3(0.0f, 0.0f, 0.0f), 
  		   glm::vec3(0.0f, -1.0f, 0.0f));

	mvMat = vMat * mMat;

	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniform1f(factorTime,(float)currentTime);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	glDisableVertexAttribArray(0);	
}

void clean() {
	glDeleteBuffers(numVBOs, vbo);
	glDeleteVertexArrays(numVAOs,  vao);
	glDeleteProgram(renderingProgram);
	glfwTerminate();
}
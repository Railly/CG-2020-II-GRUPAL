//============================================================================
// Name        :	Mouse
// Student     :	Hugo Quispe, Railly Angelo
// Version     :
// Description :
//============================================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "Utils.hpp"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846
#define HEIGHT 800
#define WIDTH 800
#define numVAOs 1
#define numVBOs 2

/*--------------- VARIABLES GLOBALES --------------- */
const float toRadians = 3.14159265f / 180.0f;
GLuint renderingProgram;

GLfloat radius = 1;
GLint const numberOfSides = 50;
GLint const numberOfVertices = numberOfSides + 2;
GLfloat twicePi = 2.0f * M_PI;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

float color_fondo[3] = { 1.0f, 1.0f, 1.0f };
float color_figura[3] = { 136. / 255.0f, 193. / 255.0f, 59. / 255.0f };

float up = 0.0f;
float down = 0.0f;
float left = 0.0f;
float right = 0.0f;

float curAngle = 0.0f;
float offset = 0.1f;
float increment = 0.1f;

void init(GLFWwindow* window) {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	GLfloat verticesX[numberOfVertices];
	GLfloat verticesY[numberOfVertices];
	GLfloat verticesZ[numberOfVertices];

	verticesX[0] = 0;
	verticesY[0] = 0;
	verticesZ[0] = 0;

	for (int i = 1; i < numberOfVertices; i++) {
		verticesX[i] = (radius * cos((i - 1) * twicePi / (numberOfSides)+twicePi / 4));
		verticesY[i] = (radius * sin((i - 1) * twicePi / (numberOfSides)+twicePi / 4));
		verticesZ[i] = 0;
	}

	GLfloat vPositions[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++) {
		vPositions[i * 3] = verticesX[i];
		vPositions[i * 3 + 1] = verticesY[i];
		vPositions[i * 3 + 2] = verticesZ[i];
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
	/*--[FRAME IMGUI]-----------*/
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(WIDTH/2, HEIGHT/2));
	ImGui::SetNextWindowPos(ImVec2(400, 0));
	/*--[FRAME IMGUI]-----------*/

	/*--[CODIGO IMGUI]-----------*/

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 1.0f;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoCollapse;


	ImGui::Begin("Panel de Control");
		ImGui::Text("Figure Color: ");
		ImGui::ColorEdit3("Edite el color de la figura", color_figura);
		ImGui::Text("Background Color: ");
		ImGui::ColorEdit3("Edite el color de fondo", color_fondo);
		if (ImGui::Button("Move UP")) {
			up = up + 0.2f;
		}
		if (ImGui::Button("Move DOWN")) {
			down = down - 0.2f;
		}
		if (ImGui::Button("Move RIGHT")) {
			right = right + 0.2f;
		}
		if (ImGui::Button("Move LEFT")) {
			left = left - 0.2f;
		}
	ImGui::End();

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor((double)color_fondo[0], (double)color_fondo[1], (double)color_fondo[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);
	glBindVertexArray(vao[0]);

	// se obtienen los uniforms del vertex y el fragment shader
	GLuint uniformColor = glGetUniformLocation(renderingProgram, "u_color");
	GLuint uniformModel = glGetUniformLocation(renderingProgram, "u_model");

	//Borde de la cabeza
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.213f, 0.213f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f + left + right, 1.28f + up + down, 0.0f));

	glUniform3f(uniformColor, 20. / 255.0f, 25. / 255.0f, 14. / 255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Cabeza
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f + left + right, 1.36f + up + down, 0.0f));

	glUniform3f(uniformColor, color_figura[0], color_figura[1], color_figura[2]);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Borde Oreja izquierda
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.088f, 0.088f, 1.0f));
	model = glm::translate(model, glm::vec3(-1.82f + left + right, 5.2f + up + down, 0.0f));

	glUniform3f(uniformColor, 20. / 255.0f, 25. / 255.0f, 14. / 255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Oreja izquierda
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.078f, 0.078f, 1.0f));
	model = glm::translate(model, glm::vec3(-2.06f + left + right, 5.86f + up + down, 0.0f));


	glUniform3f(uniformColor, color_figura[0], color_figura[1], color_figura[2]);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Borde Oreja derecha
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.088f, 0.088f, 1.0f));
	model = glm::translate(model, glm::vec3(1.82f + left + right, 5.2f + up + down, 0.0f));

	glUniform3f(uniformColor, 20. / 255.0f, 25. / 255.0f, 14. / 255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	//Oreja derecha
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.078f, 0.078f, 1.0f));
	model = glm::translate(model, glm::vec3(2.06f + left + right, 5.86f + up + down, 0.0f));

	glUniform3f(uniformColor, color_figura[0], color_figura[1], color_figura[2]);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	// Cola linea
	model = glm::mat4(1.0f);

	model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.36f, 0.36f, 1.0f));
	model = glm::translate(model, glm::vec3(-1.0f + left + right, -0.5f + up + down, 0.0f));

	glUniform3f(uniformColor, 0. / 255.0f, 0. / 255.0f, 0. / 255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices / 2 + 1);
	// Cola fondo
	model = glm::mat4(1.0f);

	model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.34f, 0.34f, 1.0f));
	model = glm::translate(model, glm::vec3(-1.0f + left + right, -0.51f + up + down, 0.0f));

	glUniform3f(uniformColor, color_fondo[0], color_fondo[1], color_fondo[2]);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices / 2 + 1);

	//Borde del Cuerpo
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.363f, 0.363f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f + left + right, -0.815f + up + down, 0.0f));

	glUniform3f(uniformColor, 30. / 255.0f, 12. / 255.0f, 63. / 255.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	// Cuerpo
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f + left + right, -0.84f + up + down, 0.0f));

	glUniform3f(uniformColor, color_figura[0], color_figura[1], color_figura[2]);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	glBindVertexArray(0);
	glUseProgram(0);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.
	//Antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mouse	", NULL, NULL);
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